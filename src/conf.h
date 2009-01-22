/* {{{ ---- [ conf.h: Configuration file API ] ------------------------------ */
/*
 * Copyright (C) 2009 Jon-Francis Bhreitheamhnaigh.
 * All rights reserved.
 *
 * This file is part of Secuutus.
 *
 * License:
 *
 * 	Secuutus is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License, version 2,
 *	as published by the Free Software Foundation.
 * 
 * 	Secuutus is distributed in the hope that it will be useful, but
 * 	WITHOUT ANY WARRANTY; without even the implied warranty of
 * 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * 	GNU General Public License for more details.
 * 
 * 	You should have received a copy of the GNU General Public License
 * 	along with Secuutus; if not, write to:
 *
 *		Free Software Foundation, Inc.
 *		59 Temple Place
 *		Suite 330
 *		Boston, MA 02111-1307
 *		USA
 *
 * Authors:
 *
 *	Jon-Francis Bhreitheamhnaigh <j.f.vd@adamo-recordings.co.uk>
 *
 */
/* }}} ---- [ conf.h: Configuration file API ] ------------------------------ */

#ifndef SECUUTUS_CONF_H
# define SECUUTUS_CONF_H

# include <libconfig.h>
# include <stdio.h>
# include "list.h"

/* {{{ typedef struct { ... } conf_t */
typedef struct
{
	/* Configuration file parser. */
	config_t *config;

	/* {{{ struct { ... } file */
	struct
	{
		/* Configuration filename. */
		char *path;

		/* Configuration file stdio stream. */
		FILE *stream;

	} file;
	/* }}} */

	/* {{{ struct { ... } settings */
	struct
	{
		/* File monitors. */
		list_t *monitors;

		/* Monitor windows. */
		list_t *windows;

	} settings;
	/* }}} */

} conf_t;
/* }}} */

conf_t *	conf_create (void);
int8_t		conf_open (conf_t *, const char *);
int8_t		conf_parse (conf_t *);
uint32_t	conf_error_line (conf_t *);
const char *	conf_error_text (conf_t *);
int8_t		conf_destroy (conf_t *);

#endif /* ! SECUUTUS_CONF_H */

/*
 * vim: ts=8 sw=8 noet fdm=marker :
 */
