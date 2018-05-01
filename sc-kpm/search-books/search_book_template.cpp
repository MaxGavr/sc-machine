/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

extern "C"
{
#include "search_book_template.h"
#include "search_books_keynodes.h"
#include "search_books.h"
#include "sc-kpm/search/search_utils.h"
#include "sc_helper.h"
}

// for sys_search
#include <sc-kpm/scp/scp_lib/scp_system_operators/sc_system_search.h>


sc_bool resolve_links(sc_addr pattern, sc_type_result& resolved_links)
{
    sc_bool links_resolved = SC_TRUE;

    sc_iterator3* links_iterator = sc_iterator3_f_a_a_new(s_books_ctx,
                                                          pattern,
                                                          sc_type_arc_pos_const_perm,
                                                          sc_type_link);
    while (sc_iterator3_next(links_iterator) == SC_TRUE)
    {
        sc_addr link = sc_iterator3_value(links_iterator, 2);

        sc_stream* link_content = NULL;
        sc_memory_get_link_content(s_books_ctx, link, &link_content);

        sc_char link_content_str[256] = "";
        sc_uint32 link_content_length;
        sc_stream_read_data(link_content, link_content_str, 256, &link_content_length);

        printf("Resolving link \"%s\"\n", link_content_str);

        sc_addr* found_links = NULL;
        sc_uint32 found_links_count = 0;
        sc_result result = sc_memory_find_links_with_content(s_books_ctx, link_content, &found_links, &found_links_count);
        if (result != SC_RESULT_OK || found_links_count != 2)
        {
            if (found_links_count > 2)
                printf("Found >1 links with the same content\n");

            printf("Failed to resolve link\n");

            links_resolved = SC_FALSE;
            break;
        }

        sc_addr found_link = SC_ADDR_IS_EQUAL(found_links[0], link) ? found_links[1] : found_links[0];
        resolved_links[link] = found_link;
    }
    sc_iterator3_free(links_iterator);

    return links_resolved;
}

void append_book_to_answer(sc_type_result* search_result, sc_addr answer)
{
    sc_type_result::iterator result_iter;

    sc_iterator3* book_iter;
    for (result_iter = search_result->begin(); result_iter != search_result->end(); ++result_iter)
    {
        book_iter = sc_iterator3_f_a_f_new(s_books_ctx,
                                           keynode_book,
                                           sc_type_arc_pos_const_perm,
                                           result_iter->second);
        if (sc_iterator3_next(book_iter) == SC_TRUE)
        {
            printf("Found book \"");
            printIdtf(s_books_ctx, sc_iterator3_value(book_iter, 2));
            printf("\n");

            appendIntoAnswer(answer, sc_iterator3_value(book_iter, 1));
            appendIntoAnswer(answer, sc_iterator3_value(book_iter, 2));
        }

        sc_iterator3_free(book_iter);
    }
}

void no_books_found_answer(sc_addr answer)
{
    appendIntoAnswer(answer, keynode_answer_books_not_found);
    appendIntoAnswer(answer, keynode_nrel_translation);

    sc_iterator5* translation_iter = sc_iterator5_a_a_f_a_f_new(s_books_ctx,
                                                                0,
                                                                sc_type_arc_common | sc_type_const,
                                                                keynode_answer_books_not_found,
                                                                sc_type_arc_pos_const_perm,
                                                                keynode_nrel_translation);
    while (SC_TRUE == sc_iterator5_next(translation_iter))
    {
        appendIntoAnswer(answer, sc_iterator5_value(translation_iter, 0));
        appendIntoAnswer(answer, sc_iterator5_value(translation_iter, 1));
        appendIntoAnswer(answer, sc_iterator5_value(translation_iter, 3));
    }
}

sc_result agent_search_book_template(const sc_event* event, sc_addr arg)
{
    sc_addr question;

    if (!sc_memory_get_arc_end(s_books_ctx, arg, &question))
        return SC_RESULT_ERROR_INVALID_PARAMS;

    // check question type
    if (SC_FALSE == sc_helper_check_arc(s_books_ctx, keynode_question_book_template, question, sc_type_arc_pos_const_perm))
        return SC_RESULT_ERROR_INVALID_TYPE;

    printf("~~~Searching book by template~~~~\n");

    sc_addr answer = create_answer_node();

    sc_iterator3* pattern_iter = sc_iterator3_f_a_a_new(s_books_ctx,
                                                        question,
                                                        sc_type_arc_pos_const_perm,
                                                        0);
    // get search pattern
    if (SC_TRUE == sc_iterator3_next(pattern_iter))
    {
        sc_addr pattern = sc_iterator3_value(pattern_iter, 2);

        sc_type_result resolved_links;

        if (SC_TRUE == resolve_links(pattern, resolved_links))
        {
            // run sys_search
            sc_type_result_vector result_vector;
            if (SC_RESULT_OK == system_sys_search(s_books_ctx, pattern, resolved_links, &result_vector))
            {
                appendIntoAnswer(answer, keynode_book);

                // extract books from found structures
                sc_type_result_vector::iterator result_it;
                for (result_it = result_vector.begin(); result_it != result_vector.end(); ++result_it)
                    append_book_to_answer(*result_it, answer);
            }
            else
                no_books_found_answer(answer);
        }
        else
            no_books_found_answer(answer);
    }
    sc_iterator3_free(pattern_iter);

    connect_answer_to_question(question, answer);
    finish_question(question);

    printf("~~~Search completed~~~\n");

    return SC_RESULT_OK;
}
