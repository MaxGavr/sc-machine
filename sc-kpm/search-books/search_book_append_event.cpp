/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

extern "C"
{
#include "search_book_append_event.h"
#include "search_books_keynodes.h"
#include "search_books.h"
#include "sc-kpm/search/search_utils.h"
#include "sc_helper.h"
#include "sc_memory_headers.h"
}

#include "search_books_utils.h"
#include "search_books_debug_utils.h"



sc_addr find_or_create_events_set(sc_addr pattern, sc_addr book)
{
    sc_addr events_set;
    SC_ADDR_MAKE_EMPTY(events_set);

    // try to find book's events set (plot)
    sc_iterator5* events_set_it = sc_iterator5_f_a_a_a_f_new(s_books_ctx,
                                                             book,
                                                             sc_type_arc_common | sc_type_var,
                                                             sc_type_node | sc_type_var,
                                                             sc_type_arc_pos_var_perm,
                                                             keynode_nrel_plot);
    if (sc_iterator5_next(events_set_it) == SC_TRUE)
    {
        DEBUG_MESSAGE("Books (append event): found existing events set (plot)");

        events_set = sc_iterator5_value(events_set_it, 2);
    }
    else
    {
        DEBUG_MESSAGE("Books (append event): failed to find events set (plot), creating new one");

        // if couldn't find, create one and append it to pattern
        events_set = utils_node_new_var();

        sc_addr nrel_arc = utils_arc_new(sc_type_arc_common | sc_type_var, book, events_set);
        sc_addr arc = utils_arc_new_var(keynode_nrel_plot, nrel_arc);

        utils_append_to_pattern(pattern, { events_set, nrel_arc, arc, keynode_nrel_plot });
    }
    sc_iterator5_free(events_set_it);

    return events_set;
}

sc_addr append_new_event(sc_addr pattern, sc_addr book)
{
    DEBUG_MESSAGE("Books (append event): creating new event");

    sc_addr events = find_or_create_events_set(pattern, book);

    // create new event
    sc_addr event = utils_node_new_var();
    //sc_addr char_arc = utils_arc_new_var(keynode_character, character);
    sc_addr event_accessory = utils_arc_new_var(events, event);
    utils_append_to_pattern(pattern, event);
    //utils_append_to_pattern(pattern, event_arc);
    utils_append_to_pattern(pattern, event_accessory);
    //utils_append_to_pattern(pattern, keynode_character);

    return event;
}

void add_event_type_to_pattern(sc_addr pattern, sc_addr event, sc_addr event_type)
{
    DEBUG_MESSAGE("Books (append event): adding 'event type' to pattern");

    sc_addr event_type_arc = utils_arc_new_var(event_type, event);

    utils_append_to_pattern(pattern, event_type);
    utils_append_to_pattern(pattern, event_type_arc);
}

sc_result agent_search_book_append_event(const sc_event* event, sc_addr arg)
{
    // get question
    sc_addr question;
    if (!sc_memory_get_arc_end(s_books_ctx, arg, &question))
        return SC_RESULT_ERROR_INVALID_PARAMS;

    // check question type
    if (SC_FALSE == sc_helper_check_arc(s_books_ctx, keynode_question_append_event, question, sc_type_arc_pos_const_perm))
        return SC_RESULT_ERROR_INVALID_TYPE;

    DEBUG_MESSAGE("Books (append event): initialize");

    sc_result result = SC_RESULT_OK;

    sc_iterator3* params_it = sc_iterator3_f_a_a_new(s_books_ctx,
                                                     question,
                                                     sc_type_arc_pos_const_perm,
                                                     0);
    // get set of agent parameters
    if (SC_TRUE == sc_iterator3_next(params_it))
    {
        sc_addr parameters = sc_iterator3_value(params_it, 2);

        // get pattern from parameters
        sc_addr pattern = get_agent_parameter(parameters, keynode_rrel_1);
        if (SC_ADDR_IS_NOT_EMPTY(pattern))
        {
            // get book from pattern
            sc_addr book = get_book_from_pattern(pattern);
            if (SC_ADDR_IS_NOT_EMPTY(book))
            {
                sc_addr event = append_new_event(pattern, book);

                sc_addr event_type = get_agent_parameter(parameters, keynode_rrel_2);
                if (SC_ADDR_IS_NOT_EMPTY(event_type))
                    add_event_type_to_pattern(pattern, event, event_type);
            }
            else
            {
                DEBUG_MESSAGE("Books (append event): failed to find book in search pattern");
                result = SC_RESULT_ERROR_INVALID_PARAMS;
            }
        }
        else
        {
            DEBUG_MESSAGE("Books (append event): failed to find search pattern");
            result = SC_RESULT_ERROR_INVALID_PARAMS;
        }
    }
    else
    {
        DEBUG_MESSAGE("Books (append event): failed to find parameters set");
        result = SC_RESULT_ERROR_INVALID_PARAMS;
    }
    sc_iterator3_free(params_it);

    finish_question(question);

    DEBUG_MESSAGE("Books (append event): finished");

    return result;
}
