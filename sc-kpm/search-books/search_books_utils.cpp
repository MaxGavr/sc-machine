/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

extern "C"
{
#include "sc_helper.h"
#include "sc-store/sc_iterator.h"

#include "search_books.h"
#include "search_books_keynodes.h"
}

#include "search_books_utils.h"



void utils_append_to_pattern(sc_addr pattern, sc_addr element)
{
    if (SC_FALSE == sc_helper_check_arc(s_books_ctx, pattern, element, sc_type_arc_pos_const_perm))
        utils_arc_new_const(pattern, element);
}

void utils_append_to_pattern(sc_addr pattern, std::vector<sc_addr> elements)
{
    for (const sc_addr& element : elements)
        utils_append_to_pattern(pattern, element);
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


sc_addr get_agent_parameter(sc_addr parameters, sc_addr rrel_ordinal)
{
    sc_addr param;
    SC_ADDR_MAKE_EMPTY(param);

    sc_iterator5* param_it = sc_iterator5_f_a_a_a_f_new(s_books_ctx,
                                                        parameters,
                                                        sc_type_arc_pos_const_perm,
                                                        0,
                                                        sc_type_arc_pos_const_perm,
                                                        rrel_ordinal);
    if (sc_iterator5_next(param_it) == SC_TRUE)
        param = sc_iterator5_value(param_it, 2);

    sc_iterator5_free(param_it);

    return param;
}

sc_addr get_book_from_pattern(sc_addr pattern)
{
    sc_addr book;
    SC_ADDR_MAKE_EMPTY(book);

    sc_iterator3* book_it = sc_iterator3_f_a_a_new(s_books_ctx,
                                                   pattern,
                                                   sc_type_arc_pos_const_perm,
                                                   0);

    while (SC_TRUE == sc_iterator3_next(book_it))
    {
        sc_addr pattern_el = sc_iterator3_value(book_it, 2);

        if (SC_TRUE == sc_helper_check_arc(s_books_ctx, keynode_book, pattern_el, sc_type_arc_pos_var_perm))
            book = pattern_el;
    }
    sc_iterator3_free(book_it);

    return book;
}
