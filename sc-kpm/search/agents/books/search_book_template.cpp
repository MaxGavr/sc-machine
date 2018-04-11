
#ifdef __cplusplus
extern "C"
{
#endif

#include "search_book_template.h"
#include "search_keynodes.h"
#include "search_utils.h"
#include "search.h"

#include <sc_helper.h>
#include <sc_memory_headers.h>

#ifdef __cplusplus
}
#endif

#include <sc-kpm/scp/scp_lib/scp_system_operators/sc_system_search.h>


void append_book_to_answer(sc_type_result* search_result, sc_addr answer, sc_addr book_class)
{
    sc_type_result::iterator it;

    sc_iterator3* sc_it;
    for (it = search_result->begin(); it != search_result->end(); ++it)
    {
        sc_it = sc_iterator3_f_a_f_new(s_default_ctx,
                                       book_class,
                                       sc_type_arc_pos_const_perm,
                                       it->second);
        if (sc_iterator3_next(sc_it) == SC_TRUE)
        {
            appendIntoAnswer(answer, sc_iterator3_value(sc_it, 1));
            appendIntoAnswer(answer, sc_iterator3_value(sc_it, 2));
        }

        sc_iterator3_free(sc_it);
    }
}


sc_result agent_search_book_template(const sc_event *event, sc_addr arg)
{
    sc_addr question;

    if (!sc_memory_get_arc_end(s_default_ctx, arg, &question))
        return SC_RESULT_ERROR_INVALID_PARAMS;

    // check question type
    if (sc_helper_check_arc(s_default_ctx, keynode_question_book_template, question, sc_type_arc_pos_const_perm) == SC_FALSE)
        return SC_RESULT_ERROR_INVALID_TYPE;

    sc_addr answer = create_answer_node();

    sc_iterator3* it1 = sc_iterator3_f_a_a_new(s_default_ctx,
                                               question,
                                               sc_type_arc_pos_const_perm,
                                               0);
    // get search pattern
    if (SC_TRUE == sc_iterator3_next(it1))
    {
        sc_addr pattern = sc_iterator3_value(it1, 2);

        sc_addr book;
        // TODO: check result
        sc_helper_resolve_system_identifier(s_default_ctx, "book", &book);

        // run sys_search
        sc_type_result_vector result_vector;
        sc_type_result params;
        sc_addr_vector requested_values;
        if (SC_RESULT_OK == system_sys_search(s_default_ctx, pattern, params, &result_vector))
        {
            appendIntoAnswer(answer, book);

            // extract books from found structures
            sc_type_result_vector::iterator result_it;
            for (result_it = result_vector.begin(); result_it != result_vector.end(); ++result_it)
                append_book_to_answer(*result_it, answer, book);
        }
    }
    sc_iterator3_free(it1);

    connect_answer_to_question(question, answer);
    finish_question(question);

    return SC_RESULT_OK;
}
