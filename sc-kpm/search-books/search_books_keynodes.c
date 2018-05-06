/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "search_books_keynodes.h"
#include "search_books.h"

#include "../common/sc_keynodes.h"


sc_addr keynode_book;
sc_addr keynode_book_search_pattern;
sc_addr keynode_nrel_characters;
sc_addr keynode_character;

sc_addr keynode_question_book_template;
sc_addr keynode_question_book_characters;
sc_addr keynode_question_initiated;

sc_addr keynode_answer_books_not_found;
sc_addr keynode_answer_not_pattern;

sc_addr keynode_nrel_translation;
sc_addr keynode_resolving_link;

const char keynode_book_str[] = "book";
const char keynode_book_search_pattern_str[] = "book_search_pattern";
const char keynode_nrel_characters_str[] = "nrel_characters";
const char keynode_character_str[] = "lit_person";

const char keynode_question_book_template_str[] = "question_search_book_by_template";
const char keynode_question_book_characters_str[] = "question_search_book_by_characters";
const char keynode_question_initiated_str[] = "question_initiated";

const char keynode_answer_books_not_found_str[] = "answer_search_book_by_template_not_found";
const char keynode_answer_not_pattern_str[] = "answer_search_book_by_template_not_template";

const char keynode_nrel_translation_str[] = "nrel_sc_text_translation";
const char keynode_resolving_link_str[] = "resolving_link";


sc_result search_books_keynodes_initialize()
{
    RESOLVE_KEYNODE(s_books_ctx, keynode_book);
    RESOLVE_KEYNODE(s_books_ctx, keynode_book_search_pattern);
    RESOLVE_KEYNODE(s_books_ctx, keynode_nrel_characters);
    RESOLVE_KEYNODE(s_books_ctx, keynode_character);

    RESOLVE_KEYNODE(s_books_ctx, keynode_question_book_template);
    RESOLVE_KEYNODE(s_books_ctx, keynode_question_book_characters);
    RESOLVE_KEYNODE(s_books_ctx, keynode_question_initiated);

    RESOLVE_KEYNODE(s_books_ctx, keynode_answer_books_not_found);
    RESOLVE_KEYNODE(s_books_ctx, keynode_answer_not_pattern);

    RESOLVE_KEYNODE(s_books_ctx, keynode_nrel_translation);
    RESOLVE_KEYNODE(s_books_ctx, keynode_resolving_link);

    return SC_RESULT_OK;
}
