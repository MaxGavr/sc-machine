/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

extern "C"
{
#include "search_book_characters.h"
#include "search_books_keynodes.h"
#include "search_books.h"
#include "sc-kpm/search/search_utils.h"
#include "sc_helper.h"
#include "sc_memory_headers.h"
#include "stdio.h"
}

sc_addr question;
sc_event* event_answer_search_book_characters;

void append_to_pattern(sc_addr pattern, sc_addr element)
{
    if (SC_FALSE == sc_helper_check_arc(s_books_ctx, pattern, element, sc_type_arc_pos_const_perm))
        sc_memory_arc_new(s_books_ctx, sc_type_arc_pos_const_perm, pattern, element);
}

void create_book_search_pattern(sc_addr& pattern, sc_addr& book)
{
    pattern = sc_memory_node_new(s_books_ctx, sc_type_const);
    sc_memory_arc_new(s_books_ctx, sc_type_arc_pos_const_perm, keynode_book_search_pattern, pattern);

    book = sc_memory_node_new(s_books_ctx, sc_type_var);
    sc_addr book_arc = sc_memory_arc_new(s_books_ctx, sc_type_arc_pos_var_perm, keynode_book, book);

    append_to_pattern(pattern, book);
    append_to_pattern(pattern, keynode_book);
    append_to_pattern(pattern, book_arc);
}

void append_characters_to_pattern(sc_addr pattern, sc_addr book, sc_addr set_of_characters)
{
    // create book's set of characters and append it via nrel_characters relation
    sc_addr book_characters = sc_memory_node_new(s_books_ctx, sc_type_var);
    sc_addr characters_arc = sc_memory_arc_new(s_books_ctx, sc_type_arc_common | sc_type_var, book_characters, book);
    sc_addr nrel_arc = sc_memory_arc_new(s_books_ctx, sc_type_arc_pos_var_perm, keynode_nrel_characters, characters_arc);

    append_to_pattern(pattern, book_characters);
    append_to_pattern(pattern, characters_arc);
    append_to_pattern(pattern, nrel_arc);
    append_to_pattern(pattern, keynode_nrel_characters);

    // apend set of characters to pattern
    sc_iterator3* set_iterator = sc_iterator3_f_a_a_new(s_books_ctx,
                                                        set_of_characters,
                                                        sc_type_arc_pos_const_perm,
                                                        0);
    while (SC_TRUE == sc_iterator3_next(set_iterator))
    {
        sc_addr element = sc_iterator3_value(set_iterator, 2);

        append_to_pattern(pattern, element);

        // check if element is character
        if (SC_TRUE == sc_helper_check_arc(s_books_ctx, keynode_character, element, sc_type_arc_pos_var_perm))
        {
            sc_addr char_arc = sc_memory_arc_new(s_books_ctx, sc_type_arc_pos_var_perm, book_characters, element);
            append_to_pattern(pattern, char_arc);

            printf("Appended character to pattern\n");
        }
    }
    sc_iterator3_free(set_iterator);
}

sc_result agent_search_book_characters_show_answer(const sc_event* event, sc_addr arg)
{
    sc_addr search_by_template_answer;
    if (!sc_memory_get_arc_end(s_books_ctx, arg, &search_by_template_answer))
        return SC_RESULT_ERROR_INVALID_PARAMS;

    sc_addr answer = create_answer_node();

    sc_iterator3* answer_iterator = sc_iterator3_f_a_a_new(s_books_ctx,
                                                           search_by_template_answer,
                                                           sc_type_arc_pos_const_perm,
                                                           0);
    while (SC_TRUE == sc_iterator3_next(answer_iterator))
        appendIntoAnswer(answer, sc_iterator3_value(answer_iterator, 2));
    sc_iterator3_free(answer_iterator);

    connect_answer_to_question(question, answer);
    finish_question(question);

    if (event_answer_search_book_characters)
        sc_event_destroy(event_answer_search_book_characters);

    printf("~~~Search by characters completed~~~\n");

    return SC_RESULT_OK;
}

void initialize_book_search_template_agent(sc_addr pattern)
{
    printf("Initialize agent of searching by pattern\n");

    // create question of type question_search_book_by_template
    sc_addr search_question = sc_memory_node_new(s_books_ctx, sc_type_const);
    sc_memory_arc_new(s_books_ctx, sc_type_arc_pos_const_perm, keynode_question_book_template, search_question);
    sc_memory_arc_new(s_books_ctx, sc_type_arc_pos_const_perm, search_question, pattern);

    // subscribe for event of creating answer for question_search_book_by_template question
    event_answer_search_book_characters = sc_event_new(s_books_ctx, search_question, SC_EVENT_ADD_OUTPUT_ARC, 0, agent_search_book_characters_show_answer, 0);
    if (event_answer_search_book_characters == null_ptr)
        printf("Failed to subscribe for event\n");

    // initialize agent that searchs book by template
    sc_memory_arc_new(s_books_ctx, sc_type_arc_pos_const_perm, keynode_question_initiated, search_question);
}

sc_result agent_search_book_characters(const sc_event* event, sc_addr arg)
{
    sc_addr tmp_question;
    if (!sc_memory_get_arc_end(s_books_ctx, arg, &tmp_question))
        return SC_RESULT_ERROR_INVALID_PARAMS;

    // check question type
    if (SC_FALSE == sc_helper_check_arc(s_books_ctx, keynode_question_book_characters, tmp_question, sc_type_arc_pos_const_perm))
        return SC_RESULT_ERROR_INVALID_TYPE;

    // preserve question node beetween search_book_by_template query and answer
    question = tmp_question;

    printf("~~~Searching book by characters~~~~\n");

    sc_iterator3* char_set_iter = sc_iterator3_f_a_a_new(s_books_ctx,
                                                         question,
                                                         sc_type_arc_pos_const_perm,
                                                         0);
    // get set of characters
    if (SC_TRUE == sc_iterator3_next(char_set_iter))
    {
        sc_addr set_of_characters = sc_iterator3_value(char_set_iter, 2);

        sc_addr pattern, book;
        printf("Creating book search pattern\n");
        create_book_search_pattern(pattern, book);

        printf("Appending characters to pattern\n");
        append_characters_to_pattern(pattern, book, set_of_characters);

        initialize_book_search_template_agent(pattern);
    }
    sc_iterator3_free(char_set_iter);

    return SC_RESULT_OK;
}
