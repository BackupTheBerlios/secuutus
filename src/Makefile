# {{{
#
# src/Makefile
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

topdir = ..
include $(topdir)/Makefile.top

all_BIN = secuutus

secuutus_SOURCES = 		\
	conf.c			\
	error-reporting.c	\
	mm.c			\
	main.c			\
	list.c			\
	ui.c
secuutus_OBJECTS = $(patsubst %.c,%.o,$(secuutus_SOURCES)) 

.PHONY: all
all: $(all_BIN)

.PHONY: deps
deps:
	$(MKDEPS) $(CC) '$(CFLAGS)' '$(secuutus_SOURCES)'

%.o: %.c
	$(call wrap_CC,,$<,$@)

secuutus: $(secuutus_OBJECTS)
	$(call wrap_LD,$@,$^,)

.PHONY: install install-bin
install: all install-bin
	
install-bin: $(all_BIN)
	@$(INSTALL_DIR) $(DESTDIR)$(bindir) || exit 1;			\
	list='$(all_BIN)'; for bin in $$list; do			\
		echo "Installing $(DESTDIR)$(bindir)/$$bin...";		\
		$(INSTALL_BIN) $$bin $(DESTDIR)$(bindir) || exit 1;	\
	done

.PHONY: clean distclean
clean:
	-rm -f $(all_BIN) $(secuutus_OBJECTS)
distclean: clean

-include Makefile.deps

# vim: ts=8 sw=8 noet fdm=marker :
