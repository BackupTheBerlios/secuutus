# {{{
#
# Top-level Makefile
#
# Copyright (C) 2009 Jon-Francis Bhreitheamhnaigh.
# All rights reserved.
#
# This file is part of Secuutus.
#
# License:
#
#	Secuutus is free software; you can redistribute it and/or modify
#	it under the terms of the GNU General Public License, version 2,
#	as published by the Free Software Foundation.
#
#	Secuutus is distributed in the hope that it will be useful, but
#	WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with Secuutus; if not, write to:
#
#		Free Software Foundation, Inc.
#		59 Temple Place
#		Suite 330
#		Boston, MA 02111-1307
#		USA
#
# Authors:
#
#	Jon-Francis Bhreitheamhnaigh <j.f.vd@adamo-recordings.co.uk>
#
# }}}

prog="$(basename $0)"

include Makefile.top

	SUBDIRS = src
CLEANDIRS = $(SUBDIRS)
DCLEANFILES =		\
	autom4te.cache	\
	autoscan*.log	\
	config.h*	\
	config.cache	\
	config.log	\
	config.status	\
	configure 	\
	configure.scan	\
	Makefile.top

.PHONY: all all-recursive
all: all-recursive
all-recursive: $(SUBDIRS)
	@list='$^'; for dir in $$list; do	\
		echo "... Building $$dir";	\
		$(SUBMAKE) -C $$dir || exit 1;	\
	done

.PHONY: deps deps-recursive
deps: deps-recursive
deps-recursive: $(SUBDIRS)
	@list='$^'; for dir in $$list; do				\
		echo "... Generating dependency information for $$dir";	\
		$(SUBMAKE) -C $$dir deps || exit 1;			\
	done

.PHONY: install install-local install-recursive
install: install-local install-recursive
install-local:
	@echo "... Installing top-level"
install-recursive: $(SUBDIRS)
	@list='$^'; for dir in $$list; do		\
		echo "... Installing $$dir";		\
		$(SUBMAKE) -C $$dir install || exit 1;	\
	done

.PHONY: clean clean-recursive
clean: clean-recursive
clean-recursive: $(CLEANDIRS)
	@list='$^'; for dir in $$list; do		\
		echo "... Cleaning $$dir";		\
		$(SUBMAKE) -C $$dir clean || exit 1;	\
	done

.PHONY: distclean distclean-recursive
distclean: distclean-recursive distclean-local
distclean-local:
	@echo "... Cleaning top-level for distribution"
	-rm -rf $(DCLEANFILES)
distclean-recursive: $(CLEANDIRS)
	@list='$^'; for dir in $$list; do			\
		echo "... Cleaning $$dir for distribution";	\
		$(SUBMAKE) -C $$dir distclean || exit 1;	\
	done

##
# vim: ts=8 sw=8 noet fdm=marker :
##
