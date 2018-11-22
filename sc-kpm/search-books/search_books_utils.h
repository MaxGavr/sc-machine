/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _search_books_utils_h_
#define _search_books_utils_h_

#ifdef __cplusplus
extern "C"
{
#endif

#include "sc_memory.h"

void utils_append_to_pattern(sc_addr pattern, sc_addr element);

sc_addr utils_node_new(sc_type type);
sc_addr utils_node_new_const();
sc_addr utils_node_new_var();

sc_addr utils_arc_new(sc_type type, sc_addr begin, sc_addr end);
sc_addr utils_arc_new_const(sc_addr begin, sc_addr end);
sc_addr utils_arc_new_var(sc_addr begin, sc_addr end);

#ifdef __cplusplus
}
#endif

#endif
