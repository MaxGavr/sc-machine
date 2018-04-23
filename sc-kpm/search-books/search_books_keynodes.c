/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "search_books_keynodes.h"
#include "search_books.h"

#include "../common/sc_keynodes.h"


sc_addr keynode_book;

sc_addr keynode_question_book_template;

sc_addr keynode_question;
sc_addr keynode_question_initiated;
sc_addr keynode_nrel_answer;
sc_addr keynode_question_finished;

sc_addr keynode_nrel_main_idtf;
sc_addr keynode_system_element;

const char keynode_book_str[] = "book";

const char keynode_question_book_template_str[] = "question_search_book_by_template";

const char keynode_question_initiated_str[] = "question_initiated";
const char keynode_question_str[] = "question";
const char keynode_nrel_answer_str[] = "nrel_answer";
const char keynode_question_finished_str[] = "question_finished";

const char keynode_nrel_main_idtf_str[] = "nrel_main_idtf";
const char keynode_system_element_str[] = "system_element";


sc_result search_books_keynodes_initialize()
{
    RESOLVE_KEYNODE(s_books_ctx, keynode_book);
    RESOLVE_KEYNODE(s_books_ctx, keynode_question_book_template);

    RESOLVE_KEYNODE(s_books_ctx, keynode_nrel_answer);

    RESOLVE_KEYNODE(s_books_ctx, keynode_question_initiated);
    RESOLVE_KEYNODE(s_books_ctx, keynode_question);
    RESOLVE_KEYNODE(s_books_ctx, keynode_question_finished);

    RESOLVE_KEYNODE(s_books_ctx, keynode_nrel_main_idtf);
    RESOLVE_KEYNODE(s_books_ctx, keynode_system_element);

    return SC_RESULT_OK;
}
