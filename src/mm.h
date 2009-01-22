/* {{{ ---- [ mm.h: "Safe" version of malloc() & co. ] ---------------------- */
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
/* }}} ---- [ mm.h: "Safe" version of malloc() & co. ] ---------------------- */

#ifndef SECUUTUS_MM_H
# define SECUUTUS_MM_H

# include <stddef.h>

void *	smalloc (size_t);
void *	srealloc (void *, size_t);
char *	sstrdup (const char *);

#endif /* ! SECUUTUS_MM_H */

/*
 * vim: ts=8 sw=8 noet fdm=marker :
 */
