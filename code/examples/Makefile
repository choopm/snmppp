# SNMPpp: https://sourceforge.net/p/snmppp/
# SNMPpp project uses the MIT license. See LICENSE for details.
# Copyright (C) 2013 Stephane Charette <stephanecharette@gmail.com>


CPPOBJS=		01_oid.o		\
		02_sessions.o	\
		03_get.o

EXAMPLES=	01_oid.out	\
		02_sessions.out	\
		03_get.out

CXXFLAGS	=	-ggdb -O0 -Wall -rdynamic -I../include

LIBS=		-lsnmp

.PHONY: all
all: $(EXAMPLES) Makefile

.PHONY: clean
clean:
	rm -rf $(CPPOBJS) $(CPPOBJS:.o=.d) $(EXAMPLES)

# pull in dependency info for all .o files
-include $(CPPOBJS:.o=.d)

%.o: %.cpp Makefile
	$(CXX) $(CXXFLAGS) -c     $*.cpp -o $@
	$(CXX) $(CXXFLAGS) -c -MM $*.cpp > $*.d

%.out: %.o Makefile ../libsnmppp.a
	$(CXX) $(CXXFLAGS) $*.o ../libsnmppp.a $(LIBS) -o $@
