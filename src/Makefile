#
# $Id$
# Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
# This file is subject to the MIT license available at,
# http://opensource.org/licenses/mit-license.php
#

include ../Makefile.usrdef
include Makefile.libdef


all: $(LIBTARGET)

.cpp.o:
	$(CXX) -fPIC -c $< $(CFLAGS) -o $@

$(LIBTARGET): $(OBJS)
	$(CXX) $(OBJS) $(CFLAGS) -o $@ $(LDFLAGS) $(SHAREDFLAGS)

install: all
	mkdir -p $(INSTALLPATH)/lib $(INSTALLPATH)/include/$(LIBNAME)
	cp $(HEADERS) $(INSTALLPATH)/include/$(LIBNAME)
	cp $(LIBTARGET) $(INSTALLPATH)/lib
	$(INSTALL_COMMAND)

uninstall:
	rm -rf $(INSTALLPATH)/include/$(LIBNAME)
	$(UNINSTALL_COMMAND)

clean distclean:
	rm -f .*.d *~ .#* $(LIBTARGET) $(OBJS)


## dependency resolution
DEPENDS=$(patsubst %.o,.%.d,$(OBJS))

%.d: %.cpp
	@set -e; $(CXX) -MM $(CFLAGS) $< \
	| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > .$@; \
	[ -s $@ ] || rm -f $@

-include $(DEPENDS)
