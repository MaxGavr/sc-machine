/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

extern "C"
{
#include "search_book_append_character.h"
#include "search_books_keynodes.h"
#include "search_books.h"
#include "sc-kpm/search/search_utils.h"
#include "sc_helper.h"
#include "sc_memory_headers.h"
}

#include "search_books_utils.h"
#include "search_books_debug_utils.h"



sc_addr find_or_create_characters_set(sc_addr pattern, sc_addr book)
{
    sc_addr characters_set;
    SC_ADDR_MAKE_EMPTY(characters_set);

    // try to find book's characters set
    sc_iterator5* char_set_it = sc_iterator5_a_a_f_a_f_new(s_books_ctx,
                                                           sc_type_node | sc_type_var,
                                                           sc_type_arc_common | sc_type_var,
                                                           book,
                                                           sc_type_arc_pos_var_perm,
                                                           keynode_nrel_characters);
    if (sc_iterator5_next(char_set_it) == SC_TRUE)
    {
        DEBUG_MESSAGE("Books (append character): found existing characters set");

        characters_set = sc_iterator5_value(char_set_it, 0);
    }
    else
    {
        DEBUG_MESSAGE("Books (append character): failed to find characters set, creating new one");

        // if couldn't find, create one and append it to pattern
        characters_set = utils_node_new_var();

        sc_addr nrel_arc = utils_arc_new(sc_type_arc_common | sc_type_var, characters_set, book);
        sc_addr arc = utils_arc_new_var(keynode_nrel_characters, nrel_arc);

        utils_append_to_pattern(pattern, { characters_set, nrel_arc, arc, keynode_nrel_characters });
    }
    sc_iterator5_free(char_set_it);

    return characters_set;
}

sc_addr append_new_character(sc_addr pattern, sc_addr book)
{
    DEBUG_MESSAGE("Books (append character): creating new character");

    sc_addr characters = find_or_create_characters_set(pattern, book);

    // create new character
    sc_addr character = utils_node_new_var();
    sc_addr char_arc = utils_arc_new_var(keynode_character, character);
    sc_addr char_accessory = utils_arc_new_var(characters, character);
    utils_append_to_pattern(pattern, character);
    utils_append_to_pattern(pattern, char_arc);
    utils_append_to_pattern(pattern, char_accessory);
    utils_append_to_pattern(pattern, keynode_character);

    return character;
}

void add_character_name_to_pattern(sc_addr pattern, sc_addr character, sc_addr character_name_link)
{
    DEBUG_MESSAGE("Books (append character): adding 'character name' to pattern");

    sc_addr resolving_link = utils_node_new_var();
    utils_append_to_pattern(pattern, resolving_link);

    utils_arc_new_var(keynode_resolving_link, resolving_link);

    sc_addr lang_arc = utils_arc_new_var(keynode_lang_ru, resolving_link);
    utils_append_to_pattern(pattern, lang_arc);
    utils_append_to_pattern(pattern, keynode_lang_ru);

    sc_addr translation_arc = utils_arc_new(sc_type_arc_common | sc_type_var, character_name_link, resolving_link);
    utils_arc_new_var(keynode_nrel_translation, translation_arc);

    sc_addr char_name_arc = utils_arc_new(sc_type_arc_common | sc_type_var, character, resolving_link);
    utils_append_to_pattern(pattern, char_name_arc);

    sc_addr char_name_rrel_arc = utils_arc_new_var(keynode_nrel_main_idtf, char_name_arc);
    utils_append_to_pattern(pattern, char_name_rrel_arc);
    utils_append_to_pattern(pattern, keynode_nrel_main_idtf);
}

void add_character_gender_to_pattern(sc_addr pattern, sc_addr character, sc_addr gender)
{
    DEBUG_MESSAGE("Books (append character): adding 'character gender' to pattern");

    sc_addr gender_arc = utils_arc_new_var(gender, character);

    utils_append_to_pattern(pattern, gender);
    utils_append_to_pattern(pattern, gender_arc);
}

void add_character_type_to_pattern(sc_addr pattern, sc_addr book, sc_addr character, sc_addr character_type)
{
    DEBUG_MESSAGE("Books (append character): adding 'character type' to pattern");

    sc_addr characters = find_or_create_characters_set(pattern, book);

    sc_iterator3* arc_it = sc_iterator3_f_a_f_new(s_books_ctx,
                                                 characters,
                                                 sc_type_arc_pos_var_perm,
                                                 character);
    if (sc_iterator3_next(arc_it) == SC_TRUE)
    {
        sc_addr type_arc = sc_iterator3_value(arc_it, 1);

        utils_append_to_pattern(pattern, utils_arc_new_var(character_type, type_arc));
        utils_append_to_pattern(pattern, character_type);
    }
    sc_iterator3_free(arc_it);
}

sc_result agent_search_book_append_character(const sc_event* event, sc_addr arg)
{
    // get question
    sc_addr question;
    if (!sc_memory_get_arc_end(s_books_ctx, arg, &question))
        return SC_RESULT_ERROR_INVALID_PARAMS;

    // check question type
    if (SC_FALSE == sc_helper_check_arc(s_books_ctx, keynode_question_append_character, question, sc_type_arc_pos_const_perm))
        return SC_RESULT_ERROR_INVALID_TYPE;

    DEBUG_MESSAGE("Books (append character): initialize");

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
                sc_addr character = append_new_character(pattern, book);

                sc_addr character_name = get_agent_parameter(parameters, keynode_rrel_2);
                if (SC_ADDR_IS_NOT_EMPTY(character_name))
                    add_character_name_to_pattern(pattern, character, character_name);

                sc_addr gender = get_agent_parameter(parameters, keynode_rrel_3);
                if (SC_ADDR_IS_NOT_EMPTY(gender))
                    add_character_gender_to_pattern(pattern, character, gender);

                sc_addr type = get_agent_parameter(parameters, keynode_rrel_4);
                if (SC_ADDR_IS_NOT_EMPTY(type))
                    add_character_type_to_pattern(pattern, book, character, type);
            }
            else
            {
                DEBUG_MESSAGE("Books (append character): failed to find book in search pattern");
                result = SC_RESULT_ERROR_INVALID_PARAMS;
            }
        }
        else
        {
            DEBUG_MESSAGE("Books (append character): failed to find search pattern");
            result = SC_RESULT_ERROR_INVALID_PARAMS;
        }
    }
    else
    {
        DEBUG_MESSAGE("Books (append character): failed to find parameters set");
        result = SC_RESULT_ERROR_INVALID_PARAMS;
    }
    sc_iterator3_free(params_it);

    finish_question(question);

    DEBUG_MESSAGE("Books (append character): finished");

    return result;
}
