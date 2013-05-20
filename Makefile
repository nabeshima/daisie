#
# $Id$
# Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
# This file is subject to the MIT license available at,
# http://opensource.org/licenses/mit-license.php
#

SUBDIR =  src
EVERYTHING = all install uninstall

$(EVERYTHING):
	@for subdir in $(SUBDIR); do ( cd $$subdir && $(MAKE) $@ ); done

clean distclean:
	find . -name "*~" -exec rm {} \;
	find . -name ".#*" -exec rm {} \;
	@for subdir in $(SUBDIR); do ( cd $$subdir && $(MAKE) $@ ); done
	(cd example && $(MAKE) $@)
	(cd doc && $(MAKE) $@)

.PHONY: doc

doc:
	(cd doc && $(MAKE) doc)
