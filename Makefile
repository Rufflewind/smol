smolc: smolc.smol smolc_orig
	./smolc_orig <smolc.smol >smolc.c
	cc smolc.c -o $@

smolc_orig: smolc_orig.c
	cc smolc_orig.c -o $@
