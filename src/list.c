/* {{{ ---- [ list.c: Linked-list (list_*) API ] ---------------------------- */
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
/* }}} ---- [ list.c: Linked-list (list_*) API ] ---------------------------- */

#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include "list.h"
#include "mm.h"

/*
 * list_init()		- statically initialise a list object.
 * list_create()	- create a new linked list object.
 * list_append()	- append an item to the head of the list.
 * list_prepend()	- prepend an item to the tail of the list.
 * list_delete()	- delete an item from the list.
 * list_apply()		- apply a function to all items in a list.
 * list_clear()		- clear (free) all nodes in a list.
 * list_destroy()	- free memory associated with list object and its nodes.
 */

/* {{{ int8_t list_init(list) */
int8_t
list_init (list_t *list)
{
	assert(list != NULL);
	if (!list)
	{
		errno = EINVAL;
		return -1;
	}

	list->head = NULL;
	list->tail = NULL;
	list->length = 0;

	return 0;
}
/* }}} */

/* {{{ list_t *list_create() */
list_t *
list_create (void)
{
	list_t *list = NULL;

	list = smalloc(sizeof(*list));
	if (list_init(list) < 0)
	{
		free(list);
		return NULL;
	}

	return list;
}
/* }}} */

/* {{{ int8_t list_append(list, data) */
int8_t
list_append (list_t *list, void *data)
{
	list_node_t *new = NULL;

	assert(list != NULL);
	assert(data != NULL);
	if (!list || !data)
	{
		errno = EINVAL;
		return -1;
	}

	new = smalloc(sizeof(*new));
	new->prev = NULL;
	new->next = NULL;
	new->data = data;

	if (!list->head)
	{
		list->head = new;
		list->tail = list->head;
	}
	else
	{
		new->prev = list->tail;
		list->tail->next = new;
		list->tail = new;
	}

	++list->length;
	return 0;
}
/* }}} */

/* {{{ int8_t list_prepend(list, data) */
int8_t
list_prepend (list_t *list, void *data)
{
	list_node_t *new = NULL;

	assert(list != NULL);
	assert(data != NULL);
	if (!list || !data)
	{
		errno = EINVAL;
		return -1;
	}

	new = smalloc(sizeof(*new));
	new->prev = NULL;
	new->next = NULL;
	new->data = data;

	if (!list->head)
	{
		list->head = new;
		list->tail = list->head;
	}
	else
	{
		new->next = list->head;
		list->head->prev = new;
		list->head = new;
	}

	++list->length;
	return 0;
}
/* }}} */

/* {{{ int8_t list_delete(list, data) */
int8_t
list_delete (list_t *list, void *data)
{
	list_node_t *cur = NULL;

	assert(list != NULL);
	assert(data != NULL);
	if (!list || !data)
	{
		errno = EINVAL;
		return -1;
	}

	cur = list->head;
	while (cur)
	{
		if (cur->data != data)
			continue;

		if (cur->next)
			cur->next->prev = cur->prev;
		else
			list->tail = cur->prev;

		if (cur->prev)
			cur->prev->next = cur->next;
		else
			list->head = cur->next;

		free(cur);
		--list->length;
		return 1;
	}

	return 0;
}
/* }}} */

/* {{{ int32_t list_apply(list, func, arg) */
int32_t
list_apply (list_t *list, list_apply_func_t func, void *arg)
{
	list_node_t	*cur = NULL;
	int32_t		i;

	assert(list != NULL);
	assert(func != NULL);
	if (!list || !func)
	{
		errno = EINVAL;
		return -1;
	}

	/* Empty list? */
	if (!list->head)
		return 0;

	i = 0;
	LIST_FOREACH (list, cur)
	{
		func(cur->data, arg);
		++i;
	}

	return i;
}
/* }}} */

/* {{{ void list_clear(list) */
void
list_clear (list_t *list)
{
	list_node_t *cur = NULL;
	list_node_t *next = NULL;

	assert(list != NULL);
	if (!list || !list->head)
		return;

	cur = list->head;
	while (cur)
	{
		next = cur->next;
		free(cur);
		cur = next;
		--list->length;
	}
}
/* }}} */

/* {{{ void list_destroy(list) */
void
list_destroy (list_t *list)
{
	assert(list != NULL);
	if (!list)
		return;

	if (list->head)
		list_clear(list);
	free(list);
}
/* }}} */

/*
 * vim: ts=8 sw=8 noet fdm=marker :
 */
