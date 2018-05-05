/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _search_books_keynodes_h_
#define _search_books_keynodes_h_

#include "sc_memory.h"

extern sc_addr keynode_book;
extern sc_addr keynode_book_search_pattern;

extern sc_addr keynode_question_book_template;
extern sc_addr keynode_question_initiated;

extern sc_addr keynode_answer_books_not_found;
extern sc_addr keynode_answer_not_pattern;

extern sc_addr keynode_nrel_translation;
extern sc_addr keynode_resolving_link;

//! Initialize keynodes used by search books module
sc_result search_books_keynodes_initialize();

#endif
