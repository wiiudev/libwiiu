ici_stub:
	# Enable machine check exceptions and exception vectors at 0xFFFxxxxx
	li r3, 0x1040
	mtmsr r3
	isync
	
	# Disable the L2 cache
	mfspr r3, 0x3f9
	lis r4, 0x7FFF
	ori r4, r4, 0xFFFF
	and r3, r3, r4
	mtspr 0x3f9, r3
	sync
	
	# Disable caching (ICache and DCache) in HID0
	mfspr r3, 0x3f0
	lis r4, 0xFFFF
	ori r4, r4, 0x3FFF
	and r3, r3, r4
	mtspr 0x3f0, r3
	sync
.loop:
	# Infinite loop (until replaced)
	b loop
