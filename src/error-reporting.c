/* {{{ ---- [ error-reporting.c: Error reporting/handling functions ] ------- */
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
/* }}} ---- [ error-reporting.c: Error reporting/handling functions ] ------- */

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error-reporting.h"

report_error_func_t report_error;

/* {{{ void console_report_error(level, format, ...) */
void
console_report_error (uint16_t level, const char *format, ...)
{
	char	c = '*';
	FILE	*fp = stdout;
	va_list	args;

	if (!level)
		return;

	if (level & L_ERROR)
		c = 'E';
	if (level & L_WARNING)
		c = 'W';
	if (level & L_NOTICE)
		c = 'N';
	if (level & L_DEBUG)
		c = 'D';

	if (!(level & L_NOTICE))
		fp = stderr;

	fflush(stdout);
	fflush(stderr);

	fprintf(fp, "(%c) ", c);
	va_start(args, format);
	vfprintf(fp, format, args);
	va_end(args);

	if (c == 'E' && errno)
	{
		fprintf(fp, ": %s (errno %d)", strerror(errno), errno);
		/*
		 * Clear errno so that if we're is called more than
		 * once, we only print the error message corresponding
		 * to errno once.
		 */
		errno = 0;
	}

	fputc('\n', fp);
	fflush(fp);

	if (level & L_DEATH)
		exit(EXIT_FAILURE);
}
/* }}} */

/*
 * vim: ts=8 sw=8 noet fdm=marker :
 */
