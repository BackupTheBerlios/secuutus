/* {{{ ---- [ ui.c: User Interface (UI) functions ] ------------------------- */
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
/* }}} ---- [ ui.c: User Interface (UI) functions ] ------------------------- */

/* {{{ ---- [ Includes ] ---------------------------------------------------- */
#define _GNU_SOURCE
#include <assert.h>
#include <ctype.h>
#include <curses.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "ui.h"
#include "error-reporting.h"
#include "mm.h"
/* }}} ---- [ Includes ] ---------------------------------------------------- */

/* UI object. */
ui_t *ui;

/* {{{ static inline void init_curses() */
static inline void
init_curses (void)
{

	if (!initscr())
	{
		fprintf(stderr, "initscr() failed\n");
		exit(EXIT_FAILURE);
	}

	cbreak();
	noecho();
	idlok(stdscr, TRUE);
	intrflush(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE);
	clear();
}
/* }}} */

/* {{{ static inline void term_disable_interrupts() */
static inline void
term_disable_interrupts (void)
{
	struct termios tios;

	if (tcgetattr(STDIN_FILENO, &tios) < 0)
	{
		fprintf(stderr, "tcgetattr() failed\n");
		exit(EXIT_FAILURE);
	}

	tios.c_cc[VINTR] = _POSIX_VDISABLE;;
	tios.c_cc[VQUIT] = _POSIX_VDISABLE;
	if (tcsetattr(STDIN_FILENO, TCSADRAIN, &tios) < 0)
	{
		fprintf(stderr, "tcsetattr() failed\n");
		exit(EXIT_FAILURE);
	}

}
/* }}} */

/* {{{ static void top_win_init(ui) */
static void
top_win_init (ui_t *ui)
{
	ui->top_win = smalloc(sizeof(*ui->top_win));
	memset(ui->top_win, 0, sizeof(*ui->top_win));
	getmaxyx(stdscr, ui->top_win->rows, ui->top_win->columns);
	ui->top_win->screen = stdscr;
	ui->top_win->parent = NULL;
	ui->top_win->end.y = ui->top_win->rows;
	ui->top_win->end.x = ui->top_win->columns - 1;
}
/* }}} */

/* {{{ static void tbar_init(ui) */
static void
tbar_init (ui_t *ui)
{
	ui->tbar_win = smalloc(sizeof(*ui->tbar_win));
	memset(ui->tbar_win, 0, sizeof(*ui->tbar_win));
	ui->tbar_win->parent = ui->top_win;
	++ui->tbar_win->parent->ref_count;
	ui->tbar_win->rows = 1;
	ui->tbar_win->columns = ui->top_win->columns;
	ui->tbar_win->end.x = ui->tbar_win->columns - 1;
}
/* }}} */

/* {{{ static void main_win_init(ui) */
static void
main_win_init (ui_t *ui)
{
	ui->main_win = smalloc(sizeof(*ui->main_win));
	memset(ui->main_win, 0, sizeof(*ui->main_win));
	ui->main_win->parent = ui->top_win;
	++ui->main_win->parent->ref_count;
	ui->main_win->rows = ui->top_win->rows - 3;
	ui->main_win->columns = ui->top_win->columns;
	ui->main_win->start.y = ui->tbar_win->end.y + 1;
	ui->main_win->end.y = ui->main_win->rows;
	ui->main_win->end.x = ui->main_win->columns - 1;
}
/* }}} */

/* {{{ static void sbar_init(ui) */
static void
sbar_init (ui_t *ui)
{
	ui->sbar_win = smalloc(sizeof(*ui->sbar_win));
	memset(ui->sbar_win, 0, sizeof(*ui->sbar_win));
	ui->sbar_win->parent = ui->top_win;
	++ui->sbar_win->parent->ref_count;
	ui->sbar_win->rows = 1;
	ui->sbar_win->columns = ui->top_win->columns;
	ui->sbar_win->start.y = ui->main_win->end.y + 1;
	ui->sbar_win->end.y = ui->main_win->end.y + 1;
	ui->sbar_win->end.x = ui->sbar_win->columns - 1;
}
/* }}} */

/* {{{ static void ibar_init(ui) */
static void
ibar_init (ui_t *ui)
{
	ui->ibar_win = smalloc(sizeof(*ui->ibar_win));
	memset(ui->ibar_win, 0, sizeof(*ui->ibar_win));
	ui->ibar_win->parent = ui->top_win;
	++ui->ibar_win->parent->ref_count;
	ui->ibar_win->rows = 1;
	ui->ibar_win->columns = ui->top_win->columns;
	ui->ibar_win->start.y = ui->sbar_win->end.y + 1;
	ui->ibar_win->end.y = ui->sbar_win->end.y + 1;
	ui->ibar_win->end.x = ui->ibar_win->columns - 1;
}
/* }}} */

/* {{{ ui_t *ui_create() */
ui_t *
ui_create (void)
{
	ui_t *ui = smalloc(sizeof(*ui));

	init_curses();
	term_disable_interrupts();

	top_win_init(ui);
	tbar_init(ui);
	main_win_init(ui);
	sbar_init(ui);
	ibar_init(ui);

	return ui;
}
/* }}} */

/* {{{ static void tbar_draw(ui) */
static void
tbar_draw (ui_t *ui)
{
	int i;

	assert(ui != NULL);
	if (!ui)
		return;

	move(ui->tbar_win->start.y, ui->tbar_win->start.x);
	wattron(UI_SCREEN(ui->tbar_win), A_REVERSE);
	waddstr(UI_SCREEN(ui->tbar_win), UI_TITLE);
	for (i = UI_TITLE_LEN; i < ui->tbar_win->columns; ++i)
		waddch(UI_SCREEN(ui->tbar_win), ' ');
	wattroff(UI_SCREEN(ui->tbar_win), A_REVERSE);
	wrefresh(UI_SCREEN(ui->tbar_win));
}
/* }}} */

