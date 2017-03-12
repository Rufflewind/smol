#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int handle(char **args, int count, int *fptr) {
    assert(count > 0); // shouldn't happen
    if (args[0][0] == '#') {
        return 0;
    }
    if (strcmp(args[0], "mov") == 0) {
        const char *star1 = "";
        const char *star2 = "";
        const char *star1b = "";
        const char *star2b = "";
        if (count != 3) {
            return 1;
        }
        if (args[1][0] == '*') {
            star1 = "*MGET(";
            star1b = ")";
            ++args[1];
        }
        if (args[2][0] == '*') {
            star2 = "*MGET(";
            star2b = ")";
            ++args[2];
        }
        printf("*MGET(%s%i%s) = *MGET(%s%i%s);\n",
               star1, atoi(args[1]), star1b,
               star2, atoi(args[2]), star2b);
        return 0;
    }
    if (strcmp(args[0], "set") == 0) {
        if (count != 3) {
            return 1;
        }
        printf("*MGET(%i) = %i;\n",
               atoi(args[1]), atoi(args[2]));
        return 0;
    }
    if (strcmp(args[0], "setstr") == 0) {
        int i, n = strlen(args[2]);
        if (count != 3) {
            return 1;
        }
        printf("*MGET(%i) = %i;\n", atoi(args[1]), n);
        for (i = 0; i < n; ++i) {
            printf("*MGET(%i) = %i;\n",
                   atoi(args[1]) + i + 1, args[2][i]);
        }
        return 0;
    }
    if (strcmp(args[0], "add") == 0) {
        if (count != 4) {
            return 1;
        }
        printf("*MGET(%i) = *MGET(%i) + *MGET(%i);\n",
               atoi(args[1]), atoi(args[2]), atoi(args[3]));
        return 0;
    }
    if (strcmp(args[0], "sub") == 0) {
        if (count != 4) {
            return 1;
        }
        printf("*MGET(%i) = *MGET(%i) - *MGET(%i);\n",
               atoi(args[1]), atoi(args[2]), atoi(args[3]));
        return 0;
    }
    if (strcmp(args[0], "mul") == 0) {
        if (count != 4) {
            return 1;
        }
        printf("*MGET(%i) = *MGET(%i) * *MGET(%i);\n",
               atoi(args[1]), atoi(args[2]), atoi(args[3]));
        return 0;
    }
    if (strcmp(args[0], "div") == 0) {
        if (count != 4) {
            return 1;
        }
        printf("*MGET(%i) = *MGET(%i) / *MGET(%i);\n",
               atoi(args[1]), atoi(args[2]), atoi(args[3]));
        return 0;
    }
    if (strcmp(args[0], "not") == 0) {
        if (count != 3) {
            return 1;
        }
        printf("*MGET(%i) = !*MGET(%i);\n",
               atoi(args[1]), atoi(args[2]));
        return 0;
    }
    if (strcmp(args[0], "lt") == 0) {
        if (count != 4) {
            return 1;
        }
        printf("*MGET(%i) = *MGET(%i) < *MGET(%i);\n",
               atoi(args[1]), atoi(args[2]), atoi(args[3]));
        return 0;
    }
    if (strcmp(args[0], "call") == 0) {
        int n = atoi(args[1]);
        if (count != 3) {
            return 1;
        }
        printf("*MGET(2) += %i;\n", n);
        printf("*MGET(-29999) = %i;\n", n);
        printf("*MGET(-30000) = %i;\n", *fptr);
        printf("goto %s;\n", args[2]);
        printf("case %i:\n", *fptr);
        ++*fptr;
        return 0;
    }
    if (strcmp(args[0], "ret") == 0) {
        if (count != 1) {
            return 1;
        }
        printf("fptr = *MGET(-30000);\n");
        printf("*MGET(2) -= *MGET(-29999);\n");
        printf("break;\n");
        return 0;
    }
    if (strcmp(args[0], "get") == 0) {
        if (count != 2) {
            return 1;
        }
        printf("{ int c = getchar(); *MGET(%i) = c == EOF ? -1 : c; }\n",
               atoi(args[1]));
        return 0;
    }
    if (strcmp(args[0], "put") == 0) {
        if (count != 2) {
            return 1;
        }
        printf("putc(*MGET(%i), *MGET(4) ? stderr : stdout);\n",
               atoi(args[1]));
        return 0;
    }
    if (strcmp(args[0], "debug") == 0) {
        if (count != 2) {
            return 1;
        }
        printf("fprintf(stderr, \"<<<[DEBUG]: %%i>>>\\n\", *MGET(%i));\n",
               atoi(args[1]));
        return 0;
    }
    if (strcmp(args[0], "jmp") == 0) {
        if (count != 3) {
            return 1;
        }
        printf("if (*MGET(%i)) goto %s;\n", atoi(args[1]), args[2]);
        return 0;
    }
    if (strcmp(args[0], ".") == 0) {
        if (count != 2) {
            return 1;
        }
        printf("%s:\n", args[1]);
        return 0;
    }
    return 1;
}

int main(void)
{
    int c;
    char line[1024] = {0};
    size_t i = 0, j;
    int fptr = 1;
    int count = 0;
    char *args[32] = {line};
    int in_string = 0, escaping = 0;
    printf("#include<stdio.h>\n#include<stdlib.h>\n");
    printf("static int *mem;\n");
    printf("int *MGET(int x)\n{\n");
    printf("if (x < -30000 || x >= 60000) { ");
    printf("fprintf(stderr, \"segfault at %%i\\n\", x); abort(); }\n");
    printf("if (x < 0) { return mem + 30000 + ((long)*MGET(2) - x) %% 30000; }\n");
    printf("return mem + x;\n}\n");
    printf("int main(void)\n{\n");
    printf("mem = (int *)malloc(60000 * sizeof(*mem));\n");
    printf("int fptr = 0;\n");
    printf("while (1) {\n");
    printf("switch (fptr) {\n");
    printf("case 0:\n");
    while ((c = getchar()) != EOF) {
        if (in_string) {
            if (!escaping) {
                if (c == '"') {
                    in_string = 0;
                } else if (c == '\\') {
                    escaping = 1;
                } else {
                    line[i] = c;
                    ++i;
                }
            } else {
                line[i] = c;
                ++i;
                escaping = 0;
            }
        } else if (c == '\n') {
            line[i] = '\0';
            if (i != 0) {
                ++count;
                if (handle(args, count, &fptr)) {
                    fprintf(stderr, "invalid:");
                    for (j = 0; j < count; ++j) {
                        fprintf(stderr, " %s", args[j]);
                    }
                    fprintf(stderr, "\n");
                    exit(EXIT_FAILURE);
                }
            }
            i = 0;
            count = 0;
        } else if (c == ' ') {
            if (i > 0 && line[i - 1] != '\0') {
                line[i] = '\0';
                ++i;
                ++count;
                args[count] = line + i;
            }
        } else if (c == '"') {
            in_string = 1;
        } else {
            line[i] = c;
            ++i;
        }
    }
    printf("return *MGET(3) ? EXIT_FAILURE : 0;\n");
    printf("}\n}\n}\n");
}
