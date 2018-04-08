
#include "search_book_template.h"
#include "search_keynodes.h"
#include "search_utils.h"
#include "search.h"

#include <sc_helper.h>
#include <sc_memory_headers.h>

sc_result agent_search_book_template(const sc_event *event, sc_addr arg)
{
    sc_addr question;

    if (!sc_memory_get_arc_end(s_default_ctx, arg, &question))
        return SC_RESULT_ERROR_INVALID_PARAMS;

    // check question type
    if (sc_helper_check_arc(s_default_ctx, keynode_question_book_template, question, sc_type_arc_pos_const_perm) == SC_FALSE)
        return SC_RESULT_ERROR_INVALID_TYPE;

    sc_addr answer = create_answer_node();

    sc_addr tmp_link = sc_memory_link_new(s_default_ctx);

    const char *data = "test";
    sc_stream *tmp_stream = sc_stream_memory_new(data, (sc_uint)strlen(data), SC_STREAM_FLAG_READ, SC_FALSE);
    sc_memory_set_link_content(s_default_ctx, tmp_link, tmp_stream);

    appendIntoAnswer(answer, tmp_link);

    connect_answer_to_question(question, answer);
    finish_question(question);

    return SC_RESULT_OK;
}
