/* {{{ ---- [ main.c: main() and friends. ] --------------------------------- */
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
/* }}} ---- [ main.c: main() and friends. ] --------------------------------- */

/* {{{ ---- [ Includes ] ---------------------------------------------------- */
#include <errno.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include "error-reporting.h"
#include "mm.h"
#include "conf.h"
#include "list.h"
#include "ui.h"
/* }}} ---- [ Includes ] ---------------------------------------------------- */

#define OPT_CHECK_CONFIG 0x01

/* Short (-?) command line options. */
char short_options[] = "c:Ch";

/* Long (--foo) command line options. */
/* {{{ static struct option long_options[] = { ... } */
struct option long_options[] =
{
	{"config-file",		required_argument,	NULL, 'c'},
	{"check-config",	required_argument,	NULL, 'C'},
	{"help",		no_argument,		NULL, 'h'},
	{NULL,			0,			NULL, 0},
};
/* }}} */

/* Secuutus "environment." */
/* {{{ static struct { ... } secuutus */
static struct
{
	/* Program filename (excluding directory part). */
	const char *prog;

	/* Program options. */
	uint16_t options;

	/* Boolean indicating whether or not we've loaded our configuration file. */
	uint8_t config_file_loaded;

	/* Full path to configuration file. */
	char *config_file;

	/* Configuration settings. */
	conf_t *config;

} secuutus;
/* }}} */

/* {{{ ---- [ Prototypes ] -------------------------------------------------- */
static void	parse_args (int argc, char **argv);
static void	check_options (void);

static char *	get_config_file_path (void);
static void	parse_config (void);
static void	check_config (void);
/* }}} ---- [ Prototypes ] -------------------------------------------------- */

/* {{{ ---- [ Functions ] --------------------------------------------------- */
/* {{{ static inline void set_prog_name(argv0) */
static inline void
set_prog_name (const char *argv0)
{
	secuutus.prog = strrchr(argv0, '/');
	if (!secuutus.prog)
		secuutus.prog = argv0;
	else
		++secuutus.prog;
}
/* }}} */

/* {{{ static inline void usage() */
static inline void
usage (void)
{
	fprintf(stderr, "Usage: %s [options]\n\n", secuutus.prog);
	fprintf(stderr, "  -c, --config-file=FILE  Read configuration settings from FILE\n");
	fprintf(stderr, "  -C, --check-config      Check whether configuration file is\n");
	fprintf(stderr, "                          syntactically correct\n");
	fprintf(stderr, "  -h, --help              Display usage information\n");
	exit(EXIT_SUCCESS);
}
/* }}} */

/* {{{ int main(argc, argv) */
int
main (int argc, char **argv)
{
	ui_t *ui = NULL;

	report_error = console_report_error;
	set_prog_name(argv[0]);
	parse_args(argc, argv);
	check_options();

	if (!secuutus.config_file)
	{
		secuutus.config_file = get_config_file_path();
		if (!secuutus.config_file)
			DIE("get_config_file_path() failed");
	}

	parse_config();
	ui = ui_create();
	ui_main(ui);

	ui_destroy(ui);
	free(secuutus.config_file);
	if (conf_destroy(secuutus.config) < 0)
		DIE("conf_destroy() failed");

	return 0;
}
/* }}} */

/* {{{ static void parse_args(argc, argv) */
static void
parse_args (int argc, char **argv)
{
	int c;

	for (;;)
	{
		c = getopt_long(argc, argv, short_options, long_options, NULL);
		if (c < 0)
			break;

		switch (c)
		{
			case 'C':
				secuutus.options |= OPT_CHECK_CONFIG;
				break;
			case 'c':
				secuutus.config_file = sstrdup(optarg);
				break;
			case 'h':
				usage();
			case '?':
				break;
			default:
				DIE("getopt_long() returned unhandled character code %o", c);
		}
	}
}
/* }}} */

/* {{{ static void check_options() */
static void
check_options (void)
{
	if(secuutus.options & OPT_CHECK_CONFIG && !secuutus.config_file)
		DIE("No configuration file specified.  "
			"See '%s --help' for information",
			secuutus.prog);

	if (secuutus.options & OPT_CHECK_CONFIG)
	{
		check_config();
		N("%s: Syntax okay\n", secuutus.config_file);
		free(secuutus.config_file);
		exit(EXIT_SUCCESS);
	}
}
/* }}} */

/* {{{ static char *get_config_file_path() */
static char *
get_config_file_path (void)
{
	char	*home_dir = NULL,
		*p = NULL,
		*cwd = NULL;
	size_t	home_dir_len, p_len, cwd_size;

	home_dir = getenv("HOME");
	if (!home_dir)
	{
		cwd_size = 32;
		cwd = smalloc(cwd_size);

		for (;;)
		{
			if (getcwd(cwd, cwd_size))
				break;
			if (errno == ERANGE)
			{
				cwd_size += 32;
				cwd = srealloc(cwd, cwd_size);
				continue;
			}

			DIE("getcwd() failed");
		}

		home_dir = cwd;
	}

	home_dir_len = strlen(home_dir);
	/* p_len = strlen(home_dir) + 1 [ for "/" ] + strlen(".secuutus.conf") + 1 */
	p_len = home_dir_len + sizeof(".secuutus.conf");
	p = smalloc(p_len + 1);
	snprintf(p, p_len + 1, "%s/%s", home_dir, ".secuutus.conf");

	if (cwd)
		free(cwd);
	return p;
}
/* }}} */

/* {{{ static void parse_config() */
static void
parse_config (void)
{
	conf_t	*conf = NULL;
	uint8_t			config_opened = 0;

	conf = conf_create();
	if (!conf)
		DIE("conf_create() failed");
	if (!conf_open(conf, secuutus.config_file))
		config_opened = 1;
	else
	{
		if (errno != ENOENT)
		{
			conf_destroy(conf);
			DIE("conf_open() failed");
		}
	}

	if (config_opened)
	{
		if (conf_parse(conf) < 0)
		{
			conf_destroy(conf);
			DIE("conf_parse() failed");
		}
	}

	secuutus.config = conf;
}
/* }}} */

/* {{{ static void check_config() */
static void
check_config (void)
{
	conf_t *conf = NULL;

	conf = conf_create();
	if (!conf)
		DIE("conf_create() failed");
	if (conf_open(conf, secuutus.config_file) < 0)
		DIE("%s: conf_t() failed", secuutus.config_file);
	if (conf_parse(conf) < 0)
		DIE("%s:%d: %s",
			secuutus.config_file,
			conf_error_line(conf),
			conf_error_text(conf));
}
/* }}} */
/* }}} ---- [ Functions ] --------------------------------------------------- */

/*
 * vim: ts=8 sw=8 noet fdm=marker :
 */
