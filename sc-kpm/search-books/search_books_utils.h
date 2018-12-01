/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _search_books_utils_h_
#define _search_books_utils_h_

extern "C"
{
#include "sc_memory.h"
}

#include <vector>


void utils_append_to_pattern(sc_addr pattern, sc_addr element);
void utils_append_to_pattern(sc_addr pattern, std::vector<sc_addr> elements);

sc_addr utils_node_new(sc_type type);
sc_addr utils_node_new_const();
sc_addr utils_node_new_var();

sc_addr utils_arc_new(sc_type type, sc_addr begin, sc_addr end);
sc_addr utils_arc_new_const(sc_addr begin, sc_addr end);
sc_addr utils_arc_new_var(sc_addr begin, sc_addr end);

sc_addr get_agent_parameter(sc_addr parameters, sc_addr rrel_ordinal);
sc_addr get_book_from_pattern(sc_addr pattern);

#endif
