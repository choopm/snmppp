# SNMPpp: https://sourceforge.net/p/snmppp/
# SNMPpp project uses the MIT license. See LICENSE for details.
# Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>


LIBSNMPPPVER=$(shell git describe --long)

CPPOBJS=		src/OID.o	\
		src/SNMPpp.o

LIBS=		-lsnmp

# Debugging:
CXXFLAGS=	-fPIC -Wall -Werror -Iinclude -O0 -fno-omit-frame-pointer -ggdb -DDEBUG
# Profiling:
#CXXFLAGS=	-fPIC -Wall -Werror -Iinclude/SNMPpp -O3 -fno-omit-frame-pointer -ggdb
# Normal builds:
#CXXFLAGS=	-fPIC -Wall -Werror -Iinclude/SNMPpp -O3


.PHONY: all
all: libsnmppp.so libsnmppp.a
	$(MAKE) -C examples
	@echo "Done building SNMP++ ${LIBSNMPPPVER}"


.PHONY: clean
clean:
	$(MAKE) clean -C examples
	rm -rf $(CPPOBJS) $(CPPOBJS:.o=.d) libsnmppp.a libsnmppp.so


# pull in dependency info for all .o files
-include $(CPPOBJS:.o=.d)


%.o: %.cpp Makefile include/SNMPpp/Version.hpp
	$(CXX)     -c $(CXXFLAGS) $*.cpp -o $@
	$(CXX) -MM -c $(CXXFLAGS) $*.cpp > $*.d


# create a dynamic library:  libsnmppp.so
libsnmppp.so: $(CPPOBJS) Makefile
	$(CXX) -shared $(CPPOBJS) $(LIBS) -o $@


# create a static library:  libsnmppp.a
libsnmppp.a: $(CPPOBJS) Makefile
	ar cr $@ $(CPPOBJS)


VER=$(shell sed -n 's/.define LIBSNMPPPVER \"\(.*\)\"/\1/p' include/SNMPpp/Version.hpp)
include/SNMPpp/Version.hpp::
	@echo "Version in Version.hpp: ${VER}"
	@echo "Version we're building: ${LIBSNMPPPVER}"
	@if [ "${VER}" != "${LIBSNMPPPVER}" ]; then sed -e 's/${VER}/${LIBSNMPPPVER}/g' -i.old $@; rm -f $@.old; fi
