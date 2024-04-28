/*	$NetBSD: tree.h,v 1.8 2004/03/28 19:38:30 provos Exp $	*/
/*	$OpenBSD: tree.h,v 1.7 2002/10/17 21:51:54 art Exp $	*/
/* $FreeBSD: head/sys/sys/tree.h 347360 2019-05-08 18:47:00Z trasz $ */

/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright 2002 Niels Provos <provos@citi.umich.edu>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef	_SYS_TREE_H_
#define	_SYS_TREE_H_

#define SPLAY_HEAD(name, type)  											\
struct name {                   											\
    struct type *sph_root;          										\
}

#define SPLAY_ENTRY(type)       											\
struct {                        											\
	struct type *spe_left;          										\
	struct type *spe_right;         										\
}

#define SPLAY_INITIALIZER(root)												\
	{ NULL }

#define SPLAY_INIT(root) do {												\
	(root)->sph_root = NULL;												\
} while (/*CONSTCOND*/ 0)

#define SPLAY_LEFT(elm, field)		(elm)->field.spe_left
#define SPLAY_RIGHT(elm, field)		(elm)->field.spe_right
#define SPLAY_ROOT(head)			(head)->sph_root
#define SPLAY_EMPTY(head)			(SPLAY_ROOT(head) == NULL)

#define SPLAY_PROTOTYPE(name, type, field, cmp)				        		\
void name##_SPLAY(struct name *, struct type *);			        		\
void name##_SPLAY_MINMAX(struct name *, int);				        		\
struct type *name##_SPLAY_INSERT(struct name *, struct type *);				\
struct type *name##_SPLAY_REMOVE(struct name *, struct type *);				\
/* Finds the node with the same key as elm */				        		\
static __unused inline struct type *					            		\
name##_SPLAY_FIND(struct name *head, struct type *elm) {            		\
	if (SPLAY_EMPTY(head))						                    		\
		return(NULL);						                        		\
	name##_SPLAY(head, elm);					                    		\
	if ((cmp)(elm, (head)->sph_root) == 0)				            		\
		return (head->sph_root);				                    		\
	return (NULL);						                        			\
}									                                		\
static __unused inline struct type *					            		\
name##_SPLAY_NEXT(struct name *head, struct type *elm)			    		\
{									                                		\
	name##_SPLAY(head, elm);					                    		\
	if (SPLAY_RIGHT(elm, field) != NULL) {				            		\
		elm = SPLAY_RIGHT(elm, field);				                		\
		while (SPLAY_LEFT(elm, field) != NULL) {		            		\
			elm = SPLAY_LEFT(elm, field);			                		\
		}							                                		\
	} else								                            		\
		elm = NULL;						                            		\
	return (elm);							                        		\
}									                                		\
static __unused inline struct type *					            		\
name##_SPLAY_MIN_MAX(struct name *head, int val)							\
{																			\
	name##_SPLAY_MINMAX(head, val);											\
        return (SPLAY_ROOT(head));											\
}

#define SPLAY_NEGINF	-1
#define SPLAY_INF		1

#define SPLAY_INSERT(name, x, y)	name##_SPLAY_INSERT(x, y)
#define SPLAY_REMOVE(name, x, y)	name##_SPLAY_REMOVE(x, y)
#define SPLAY_FIND(name, x, y)		name##_SPLAY_FIND(x, y)
#define SPLAY_NEXT(name, x, y)		name##_SPLAY_NEXT(x, y)
#define SPLAY_MIN(name, x)			(SPLAY_EMPTY(x) ? NULL : name##_SPLAY_MIN_MAX(x, SPLAY_NEGINF))
#define SPLAY_MAX(name, x)			(SPLAY_EMPTY(x) ? NULL : name##_SPLAY_MIN_MAX(x, SPLAY_INF))

#define SPLAY_FOREACH(x, name, head)										\
	for ((x) = SPLAY_MIN(name, head);										\
	     (x) != NULL;														\
	     (x) = SPLAY_NEXT(name, head, x))

/* Main splay operation.
 * Moves node close to the key of elm to top
 */
#define SPLAY_GENERATE(name, type, field, cmp)								\
struct type *																\
name##_SPLAY_INSERT(struct name *head, struct type *elm)					\
{																			\
    if (SPLAY_EMPTY(head)) {												\
	    SPLAY_LEFT(elm, field) = SPLAY_RIGHT(elm, field) = NULL;			\
    } else {																\
	    int __comp;															\
	    name##_SPLAY(head, elm);											\
	    __comp = (cmp)(elm, (head)->sph_root);								\
	    if(__comp < 0) {													\
		    SPLAY_LEFT(elm, field) = SPLAY_LEFT((head)->sph_root, field);	\
		    SPLAY_RIGHT(elm, field) = (head)->sph_root;						\
		    SPLAY_LEFT((head)->sph_root, field) = NULL;						\
	    } else if (__comp > 0) {											\
		    SPLAY_RIGHT(elm, field) = SPLAY_RIGHT((head)->sph_root, field);	\
		    SPLAY_LEFT(elm, field) = (head)->sph_root;						\
		    SPLAY_RIGHT((head)->sph_root, field) = NULL;					\
	    } else																\
		    return ((head)->sph_root);										\
    }																		\
    (head)->sph_root = (elm);												\
    return (NULL);															\
}	

#endif
