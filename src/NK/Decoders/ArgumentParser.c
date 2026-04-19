/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */

#include "NK/Decoders/ArgumentParser.h"
#include "NK/Core/Panic.h"
#include "NK/Memory.h"
#include "NK/Support.h"
#include "NK/Enums.h"

NK_ArgumentParser*
NK_ArgumentParserNew()
{
    NK_ArgumentParser* arg_parser =
        (NK_ArgumentParser*)(
            NK_AllocatorGet(sizeof(NK_ArgumentParser))
        );
    return arg_parser;
}

void
NK_ArgumentParserFree(
    NK_ArgumentParser* arg_parser
)
{
    NK_AllocatorFree(arg_parser);
}

void
NK_ArgumentParserConstruct(
    NK_ArgumentParser* arg_parser,
    const NK_Size args_counter,
    const NK_C8** args_source,
    void* userdata
)
{
    /**
     * NOTE: We want to skip the first index, which is the file we are running
     * anyways.
     */
    arg_parser->args_index = 0;
    arg_parser->args_length = args_counter;
    arg_parser->args_source = args_source;
    arg_parser->userdata = userdata;
    arg_parser->state = NK_ENUMS_ARGUMENT_PARSER_STATE_RUNNING;
    arg_parser->error_register = NK_ENUMS_ARGUMENT_PARSER_ERROR_CODE_NONE;

    /**
     * Construct the logics.
     */
    NK_MapConstruct(
        &arg_parser->actions,
        512,
        sizeof(NK_ArgumentParserActionFunction)
    );
    NK_MapConstruct(
        &arg_parser->links,
        512,
        sizeof(NK_SubmergedString)
    );
}

static
NK_Result
P_NK_ArgumentParserDestructCleanLinks(
    NK_Map* map,
    const NK_C8* key,
    const NK_Size index,
    NK_SubmergedString* data,
    void* userdata
)
{
    /** We just need this P_ function to erase and drop those strings. */
    NK_SubmergedStringDestruct(data);
    return true;
}

void
NK_ArgumentParserDestruct(
    NK_ArgumentParser* arg_parser
)
{
    /**
     * Actions don't need any special cleanup, it is just pointers anyway, but
     * `->links` need since it contains `NK_SubmergedString`(s), which need to 
     * be dropped (so they can be cleaned).
     */
    NK_MapDestruct(&arg_parser->actions);
    NK_MapIterate(
        &arg_parser->links,
        (NK_MapIteratorFunction)(P_NK_ArgumentParserDestructCleanLinks),
        NULL
    );
    NK_MapDestruct(&arg_parser->links);
}

void
NK_ArgumentParserAddAction(
    NK_ArgumentParser* arg_parser,
    const NK_C8* key,
    const NK_ArgumentParserActionFunction function
)
{
    /** We copy this to the map, we don't mind replaces. */
    NK_MapInsertOrAssign(&arg_parser->actions, key, (const void*)(&function));
}

void
NK_ArgumentParserAddLink(
    NK_ArgumentParser* arg_parser,
    const NK_C8* key,
    const NK_C8* action
)
{
    NK_SubmergedString vessel;
    NK_SubmergedStringConstruct(&vessel, action);
    NK_MapInsert(
        &arg_parser->links,
        key,
        (void*)(&vessel)
    );
}

const NK_C8*
NK_ArgumentParserPull(
    NK_ArgumentParser* arg_parser
)
{
    NK_C8* param = (
        (arg_parser->args_index >= arg_parser->args_length)
        ? NULL
        : arg_parser->args_source[arg_parser->args_index]
    );
    /** 
     * NOTE: We don't care if we are out the list and NULL was returned, we
     * check for it every time anyways.
     */
    arg_parser->args_index++;
    return param;
}

static
void
P_NK_ArgumentParserPerform(
    NK_ArgumentParser* arg_parser,
    const NK_C8* argument
)
{
    /**
     * NOTE: We can assume this function will always have a valid argument,
     * since we did check up before anyway on NK_ArgumentParserStep, remember?
     */
    NK_SubmergedString* maybe_action = 
        (NK_SubmergedString*)(NK_MapGet(&arg_parser->links, argument));

    NK_ArgumentParserActionFunction* maybe_function;
    if(maybe_action == NULL)
    {
        arg_parser->state =
            NK_ENUMS_ARGUMENT_PARSER_STATE_DIED;
        arg_parser->error_register = 
            NK_ENUMS_ARGUMENT_PARSER_ERROR_NO_LINK_FOUND;
        return;
    }

    /** We continue: */
    maybe_function = 
        (NK_ArgumentParserActionFunction*)(
            NK_MapGet(
                &arg_parser->actions,
                NK_SubmergedStringGet(maybe_action)
            )
        );
    if(maybe_function == NULL)
    {
        arg_parser->state =
            NK_ENUMS_ARGUMENT_PARSER_STATE_DIED;
        arg_parser->error_register =
            NK_ENUMS_ARGUMENT_PARSER_ERROR_NO_ACTION_FOUND;
        return;
    }
    
    /** In this very case, we have an valid argument: */
    const NK_Result should_continue = (*maybe_function)(arg_parser);
    if(!should_continue)
    {
        arg_parser->state =
            NK_ENUMS_ARGUMENT_PARSER_STATE_DIED;
        arg_parser->error_register =
            NK_ENUMS_ARGUMENT_PARSER_ERROR_USER_TRIGGER;
    }
}

NK_U8
NK_ArgumentParserStep(
    NK_ArgumentParser* arg_parser
)
{
    if(arg_parser->state == NK_ENUMS_ARGUMENT_PARSER_STATE_RUNNING)
    {
        /** Can we pull from the list? */
        if(arg_parser->args_index >= arg_parser->args_length)
        {
            arg_parser->state = NK_ENUMS_ARGUMENT_PARSER_STATE_FINISHED;
        }
        else
        {
            /** If we have, we can pull from the list: */
            P_NK_ArgumentParserPerform(
                arg_parser,
                NK_ArgumentParserPull(arg_parser)
            );
        }
    }
    return arg_parser->state;
}

const NK_U8
NK_ArgumentParserGetState(
    NK_ArgumentParser* arg_parser
)
{
    return arg_parser->state;
}

const NK_U8
NK_ArgumentParserGetErrorRegister(
    NK_ArgumentParser* arg_parser
)
{
    return arg_parser->error_register;
}