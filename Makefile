# SNMPpp: https://sourceforge.net/p/snmppp/
# SNMPpp project uses the MIT license. See LICENSE for details.
# Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>


.PHONY: all
all:
	$(MAKE) -C code

.PHONY: clean
clean:
	$(MAKE) clean -C code

