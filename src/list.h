/* {{{ ---- [ list.h: Linked-list (list_*) API ] ---------------------------- */
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
/* }}} ---- [ list.h: Linked-list (list_*) API ] ---------------------------- */

#ifndef SECUUTUS_LIST_H
# define SECUUTUS_LIST_H

# include <inttypes.h>

# define LIST_FOREACH(list, cur) \
	for ((cur) = (list)->head; (cur); (cur) = (cur)->next)

/* {{{ typedef struct { ... } list_node_t */
typedef struct list_node
{
	/* pointer to previous node in the chain. */
	struct list_node *prev;

	/* pointer to next node in the chain. */
	struct list_node *next;

	/* pointer to node data. */
	void *data;

} list_node_t;
/* }}} */

/* {{{ typedef struct { ... } list_t */
typedef struct
{
	/* list head pointer. */
	list_node_t *head;

	/* list tail pointer. */
	list_node_t *tail;

	/* list length. */
	uint32_t length;

} list_t;
/* }}} */

typedef void (*list_apply_func_t) (void *, void *);

int8_t		list_init (list_t *list);
list_t *	list_create (void);
int8_t		list_append (list_t *list, void *data);
int8_t		list_prepend (list_t *list, void *data);
int32_t		list_apply (list_t *list, list_apply_func_t func, void *arg);
void		list_clear (list_t *list);
void		list_destroy (list_t *list);

#endif /* ! SECUUTUS_LIST_H */

/*
 * vim: ts=8 sw=8 noet fdm=marker :
 */
