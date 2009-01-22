/* {{{ ---- [ error-reporting.h: Error reporting/handling functions ] ------- */
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
/* }}} ---- [ error-reporting.h: Error reporting/handling functions ] ------- */

#ifndef SECUUTUS_ERROR_REPORTING_H
# define SECUUTUS_ERROR_REPORTING_H

# include "ui.h" 

/* {{{ ---- [ Constantants and macros ] ------------------------------------- */
# define L_DEATH	0x01
# define L_ERROR	0x02
# define L_WARNING	0x04
# define L_NOTICE	0x08
# define L_DEBUG	0x10

# define N(...)		report_error(L_NOTICE, __VA_ARGS__)
# define W(...)		report_error(L_WARNING, __VA_ARGS__)
# define E(...)		report_error(L_ERROR, __VA_ARGS__)
# define D(...)		report_error(L_DEBUG, __VA_ARGS__)

# define DIE(...)							\
	do								\
	{								\
		if (ui)							\
		{							\
			ui_destroy(ui);					\
			report_error(L_ERROR | L_DEATH, __VA_ARGS__);	\
		}							\
	} while (0)

# define OPT_CHECK_CONFIG 0x01
/* }}} ---- [ Constantants and macros ] ------------------------------------- */

typedef void (*report_error_func_t) (uint16_t, const char *, ...);

extern report_error_func_t report_error;

void console_report_error (uint16_t, const char *, ...);

#endif /* ! SECUUTUS_ERROR_REPORTING_H */

/*
 * vim: ts=8 sw=8 noet fdm=marker :
 */
