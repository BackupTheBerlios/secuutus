/* {{{ ---- [ ui.h: User Interface (UI) functions ] ------------------------- */
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
/* }}} ---- [ ui.h: User Interface (UI) functions ] ------------------------- */

#ifndef SECUUTUS_UI_H
# define SECUUTUS_UI_H

# include <curses.h>
# include <stdint.h>
# include "config.h"

# define UI_SCREEN(win) \
	((win)->parent ? (win)->parent->screen : (win)->screen)

# define UI_TITLE	"Secuutus, version " PACKAGE_VERSION
# define UI_TITLE_LEN	sizeof(UI_TITLE) - 1

/* {{{ typedef struct ui_window { ... } ui_window_t */
typedef struct ui_window
{
	/* Curses window (screen). */
	WINDOW *screen;

	/* Parent window. */
	struct ui_window *parent;

	/* Reference count (number of child windows). */
	uint16_t ref_count;

	/* Window rows. */
	uint16_t rows;

	/* Window columns. */
	uint16_t columns;

	/* Start X/Y positions. */
	struct
	{
		uint16_t x, y;

	} start;

	/* End X/Y positions. */
	struct
	{
		uint16_t x, y;

	} end;

	/* Current X/Y positions. */
	struct
	{
		uint16_t x, y;

	} cur;

} ui_window_t;
/* }}} */

/* {{{ typedef struct { ... } ui_t */
typedef struct
{
	/* Top-level window. */
	ui_window_t *top_win;

	/* Title bar window. */
	ui_window_t *tbar_win;

	/* Main display window. */
	ui_window_t *main_win;

	/* Status bar window. */
	ui_window_t *sbar_win;

	/* Input bar window. */
	ui_window_t *ibar_win;

} ui_t;
/* }}} */

extern ui_t *ui;

ui_t *	ui_create (void);
char *	ui_read_line (void);
void	ui_report_error (uint16_t, const char *, ...);
int8_t	ui_main (ui_t *);
void	ui_destroy (ui_t *);

#endif /* ! SECUUTUS_UI_H */

/*
 * vim: ts=8 sw=8 noet fdm=marker :
 */
