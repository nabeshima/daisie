#
# $Id$
# Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
# This file is subject to the MIT license available at,
# http://opensource.org/licenses/mit-license.php
#

include ../Makefile.usrdef

TARGETS= \
	ex1.exe \
	ex2.exe \
	ex3.exe \
	ex4.exe \

ifneq (${shell if pkg-config --exists ode; then echo DUMMY; fi},)
ODE_CFLAGS=`pkg-config --cflags ode`
ODE_LDFLAGS=`pkg-config --libs ode`
endif

CFLAGS=-I$(INSTALLPATH)/include/daisie $(USR_CFLAGS) $(ODE_CFLAGS)
LDFLAGS=-L$(INSTALLPATH)/lib -ldaisie $(USR_LDFLAGS) $(ODE_LDFLAGS)


all: $(TARGETS)

%.exe: %.o
	$(CXX) $< $(CFLAGS) -o $@ $(LDFLAGS) $(LIBTARGET)

.cpp.o:
	$(CXX) -c $< $(CFLAGS) -o $@

install uninstall:

clean distclean:
	rm -rf *~ .#* *.dSYM $(TARGETS) *.o
