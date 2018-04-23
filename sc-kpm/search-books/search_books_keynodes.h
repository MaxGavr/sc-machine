/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _search_books_keynodes_h_
#define _search_books_keynodes_h_

#include "sc_memory.h"

extern sc_addr keynode_book;

extern sc_addr keynode_question_book_template;

extern sc_addr keynode_nrel_answer;
extern sc_addr keynode_question_finished;
extern sc_addr keynode_question_initiated;
extern sc_addr keynode_question;
extern sc_addr keynode_nrel_main_idtf;
extern sc_addr keynode_system_element;

//! Initialie keynodes that used by search books module
sc_result search_books_keynodes_initialize();

#endif
