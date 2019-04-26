/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "search_books_keynodes.h"
#include "search_books.h"

#include "../common/sc_keynodes.h"


sc_addr keynode_nrel_main_idtf;
sc_addr keynode_lang_ru;

sc_addr keynode_book;
sc_addr keynode_nrel_author;
sc_addr keynode_nrel_original_lang;
sc_addr keynode_book_search_pattern;

sc_addr keynode_nrel_characters;
sc_addr keynode_character;

sc_addr keynode_nrel_plot;

sc_addr keynode_question_initiated;

sc_addr keynode_question_book_template;
sc_addr keynode_question_book_characters;
sc_addr keynode_question_append_general_info;
sc_addr keynode_question_append_character;
sc_addr keynode_question_append_event;

sc_addr keynode_answer_books_not_found;
sc_addr keynode_answer_not_pattern;

sc_addr keynode_nrel_translation;
sc_addr keynode_resolving_link;

sc_addr keynode_rrel_1;
sc_addr keynode_rrel_2;
sc_addr keynode_rrel_3;
sc_addr keynode_rrel_4;
sc_addr keynode_rrel_5;

const char keynode_nrel_main_idtf_str[] = "nrel_main_idtf";
const char keynode_lang_ru_str[] = "lang_ru";

const char keynode_book_str[] = "book";
const char keynode_nrel_author_str[] = "nrel_author";
const char keynode_nrel_original_lang_str[] = "nrel_original_language";
const char keynode_book_search_pattern_str[] = "book_search_pattern";

const char keynode_nrel_characters_str[] = "nrel_characters";
const char keynode_character_str[] = "lit_person";

const char keynode_nrel_plot_str[] = "nrel_plot";

const char keynode_question_initiated_str[] = "question_initiated";

const char keynode_question_book_template_str[] = "question_search_book_by_template";
const char keynode_question_book_characters_str[] = "question_search_book_by_characters";
const char keynode_question_append_general_info_str[] = "question_append_general_info_to_pattern";
const char keynode_question_append_character_str[] = "question_append_character_to_pattern";
const char keynode_question_append_event_str[] = "question_append_event_to_pattern";

const char keynode_answer_books_not_found_str[] = "answer_search_book_by_template_not_found";
const char keynode_answer_not_pattern_str[] = "answer_search_book_by_template_not_template";

const char keynode_nrel_translation_str[] = "nrel_sc_text_translation";
const char keynode_resolving_link_str[] = "resolving_link";

const char keynode_rrel_1_str[] = "rrel_1";
const char keynode_rrel_2_str[] = "rrel_2";
const char keynode_rrel_3_str[] = "rrel_3";
const char keynode_rrel_4_str[] = "rrel_4";
const char keynode_rrel_5_str[] = "rrel_5";

sc_result search_books_keynodes_initialize()
{
    RESOLVE_KEYNODE(s_books_ctx, keynode_nrel_main_idtf);
    RESOLVE_KEYNODE(s_books_ctx, keynode_lang_ru);

    RESOLVE_KEYNODE(s_books_ctx, keynode_book);
    RESOLVE_KEYNODE(s_books_ctx, keynode_nrel_author);
    RESOLVE_KEYNODE(s_books_ctx, keynode_nrel_original_lang);
    RESOLVE_KEYNODE(s_books_ctx, keynode_book_search_pattern);

    RESOLVE_KEYNODE(s_books_ctx, keynode_nrel_characters);
    RESOLVE_KEYNODE(s_books_ctx, keynode_character);

    RESOLVE_KEYNODE(s_books_ctx, keynode_nrel_plot);

    RESOLVE_KEYNODE(s_books_ctx, keynode_question_initiated);

    RESOLVE_KEYNODE(s_books_ctx, keynode_question_book_template);
    RESOLVE_KEYNODE(s_books_ctx, keynode_question_book_characters);
    RESOLVE_KEYNODE(s_books_ctx, keynode_question_append_general_info);
    RESOLVE_KEYNODE(s_books_ctx, keynode_question_append_character);
    RESOLVE_KEYNODE(s_books_ctx, keynode_question_append_event);

    RESOLVE_KEYNODE(s_books_ctx, keynode_answer_books_not_found);
    RESOLVE_KEYNODE(s_books_ctx, keynode_answer_not_pattern);

    RESOLVE_KEYNODE(s_books_ctx, keynode_nrel_translation);
    RESOLVE_KEYNODE(s_books_ctx, keynode_resolving_link);

    RESOLVE_KEYNODE(s_books_ctx, keynode_rrel_1);
    RESOLVE_KEYNODE(s_books_ctx, keynode_rrel_2);
    RESOLVE_KEYNODE(s_books_ctx, keynode_rrel_3);
    RESOLVE_KEYNODE(s_books_ctx, keynode_rrel_4);
    RESOLVE_KEYNODE(s_books_ctx, keynode_rrel_5);

    return SC_RESULT_OK;
}