/* {{{ static void sbar_draw(ui) */
static void
sbar_draw (ui_t *ui)
{
	int i;

	assert(ui != NULL);
	move(ui->sbar_win->start.y, ui->sbar_win->start.x);
	wattron(UI_SCREEN(ui->sbar_win), A_REVERSE);
	for (i = 0; i < ui->sbar_win->columns; ++i)
		waddch(UI_SCREEN(ui->sbar_win), ' ');
	wattroff(UI_SCREEN(ui->sbar_win), A_REVERSE);
	wrefresh(UI_SCREEN(ui->sbar_win));
}
/* }}} */

/* {{{ static void sbar_set_text(ui, text) */
static void
sbar_set_text (ui_t *ui, char *text)
{
	size_t	text_len = strlen(text);
	char	c;

	move(ui->sbar_win->start.y, ui->sbar_win->start.x);
	wattron(UI_SCREEN(ui->sbar_win), A_REVERSE);
	if (text_len <= ui->sbar_win->columns)
		waddstr(UI_SCREEN(ui->sbar_win), text);
	else
	{
		c = text[ui->sbar_win->columns - 3];
		text[ui->sbar_win->columns - 3] = '\0';
		waddstr(UI_SCREEN(ui->sbar_win), text);
		waddstr(UI_SCREEN(ui->sbar_win), "...");
		text[ui->sbar_win->columns - 3] = c;
	}
	wattroff(UI_SCREEN(ui->sbar_win), A_REVERSE);
	wrefresh(UI_SCREEN(ui->sbar_win));
}
/* }}} */

/* {{{ static void main_win_draw(ui) */
static void
main_win_draw (ui_t *ui)
{
	int i;

	assert(ui != NULL);
	for (i = 0; i < ui->main_win->rows; ++i)
		mvwaddch(UI_SCREEN(ui->main_win),
			ui->main_win->start.y + i,
			ui->main_win->start.x, '~');
	wrefresh(UI_SCREEN(ui->main_win));
}
/* }}} */

/* {{{ static char *input_read_line(ui) */
static char *
input_read_line (ui_t *ui)
{
	char	*line;
	size_t	line_size = 32,
		line_len = 0;
	int	c, x, y;

	line = smalloc(line_size);
	for (;;)
	{
		c = wgetch(UI_SCREEN(ui->top_win));
		if (c == ERR)
			continue;

		/*
		 * For some reason, [n]curses doesn't seem to reliably return
		 * KEY_ENTER when the enter/return keys are pressed.  Some
		 * digging on the 'net turned up this hack; making the ASCII
		 * character 10 and KEY_ENTER synonymous, which does work
		 * reliably (at least with on my system with ncurses-5.6).
		 *
		 * -- jfvd
		 */
		if (c == KEY_ENTER || c == 10)
			break;
		else if (c == KEY_BACKSPACE)
		{
			if (!line_len)
				continue;

			--line_len;
			getyx(stdscr, y, x);
			mvaddch(y, x - 1, ' ');
			move(y, x - 1);
			continue;
		}
		else if (iscntrl(c))
			continue;

		if (line_len > line_size)
		{
			line_size += 32;
			line = srealloc(line, line_size);
		}

		line[line_len++] = c;
		waddch(UI_SCREEN(ui->top_win), c);
	}
	line[line_len] = '\0';

	return line;
}
/* }}} */

/* {{{ static void ibar_clear(ui) */
static void
ibar_clear (ui_t *ui)
{
	int i;

	assert(ui != NULL);
	move(ui->ibar_win->start.y, ui->ibar_win->start.x);
	for (i = 0; i < ui->ibar_win->columns; ++i)
		waddch(UI_SCREEN(ui->ibar_win), ' ');
	move(ui->ibar_win->start.y, ui->ibar_win->start.x);
	wrefresh(UI_SCREEN(ui->ibar_win));
}
/* }}} */

/* {{{ int8_t ui_main() */
int8_t
ui_main (ui_t *ui)
{
	char *line = NULL, *cmd = NULL;

	assert(ui != NULL);
	if (!ui)
	{
		errno = EINVAL;
		return -1;
	}

	tbar_draw(ui);
	sbar_draw(ui);
	main_win_draw(ui);
	move(ui->ibar_win->start.y, ui->ibar_win->start.x);
	wrefresh(UI_SCREEN(ui->ibar_win));

	for (;;)
	{
		line = input_read_line(ui);

		cmd = strtok(line, " ");
		if (!cmd)
			cmd = line;

		if (!strcasecmp(cmd, "quit"))
		{
			ibar_clear(ui);
			free(line);
			break;
		}
		else
		{
			char *error = NULL;
			asprintf(&error, "Error: Unknown command '%s'", cmd);
			sbar_set_text(ui, error);
			free(error);
		}

		ibar_clear(ui);
	}

	return 0;
}
/* }}} */

/* {{{ void ui_destroy(ui) */
void
ui_destroy (ui_t *ui)
{
	assert(ui != NULL);
	if (!ui)
		return;

	endwin();

	free(ui->tbar_win);
	free(ui->main_win);
	free(ui->sbar_win);
	free(ui->ibar_win);
	free(ui->top_win);
	free(ui);

	report_error = console_report_error;
}
/* }}} */

/*
 * vim: ts=8 sw=8 noet fdm=marker :
 */
