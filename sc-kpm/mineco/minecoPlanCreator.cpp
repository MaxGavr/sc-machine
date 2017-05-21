/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "sc-memory/cpp/sc_memory.hpp"
#include <iostream>

#include "minecoPlanCreator.hpp"

namespace mineco
{
ScAddr AMinEcoPlanCreator::msAgentKeynode;
ScAddr AMinEcoPlanCreator::rrel_1;
ScAddr AMinEcoPlanCreator::rrel_2;
ScAddr AMinEcoPlanCreator::concept_organization;
ScAddr AMinEcoPlanCreator::ap_receiving_and_registering_book;
ScAddr AMinEcoPlanCreator::concept_store_opening;
ScAddr AMinEcoPlanCreator::question_plan_creation;
ScAddr AMinEcoPlanCreator::question_initiated;
ScAddr AMinEcoPlanCreator::nrel_answer;
ScAddr AMinEcoPlanCreator::nrel_goto;
ScAddr AMinEcoPlanCreator::nrel_decomposition_of_action;
ScAddr AMinEcoPlanCreator::nrel_address;
ScAddr AMinEcoPlanCreator::nrel_legal_address;
ScAddr AMinEcoPlanCreator::nrel_administrative_subordination;
ScAddr AMinEcoPlanCreator::nrel_registration;
ScAddr AMinEcoPlanCreator::inspection_MNS;

SC_AGENT_IMPLEMENTATION(AMinEcoPlanCreator)
{
    if (!edgeAddr.IsValid())
        return SC_RESULT_ERROR;

    ScAddr action = ms_context->GetArcEnd(edgeAddr);

    if (!ms_context->HelperCheckArc(question_plan_creation, action, ScType::EdgeAccessConstPosPerm))
        return SC_RESULT_ERROR_INVALID_PARAMS;

    ScAddr program, param;
    ScIterator5Ptr iter_param = ms_context->Iterator5(action, ScType::EdgeAccessConstPosPerm, ScType::Node, ScType::EdgeAccessConstPosPerm, rrel_1);
    if (iter_param->Next())
        program = iter_param->Get(2);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    iter_param = ms_context->Iterator5(action, ScType::EdgeAccessConstPosPerm, ScType::Node, ScType::EdgeAccessConstPosPerm, rrel_2);
    if (iter_param->Next())
        param = iter_param->Get(2);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    ScTemplate program_templ;
    ms_context->HelperBuildTemplate(program_templ, program);
    ScTemplateGenParams gen_params;

    ScTemplateGenResult res;
    ms_context->HelperGenTemplate(program_templ, res);

    ScAddr answer = ms_context->CreateNode(ScType::Const);

    for (int i = 0; i < res.GetSize(); i++)
    {
        ms_context->CreateArc(ScType::EdgeAccessConstPosPerm, answer, res[i]);
    }

    // search of actions
    ScAddr store_opening, book_registering;
    for (int i = 0; i < res.GetSize(); i++)
    {
        if (ms_context->GetElementType(res[i]).IsNode())
        {

            if (ms_context->HelperCheckArc(concept_store_opening, res[i], ScType::EdgeAccessConstPosPerm))
            {
                store_opening = res[i];
            }
            if (ms_context->HelperCheckArc(ap_receiving_and_registering_book, res[i], ScType::EdgeAccessConstPosPerm))
            {
                book_registering = res[i];
            }
        }
    }

    if (!store_opening.IsValid() || !book_registering.IsValid())
        return SC_RESULT_ERROR_INVALID_PARAMS;

    //Removing unnecessary actions
    ScAddr decomp, curr_action, next_action;

    ScIterator5Ptr iter_action = ms_context->Iterator5(ScType::Node, ScType::EdgeDCommonConst, store_opening, ScType::EdgeAccessConstPosPerm, nrel_decomposition_of_action);
    if (iter_action->Next())
        decomp = iter_action->Get(0);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    iter_action = ms_context->Iterator5(decomp, ScType::EdgeAccessConstPosPerm, ScType::Node, ScType::EdgeAccessConstPosPerm, rrel_1);
    if (iter_action->Next())
        curr_action = iter_action->Get(2);
    else
        return SC_RESULT_ERROR_INVALID_PARAMS;

    while (true)
    {
        if (ms_context->HelperCheckArc(ap_receiving_and_registering_book, curr_action, ScType::EdgeAccessConstPosPerm))
        {
            ScTemplate mytempl1;
            mytempl1.TripleWithRelation(
                param,
                ScType::EdgeDCommonVar,
                ScType::Link >> "_addr1",
                ScType::EdgeAccessVarPosPerm,
                nrel_legal_address
            );
            mytempl1.TripleWithRelation(
                ScType::NodeVar >> "_district",
                ScType::EdgeDCommonVar,
                "_addr1",
                ScType::EdgeAccessVarPosPerm,
                nrel_administrative_subordination
            );

            ScTemplateSearchResult result;
            ms_context->HelperSearchTemplate(mytempl1, result);
            ScAddr district;
            if (result.Size() > 0)
                district = result[0]["_district"];
            else
                break;

            ScIterator5Ptr iter1 = ms_context->Iterator5(district, ScType::EdgeDCommonConst, ScType::Const, ScType::EdgeAccessConstPosPerm, nrel_administrative_subordination);
            while (iter1->Next())
            {
                ScAddr addr2 = iter1->Get(2);
                ScIterator5Ptr iter2 = ms_context->Iterator5(ScType::Node, ScType::EdgeDCommonConst, addr2, ScType::EdgeAccessConstPosPerm, nrel_address);
                while (iter2->Next())
                {
                    ScAddr imns = iter2->Get(0);;
                    if (ms_context->HelperCheckArc(inspection_MNS, imns, ScType::EdgeAccessConstPosPerm))
                    {
                        ScAddr arc1 = ms_context->CreateArc(ScType::EdgeDCommonConst, curr_action, imns);
                        ScAddr arc2 = ms_context->CreateArc(ScType::EdgeAccessConstPosPerm, nrel_registration, arc1);
                        ms_context->CreateArc(ScType::EdgeAccessConstPosPerm, answer, arc1);
                        ms_context->CreateArc(ScType::EdgeAccessConstPosPerm, answer, arc2);
                        ms_context->CreateArc(ScType::EdgeAccessConstPosPerm, answer, imns);
                        ms_context->CreateArc(ScType::EdgeAccessConstPosPerm, answer, nrel_registration);
                    }
                }
            }
            break;
        }
        iter_action = ms_context->Iterator5(curr_action, ScType::EdgeDCommonConst, ScType::Node, ScType::EdgeAccessConstPosPerm, nrel_goto);
        if (iter_action->Next())
        {
            next_action = iter_action->Get(2);
            ms_context->EraseElement(curr_action);
            curr_action = next_action;
        }
        else
        {
            ms_context->EraseElement(curr_action);
            break;
        }
    }

    //finish action
    ScAddr myarc = ms_context->CreateArc(ScType::EdgeDCommonConst, action, answer);
    ms_context->CreateArc(ScType::EdgeAccessConstPosPerm, nrel_answer, myarc);

    return SC_RESULT_OK;
}

}
