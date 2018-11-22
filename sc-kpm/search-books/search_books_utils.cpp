/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

extern "C"
{
#include "sc_helper.h"
#include "search_books.h"
#include "search_books_utils.h"
}



void utils_append_to_pattern(sc_addr pattern, sc_addr element)
{
    if (SC_FALSE == sc_helper_check_arc(s_books_ctx, pattern, element, sc_type_arc_pos_const_perm))
        utils_arc_new_const(pattern, element);
}


sc_addr utils_node_new(sc_type type)
{
    return sc_memory_node_new(s_books_ctx, type);
}

sc_addr utils_node_new_const()
{
    return utils_node_new(sc_type_const);
}

sc_addr utils_node_new_var()
{
    return utils_node_new(sc_type_var);
}


sc_addr utils_arc_new(sc_type type, sc_addr begin, sc_addr end)
{
    return sc_memory_arc_new(s_books_ctx, type, begin, end);
}

sc_addr utils_arc_new_const(sc_addr begin, sc_addr end)
{
    return utils_arc_new(sc_type_arc_pos_const_perm, begin, end);
}

sc_addr utils_arc_new_var(sc_addr begin, sc_addr end)
{
    return utils_arc_new(sc_type_arc_pos_var_perm, begin, end);
}
