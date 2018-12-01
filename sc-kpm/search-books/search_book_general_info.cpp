/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

extern "C"
{
#include "search_book_general_info.h"
#include "search_books_keynodes.h"
#include "search_books.h"
#include "sc-kpm/search/search_utils.h"
#include "sc_helper.h"
#include "sc_memory_headers.h"
}

#include "search_books_utils.h"
#include "search_books_debug_utils.h"



void add_author_name_to_pattern(sc_addr pattern, sc_addr book, sc_addr author_name_link)
{
    DEBUG_MESSAGE("Books agent (general info): adding 'author name' to pattern");

    sc_addr resolving_link = utils_node_new_var();
    utils_append_to_pattern(pattern, resolving_link);

    utils_arc_new_var(keynode_resolving_link, resolving_link);

    sc_addr lang_arc = utils_arc_new_var(keynode_lang_ru, resolving_link);
    utils_append_to_pattern(pattern, lang_arc);
    utils_append_to_pattern(pattern, keynode_lang_ru);

    sc_addr translation_arc = utils_arc_new(sc_type_arc_common | sc_type_var, author_name_link, resolving_link);
    utils_arc_new_var(keynode_nrel_translation, translation_arc);


    sc_addr author = utils_node_new_var();
    utils_append_to_pattern(pattern, author);

    sc_addr author_name_arc = utils_arc_new(sc_type_arc_common | sc_type_var, author, resolving_link);
    utils_append_to_pattern(pattern, author_name_arc);

    sc_addr author_name_rrel_arc = utils_arc_new_var(keynode_nrel_main_idtf, author_name_arc);
    utils_append_to_pattern(pattern, author_name_rrel_arc);
    utils_append_to_pattern(pattern, keynode_nrel_main_idtf);

    sc_addr author_arc = utils_arc_new(sc_type_arc_common | sc_type_var, book, author);
    utils_append_to_pattern(pattern, author_arc);

    sc_addr author_rrel_arc = utils_arc_new_var(keynode_nrel_author, author_arc);
    utils_append_to_pattern(pattern, author_rrel_arc);
    utils_append_to_pattern(pattern, keynode_nrel_author);
}

void add_genre_to_pattern(sc_addr pattern, sc_addr book, sc_addr genre)
{
    DEBUG_MESSAGE("Books agent (general info): adding 'genre' to pattern");

    sc_addr genre_arc = utils_arc_new_var(genre, book);

    utils_append_to_pattern(pattern, genre);
    utils_append_to_pattern(pattern, genre_arc);
}

void add_language_to_pattern(sc_addr pattern, sc_addr book, sc_addr language)
{
    DEBUG_MESSAGE("Books agent (general info): adding 'language' to pattern");

    sc_addr lang_arc = utils_arc_new(sc_type_arc_common | sc_type_var, language, book);

    utils_append_to_pattern(pattern, language);
    utils_append_to_pattern(pattern, lang_arc);

    sc_addr lang_nrel_arc = utils_arc_new_var(keynode_nrel_original_lang, lang_arc);
    utils_append_to_pattern(pattern, lang_nrel_arc);
    utils_append_to_pattern(pattern, keynode_nrel_original_lang);
}

sc_result agent_search_book_general_info(const sc_event* event, sc_addr arg)
{
    // get question
    sc_addr question;
    if (!sc_memory_get_arc_end(s_books_ctx, arg, &question))
        return SC_RESULT_ERROR_INVALID_PARAMS;

    // check question type
    if (SC_FALSE == sc_helper_check_arc(s_books_ctx, keynode_question_append_general_info, question, sc_type_arc_pos_const_perm))
        return SC_RESULT_ERROR_INVALID_TYPE;

    DEBUG_MESSAGE("Books agent (general info): initialize");

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
                sc_addr author_name = get_agent_parameter(parameters, keynode_rrel_2);
                if (SC_ADDR_IS_NOT_EMPTY(author_name))
                    add_author_name_to_pattern(pattern, book, author_name);

                sc_addr genre = get_agent_parameter(parameters, keynode_rrel_3);
                if (SC_ADDR_IS_NOT_EMPTY(genre))
                    add_genre_to_pattern(pattern, book, genre);

                sc_addr language = get_agent_parameter(parameters, keynode_rrel_4);
                if (SC_ADDR_IS_NOT_EMPTY(language))
                    add_language_to_pattern(pattern, book, language);
            }
            else
            {
                DEBUG_MESSAGE("Books agent (general info): failed to find book in search pattern");
                result = SC_RESULT_ERROR_INVALID_PARAMS;
            }
        }

        else
        {
            DEBUG_MESSAGE("Books agent (general info): failed to find search pattern");
            result = SC_RESULT_ERROR_INVALID_PARAMS;
        }
    }
    else
    {
        DEBUG_MESSAGE("Books agent (general info): failed to find parameters set");
        result = SC_RESULT_ERROR_INVALID_PARAMS;
    }
    sc_iterator3_free(params_it);

    finish_question(question);

    DEBUG_MESSAGE("Books agent (general info): finished");

    return result;
}
