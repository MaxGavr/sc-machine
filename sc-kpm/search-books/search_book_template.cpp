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

#include "search_books_debug_utils.h"

#include "algorithm"

// for sys_search
#include <sc-kpm/scp/scp_lib/scp_system_operators/sc_system_search.h>



sc_bool get_resolving_link_translation(sc_addr resolving_link, sc_addr& translation_link)
{
    sc_iterator5* translation_iter = sc_iterator5_a_a_f_a_f_new(s_books_ctx,
                                                                sc_type_link,
                                                                sc_type_arc_common | sc_type_var,
                                                                resolving_link,
                                                                sc_type_arc_pos_var_perm,
                                                                keynode_nrel_translation);
    if (SC_TRUE == sc_iterator5_next(translation_iter))
    {
        translation_link = sc_iterator5_value(translation_iter, 0);
        sc_iterator5_free(translation_iter);

        return SC_TRUE;
    }
    sc_iterator5_free(translation_iter);

    return SC_FALSE;
}

bool is_translation_of_resolved_link(sc_addr element)
{
    bool isTranslation = false;

    sc_iterator5* translation_it = sc_iterator5_f_a_a_a_f_new(s_books_ctx,
                                                              element,
                                                              sc_type_arc_common,
                                                              sc_type_node,
                                                              sc_type_arc_access,
                                                              keynode_nrel_translation);

    while (SC_TRUE == sc_iterator5_next(translation_it))
    {
        sc_addr translation = sc_iterator5_value(translation_it, 2);

        if (SC_TRUE == sc_helper_check_arc(s_books_ctx, keynode_resolving_link, translation, sc_type_arc_access))
        {
            isTranslation = true;
            break;
        }
    }

    sc_iterator5_free(translation_it);

    return isTranslation;
}

void filter_resolved_links_from_search_patterns(sc_addr_vector& links)
{
    links.erase(std::remove_if(links.begin(), links.end(), is_translation_of_resolved_link), links.end());
}

sc_result find_links_with_content(sc_stream* content, sc_addr_vector& found_links)
{
    sc_addr* found_links_ptr = nullptr;
    sc_uint32 found_links_count = 0;
    sc_result result = sc_memory_find_links_with_content(s_books_ctx, content, &found_links_ptr, &found_links_count);

    found_links.clear();
    for (sc_uint32 i = 0; i < found_links_count; ++i)
        found_links.push_back(found_links_ptr[i]);

    return result;
}

sc_bool resolve_links(sc_addr pattern, sc_type_result& resolved_links)
{
    sc_bool links_resolved = SC_TRUE;

    sc_iterator3* pattern_iter = sc_iterator3_f_a_a_new(s_books_ctx,
                                                        pattern,
                                                        sc_type_arc_pos_const_perm,
                                                        sc_type_node | sc_type_var);
    while (SC_TRUE == sc_iterator3_next(pattern_iter))
    {
        sc_addr link = sc_iterator3_value(pattern_iter, 2);

        if (SC_FALSE == sc_helper_check_arc(s_books_ctx, keynode_resolving_link, link, sc_type_arc_pos_var_perm))
            continue;

        sc_addr resolved_link;
        if (!get_resolving_link_translation(link, resolved_link))
        {
            DEBUG_MESSAGE("Books (search by pattern): can't find translation of resolving link");
            return SC_FALSE;
        }

        sc_stream* link_content = nullptr;
        sc_memory_get_link_content(s_books_ctx, resolved_link, &link_content);

        sc_char link_content_str[256] = "";
        sc_uint32 link_content_length;
        sc_stream_read_data(link_content, link_content_str, 256, &link_content_length);

        DEBUG_MESSAGE("Books (search by pattern): resolving link \"" << link_content_str << "\"");

        sc_addr_vector found_links;
        sc_result result = find_links_with_content(link_content, found_links);

        filter_resolved_links_from_search_patterns(found_links);

        if (result != SC_RESULT_OK || found_links.size() != 1)
        {
            if (found_links.size() > 1)
                DEBUG_MESSAGE("Books (search by pattern): found >1 links with the same content");

            DEBUG_MESSAGE("Books (search by pattern): failed to resolve link");

            links_resolved = SC_FALSE;
            break;
        }

        DEBUG_MESSAGE("Books (search by pattern): link \"" << link_content_str << "\" successfully resolved");

        resolved_links[link] = found_links.front();
    }
    sc_iterator3_free(pattern_iter);

    return links_resolved;
}

void append_book_to_answer(sc_type_result* search_result, sc_addr answer)
{
    sc_type_result::iterator result_iter;

    for (result_iter = search_result->begin(); result_iter != search_result->end(); ++result_iter)
    {
        sc_iterator3* book_iter = sc_iterator3_f_a_f_new(s_books_ctx,
                                                         keynode_book,
                                                         sc_type_arc_pos_const_perm,
                                                         result_iter->second);
        if (sc_iterator3_next(book_iter) == SC_TRUE)
        {
            DEBUG_MESSAGE("Books (search by pattern): found book:");
            printIdtf(s_books_ctx, sc_iterator3_value(book_iter, 2));
            DEBUG_MESSAGE("");

            appendIntoAnswer(answer, sc_iterator3_value(book_iter, 1));
            appendIntoAnswer(answer, sc_iterator3_value(book_iter, 2));
        }

        sc_iterator3_free(book_iter);
    }
}

void set_answer_type(sc_addr answer, sc_addr answer_type)
{
    appendIntoAnswer(answer, answer_type);
    appendIntoAnswer(answer, keynode_nrel_translation);

    sc_iterator5* translation_iter = sc_iterator5_a_a_f_a_f_new(s_books_ctx,
                                                                0,
                                                                sc_type_arc_common | sc_type_const,
                                                                answer_type,
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

    DEBUG_MESSAGE("Books (search by pattern): initialize");

    sc_addr answer = create_answer_node();

    sc_iterator3* pattern_iter = sc_iterator3_f_a_a_new(s_books_ctx,
                                                        question,
                                                        sc_type_arc_pos_const_perm,
                                                        0);
    // get search pattern
    if (SC_TRUE == sc_iterator3_next(pattern_iter))
    {
        sc_addr pattern = sc_iterator3_value(pattern_iter, 2);

        // check if pattern is a book search pattern
        if (SC_TRUE == sc_helper_check_arc(s_books_ctx, keynode_book_search_pattern, pattern, sc_type_arc_pos_const_perm))
        {
            sc_type_result resolved_links;

            if (SC_TRUE == resolve_links(pattern, resolved_links))
            {
                // run sys_search
                sc_type_result_vector result_vector;
                sc_result result = system_sys_search_only_full(s_books_ctx, pattern, resolved_links, &result_vector);
                if (SC_RESULT_OK == result && !result_vector.empty())
                {
                    appendIntoAnswer(answer, keynode_book);

                    // extract books from found structures
                    sc_type_result_vector::iterator result_it;
                    for (result_it = result_vector.begin(); result_it != result_vector.end(); ++result_it)
                        append_book_to_answer(*result_it, answer);
                }
                else
                    set_answer_type(answer, keynode_answer_books_not_found);
            }
            else
                set_answer_type(answer, keynode_answer_books_not_found);
        }
        else
            set_answer_type(answer, keynode_answer_not_pattern);
    }
    else
        set_answer_type(answer, keynode_answer_not_pattern);

    sc_iterator3_free(pattern_iter);

    connect_answer_to_question(question, answer);
    finish_question(question);

    DEBUG_MESSAGE("Books (search by pattern): finished");

    return SC_RESULT_OK;
}
