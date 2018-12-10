/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _search_books_keynodes_h_
#define _search_books_keynodes_h_

#include "sc_memory.h"

extern sc_addr keynode_nrel_main_idtf;
extern sc_addr keynode_lang_ru;

extern sc_addr keynode_book;
extern sc_addr keynode_nrel_author;
extern sc_addr keynode_nrel_original_lang;
extern sc_addr keynode_book_search_pattern;

extern sc_addr keynode_nrel_characters;
extern sc_addr keynode_character;

extern sc_addr keynode_question_book_template;
extern sc_addr keynode_question_book_characters;
extern sc_addr keynode_question_append_general_info;
extern sc_addr keynode_question_append_character;
extern sc_addr keynode_question_initiated;

extern sc_addr keynode_answer_books_not_found;
extern sc_addr keynode_answer_not_pattern;

extern sc_addr keynode_nrel_translation;
extern sc_addr keynode_resolving_link;

extern sc_addr keynode_rrel_1;
extern sc_addr keynode_rrel_2;
extern sc_addr keynode_rrel_3;
extern sc_addr keynode_rrel_4;
extern sc_addr keynode_rrel_5;

//! Initialize keynodes used by search books module
sc_result search_books_keynodes_initialize();

#endif
