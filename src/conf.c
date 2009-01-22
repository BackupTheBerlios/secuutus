/* {{{ ---- [ conf.c: Configuration file API ] ------------------------------ */
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
/* }}} ---- [ conf.c: Configuration file API ] ------------------------------ */

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "conf.h"
#include "list.h"
#include "mm.h"

/* {{{ conf_t *conf_create() */
conf_t *
conf_create (void)
{
	conf_t *conf = NULL;

	conf = smalloc(sizeof(*conf));
	memset(conf, 0, sizeof(*conf));
	conf->config = smalloc(sizeof(*conf->config));
	
	return conf;
}
/* }}} */

/* {{{ int8_t conf_open(conf, path) */
int8_t
conf_open (conf_t *conf, const char *path)
{
	int		fd;
	struct		sb;
	FILE		*fp = NULL;
	struct stat	sb;

	assert(conf != NULL);
	assert(path != NULL);
	assert(*path != '\0');
	if (!conf || !path || !*path)
	{
		errno = EINVAL;
		return -1;
	}

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return -1;
	if (fstat(fd, &sb) < 0)
		return -1;
	if (!S_ISREG(sb.st_mode))
	{
		errno = EINVAL;
		return -1;
	}

	fp = fdopen(fd, "r");
	if (!fp)
		return -1;

	conf->file.path = sstrdup(path);
	conf->file.stream = fp;

	return 0;
}
/* }}} */

/* {{{ int8_t conf_parse(conf) */
int8_t
conf_parse (conf_t *conf)
{
	assert(conf != NULL);
	if (!conf)
	{
		errno = EINVAL;
		return -1;
	}

	config_init(conf->config);
	if (config_read(conf->config, conf->file.stream) != CONFIG_TRUE)
	{
		config_destroy(conf->config);
		conf->config = NULL;
		return -1;
	}

	return 0;
}
/* }}} */

/* {{{ uint32_t conf_error_line(conf) */ 
uint32_t
conf_error_line (conf_t *conf)
{
	assert(conf != NULL);
	if (!conf)
	{
		errno = EINVAL;
		return 0;
	}

	if (!conf->config)
		return 0;
	return config_error_line(conf->config);
}
/* }}} */

/* {{{ const char *conf_error_text(conf) */
const char *
conf_error_text (conf_t *conf)
{
	assert(conf != NULL);
	if (!conf)
	{
		errno = EINVAL;
		return NULL;
	}

	if (!conf->config)
		return "Configuration file not loaded";
	return config_error_text(conf->config);
}
/* }}} */

/* {{{ int8_t conf_destroy(conf) */
int8_t
conf_destroy (conf_t *conf)
{
	assert(conf != NULL);
	if (!conf)
	{
		errno = EINVAL;
		return -1;
	}

	if (conf->config)
		config_destroy(conf->config);
	free(conf->config);
	free(conf);

	return 0;
}
/* }}} */

/*
 * vim: ts=8 sw=8 noet fdm=marker :
 */
