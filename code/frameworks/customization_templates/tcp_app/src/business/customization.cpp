// This file was generated by $CPP_ASSISTANT_ROOT/scripts/generate_makefile_from_cpp_assistant.sh.
// Edit it before it can be practically used.

/*
 * Copyright (c) 2016-2018, Wen Xiongchang <udc577 at 126 dot com>
 * All rights reserved.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must
 * not claim that you wrote the original software. If you use this
 * software in a product, an acknowledgment in the product documentation
 * would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 */

// NOTE: The original author also uses (short/code) names listed below,
//       for convenience or for a certain purpose, at different places:
//       wenxiongchang, wxc, Damon Wen, udc577

#include <stdio.h>

#include "framework_public/base/all.h"
#include "framework_public/common_configurations.h"
#include "framework_public/signal_registration.h"
#include "framework_public/timed_task_scheduler.h"
#include "framework_public/handler_component_definitions.h"
#include "framework_public/protocol_common.h"
#include "framework_public/customization.h"

#include "packet_processor.h"

static const calns::command_line::user_option s_kPrivateOptions[]
/*const calns::command_line::user_option g_kPrivateOptions[]*/ = {
    {
        /* .full_name = */"l,list",
        /* .desc = */"列出所支持的命令码或错误码含义。",
        /* .least_value_count = */1,
        /* .value_count_is_fixed = */true,
        /* .assign_expression = */"=c[ommandcode]|e[rrorcode]",
        /* .default_values = */"commandcode"
    },
    {
        /* .full_name = */"x,extended-option",
        /* .desc = */"扩展选项。",
        /* .least_value_count = */1,
        /* .value_count_is_fixed = */true,
        /* .assign_expression = */" [选项值1 选项值2 ...]",
        /* .default_values = */NULL
    },
    // TODO: Add your own options here, or do nothing if there is none.
    { NULL }
};

const calns::command_line::user_option *g_kPrivateOptions = s_kPrivateOptions;

static void print_return_code_descriptions(void)
{
    ;
}

const char *get_return_code_description(int retcode)
{
    ; // TODO
    return "UNKNOWN_ERROR";
}

int check_private_commandline_options(calns::command_line &cmdline, bool &should_exit)
{
    const calns::command_line::option_value_t *op_val = NULL;

    op_val = cmdline.get_option_value("list");
    if (op_val->is_specified)
    {
        const char *list_target = op_val->values[0].c_str();

        if ('c' == list_target[0])
            cafw::packet_processor::print_supported_commands();
        else
            print_return_code_descriptions();

        should_exit = true;

        return 0;
    }

    op_val = cmdline.get_option_value("extended-option");
    if (op_val->is_specified)
    {
        printf("Value(s) of extended option:");
        for (size_t i = 0; i < op_val->values.size(); ++i)
        {
            printf(" [%s]", op_val->values[i].c_str());
        }
        printf("\n");
    }

    // TODO: Add your own stuff here, or do nothing if there is none.

    return 0;
}

int init_extra_config(struct extra_config_t **extra_items)
{
    if (NULL == extra_items)
    {
        LOGF_NS(E, "", "null param\n");
        return RET_FAILED;
    }

    if (NULL != *extra_items)
    {
        LOGF_NS(I, "", "extra configuration structure already initialized\n");
        return RET_OK;
    }

    if (NULL == (*extra_items = new extra_config_t))
    {
        LOGF_NS(E, "", "new extra_conf_t failed\n");
        return RET_FAILED;
    }

    // TODO: Allocate and initialize resources for your own configuration items here,
    //     or do nothing if not needed.

    return RET_OK;
}

int destroy_extra_config(struct extra_config_t **extra_items)
{
    if (NULL == extra_items)
    {
        LOGF_NS(E, "", "null param\n");
        return RET_FAILED;
    }

    if (NULL == extra_items)
    {
        LOGF_NS(I, "", "extra configuration structure already destroyed\n");
        return RET_OK;
    }

    // TODO: Clean or release resources of your own configuration items here,
    //     or do nothing if not needed.

    delete (*extra_items);
    *extra_items = NULL;

    return RET_OK;
}

int load_extra_config(struct extra_config_t *extra_items)
{
    // TODO: Load values of your own configuration items here, or do nothing if there is none.
    return RET_OK;
}

int reload_partial_extra_config(struct extra_config_t *extra_items)
{
    // TODO: Reload partial values of your own configuration items here, or do nothing if there is none.
    return RET_OK;
}

int prepare_extra_resource(const void *condition, struct extra_resource_t *target)
{
    // TODO: Prepare your own resources here, or do nothing if there is none.
    return RET_OK;
}

void release_extra_resource(struct extra_resource_t **target)
{
    ; // TODO: Release your own resources here, or do nothing if there is none.
}

// TODO: If you want to define your own customized signal handlers,
//     delete this line and write your handlers.
SET_ALL_CUSTOMIZED_SIG_HANDLERS_TO_DEFAULT();

// NOTE: Contents below are just for test, feel free to delete them in your project.
/********************************** Simple tests begin **********************************/

enum
{
    CMD_SIMPLE_SINGLE_PACKET_TEST_REQ = 0x00020000,
    CMD_SIMPLE_SINGLE_PACKET_TEST_RESP = 0x00020001,
    CMD_SIMPLE_MULTI_PACKET_TEST_REQ = 0x00020002,
    CMD_SIMPLE_MULTI_PACKET_TEST_RESP = 0x00020003
};

#ifndef USE_JSON_MSG

typedef cafw::MinimalBody SimpleSinglePacketTestReq;
typedef cafw::MinimalBody SimpleSinglePacketTestResp;

typedef cafw::MinimalBody SimpleMultiPacketTestResp;

#endif // #ifndef USE_JSON_MSG

int g_send_packet_count = 0;
int g_recv_packet_count = 0;

void make_test_packets(void)
{
    char sid[SID_LEN + 1];
    const int64_t kRouteIdForSinglePacket = 0;
#ifndef USE_JSON_MSG
    const int64_t kRouteIdForMultiPacket = 10;
    SimpleSinglePacketTestReq single_packet_req;
    cafw::SimpleMultiPacketTestReq multi_packet_req;
#else
    Json::Value single_packet_req;
#endif
    int send_bytes = 0;

    memset(sid, 0, sizeof(sid));
    make_session_id(&kRouteIdForSinglePacket, sizeof(sid), sid);
#ifndef USE_JSON_MSG
    single_packet_req.set_session_id(sid);
#else
    single_packet_req[SID_KEY_STR] = sid;
#endif
    send_bytes = send_lite_packet("casdk_server", 1, CMD_SIMPLE_SINGLE_PACKET_TEST_REQ, PROTO_RET_SUCCESS,
        &single_packet_req, false, cafw::DISPATCHED_RANDOMLY, kRouteIdForSinglePacket);
    if (send_bytes > 0)
        ++g_send_packet_count;

#ifndef USE_JSON_MSG
    memset(sid, 0, sizeof(sid));
    make_session_id(&kRouteIdForMultiPacket, sizeof(sid), sid);
    multi_packet_req.set_session_id(sid);
    multi_packet_req.add_packet_number(0);
    for (int i = 0; i < 3; ++i)
    {
        multi_packet_req.set_packet_number(0, i + 1);
        send_bytes = send_lite_packet("casdk_server", 1, CMD_SIMPLE_MULTI_PACKET_TEST_REQ, PROTO_RET_SUCCESS,
            &multi_packet_req, false, cafw::DISPATCHED_BY_ID, kRouteIdForMultiPacket,
            (3 == i + 1), i + 1);
        if (send_bytes > 0)
            ++g_send_packet_count;
    }
#endif // #ifndef USE_JSON_MSG
}

cafw::timed_task_info_t g_customized_timed_tasks[] = {
    /*
     * {
     *      task name,
     *      { trigger_type, has_triggered, event_time/last_op_time[us], time_offset/time_interval[ms], operation }
     * }
     */

    /********************************** Simple test begin **********************************/
    {
        "simple_test",
        { cafw::timed_task_config::TRIGGERED_PERIODICALLY,   true,   {0},    {15000},    make_test_packets }
    },
    /********************************** Simple test end **********************************/

    // TODO: Add your own stuff here, or add nothing if there is none.

    {
        NULL,
        {}
    }
};

int init_business(int argc, char **argv)
{
    // TODO: Add your own stuff, or do nothing if there is none.
    return RET_OK;
}

// This function will be run in a loop periodically if @exit_after_this_round is set to false,
// otherwise will be run only once if @exit_after_this_round if set to true.
// Modify this function to meet your needs.
int run_private_business(bool &exit_after_this_round)
{
    // TODO: Add your own business here

#if 1 // Just for test. Feel free to remove these lines.
    struct timeval tv;
    struct tm now;

    gettimeofday(&tv, NULL);
    localtime_r((time_t *)&(tv.tv_sec), &now);
    if (0 == now.tm_hour && now.tm_min >= 1)
        exit_after_this_round = true;
#endif

    return RET_OK;
}

void finalize_business(void)
{
    RLOGF(I, "g_send_packet_count = %d, g_recv_packet_count = %d\n",
        g_send_packet_count, g_recv_packet_count);
    ; // TODO: Add your own stuff, or do nothing if there is none.
}

bool proto_uses_general_prefix(const int32_t cmd)
{
    // TODO: Add some command codes for protocols that do not use a general body prefix,
    //     if there're any.
    return true;
}

int parse_proto_header_extension(const char *raw_extension, char *parsed_extension)/*  CA_NOTNULL(1,2) */
{
    // TODO: Define the method of parsing the extension field of the protocol header here, or do nothing if not needed.
    return RET_OK;
}

int assemble_proto_header_extension(const char *parsed_extension, char *serialized_extension)/*  CA_NOTNULL(1,2) */
{
    // TODO: Define the method of assembling the extension field of the protocol header here, or do nothing if not needed.
    return RET_OK;
}

int make_session_id(const void *condition, const int sid_holder_size, char *sid_result)
{
    /*
     * TODO: This is a default version, you can modify it and implement yours.
     */

    uint64_t d1 = calns::time_util::get_utc_microseconds();
    uint64_t d2 = calns::time_util::get_utc_microseconds();
    uint64_t d3 = calns::time_util::get_utc_microseconds();
    uint64_t d4 = *((uint64_t *)condition);

    memset(sid_result, 0, sid_holder_size);
    snprintf(sid_result, sid_holder_size, "%08lx%08lx%08lx%08lx", d1, d2, d3, d4);
    sid_result[sid_holder_size - 1] = '\0';

    return RET_OK;
}

bool session_exists(const char *sid)
{
    ; // TODO: Implement it!
    return false;
}

int fetch_session_info(const char *sid, void* outbuf, int &outlen)
{
    ; // TODO: Implement it!
    return RET_OK;
}

int save_session_info(const struct calns::net_connection *src_conn, const void* buf, int buflen, bool commit_now/* = false*/)
{
    ; // TODO: Implement it!
    return RET_OK;
}

int save_session_info(const struct calns::net_connection *src_conn, const cafw::msg_base *body, const cafw::proto_header_t &header, bool commit_now/* = false*/)
{
    ; // TODO: Implement it!
    return RET_OK;
}

int update_session_info(const void *buf, int buflen, bool commit_now/* = false*/)
{
    ; // TODO: Implement it!
    return RET_OK;
}

int update_session_info(const cafw::msg_base *body, const cafw::proto_header_t &header, bool commit_now/* = false*/)
{
    ; // TODO: Implement it!
    return RET_OK;
}

void clean_expired_sessions(void)
{
    ; // TODO: Implement it!
}

bool message_is_time_consuming(const int32_t cmd)
{
    ; // TODO: Implement it!
    return true;
}

// NOTE: Contents below are just for test, feel free to delete them in your project.
/********************************** Simple tests begin **********************************/

/*
 * Forward declarations.
 */

SET_GROUP_FUNC_TO_NULL(single_packet_request_test);
SET_VALIDATE_FUNC_TO_NULL(single_packet_request_test);
DECLARE_BUSINESS_FUNC(single_packet_request_test);
SET_ALLOC_FUNC_TO_NULL(single_packet_request_test);
#ifndef USE_JSON_MSG
DEFINE_DEFAULT_ASSEMBLE_OUT_FUNC(single_packet_request_test, SimpleSinglePacketTestReq, SimpleSinglePacketTestResp)
#else
SET_ASSEMBLE_OUT_FUNC(single_packet_request_test, cafw::ASSEMBLE_OUT_PACKET_FUNC(minimal));
#endif

SET_GROUP_FUNC_TO_NULL(single_packet_response_test);
SET_VALIDATE_FUNC_TO_NULL(single_packet_response_test);
DECLARE_BUSINESS_FUNC(single_packet_response_test);
SET_ALLOC_FUNC_TO_NULL(single_packet_response_test);
SET_ASSEMBLE_OUT_FUNC_TO_NULL(single_packet_response_test);

#ifndef USE_JSON_MSG

DECLARE_GROUP_FUNC(multi_packet_request_test);
SET_VALIDATE_FUNC_TO_NULL(multi_packet_request_test);
DECLARE_BUSINESS_FUNC(multi_packet_request_test);
DEFINE_ALLOC_FUNC(multi_packet_request_test, cafw::SimpleMultiPacketTestReq)
DEFINE_DEFAULT_ASSEMBLE_OUT_FUNC(multi_packet_request_test, cafw::SimpleMultiPacketTestReq, SimpleMultiPacketTestResp)

SET_GROUP_FUNC_TO_NULL(multi_packet_response_test);
SET_VALIDATE_FUNC_TO_NULL(multi_packet_response_test);
DECLARE_BUSINESS_FUNC(multi_packet_response_test);
SET_ALLOC_FUNC_TO_NULL(multi_packet_response_test);
SET_ASSEMBLE_OUT_FUNC_TO_NULL(multi_packet_response_test);

#endif // #ifndef USE_JSON_MSG

/*
 * Implementations.
 */

DECLARE_BUSINESS_FUNC(single_packet_request_test)
{
    ++g_recv_packet_count;
    LOGF_NS(I, "", "g_recv_packet_count = %d\n", g_recv_packet_count);
    return RET_OK;
}

DECLARE_BUSINESS_FUNC(single_packet_response_test)
{
    ++g_recv_packet_count;
    LOGF_NS(I, "", "g_recv_packet_count = %d\n", g_recv_packet_count);
    return RET_OK;
}

#ifndef USE_JSON_MSG

DECLARE_GROUP_FUNC(multi_packet_request_test)
{
    DECLARE_AND_CAST(fragment, converted_frag, const cafw::SimpleMultiPacketTestReq);
    DECLARE_AND_CAST(whole, converted_whole, cafw::SimpleMultiPacketTestReq);

    if (NULL == converted_frag)
    {
        LOGF_NS(E, "", "failed to convert \"fragment\" pointer from msg_base* to"
            " SimpleMultiPacketTestReq*\n");
        return RET_FAILED;
    }

    if (NULL == converted_whole)
    {
        LOGF_NS(E, "", "failed to convert \"whole\" pointer from msg_base* to"
            " SimpleMultiPacketTestReq*\n");
        return RET_FAILED;
    }

    converted_whole->add_packet_number(converted_frag->packet_number(0));

    return RET_OK;
}

DECLARE_BUSINESS_FUNC(multi_packet_request_test)
{
    DECLARE_AND_CAST(in_body, req, const cafw::SimpleMultiPacketTestReq);

    for (int i = 0; i < req->packet_number_size(); ++i)
    {
        LOGF_NS(I, "", "packet[%d] was handled successfully\n", req->packet_number(i));
    }
    g_recv_packet_count += req->packet_number_size();
    LOGF_NS(I, "", "g_recv_packet_count = %d\n", g_recv_packet_count);

    return RET_OK;
}

DECLARE_BUSINESS_FUNC(multi_packet_response_test)
{
    ++g_recv_packet_count;
    LOGF_NS(I, "", "g_recv_packet_count = %d\n", g_recv_packet_count);
    return RET_OK;
}

#endif // #ifndef USE_JSON_MSG

/********************************** Simple tests end **********************************/

cafw::handler_component g_packet_handler_components[] = {
    /********************************** Simple tests begin **********************************/
    {
        CMD_SIMPLE_SINGLE_PACKET_TEST_REQ,      CMD_SIMPLE_SINGLE_PACKET_TEST_RESP,
        "单包测试请求",                            FILTERS_REPEATED_SESSION,
#ifndef USE_JSON_MSG
        SET_BODY_CONTAINERS(NO_MULTI_FRAGMENTS, SimpleSinglePacketTestReq, HAS_OUTPUT, SimpleSinglePacketTestResp),
#else
        SET_BODY_CONTAINERS(NO_MULTI_FRAGMENTS, HAS_OUTPUT),
#endif
        SET_HANDLER_FUNCS_ONE_BY_ONE(single_packet_request_test,
            NO_GROUP_FUNC,
            NO_VALIDATION_FUNC,
            HAS_ASSEMBLE_OUT_FUNC,
            NEEDS_DB_COMMIT)
    },
    {
        CMD_SIMPLE_SINGLE_PACKET_TEST_RESP,      CMD_UNUSED,
        "单包测试响应",                            FILTERS_REPEATED_SESSION,
#ifndef USE_JSON_MSG
        SET_BODY_CONTAINERS(NO_MULTI_FRAGMENTS, SimpleSinglePacketTestResp, NO_OUTPUT, cafw::Unused),
#else
        SET_BODY_CONTAINERS(NO_MULTI_FRAGMENTS, NO_OUTPUT),
#endif
        SET_HANDLER_FUNCS_ONE_BY_ONE(single_packet_response_test,
            NO_GROUP_FUNC,
            NO_VALIDATION_FUNC,
            NO_ASSEMBLE_OUT_FUNC,
            NEEDS_DB_COMMIT)
    },
#ifndef USE_JSON_MSG
    {
        CMD_SIMPLE_MULTI_PACKET_TEST_REQ,      CMD_SIMPLE_MULTI_PACKET_TEST_RESP,
        "多包测试请求",                            FILTERS_REPEATED_SESSION,
        SET_BODY_CONTAINERS(HAS_MULTI_FRAGMENTS, cafw::SimpleMultiPacketTestReq, HAS_OUTPUT, SimpleMultiPacketTestResp),
        SET_HANDLER_FUNCS_ONE_BY_ONE(multi_packet_request_test,
            HAS_GROUP_FUNC,
            NO_VALIDATION_FUNC,
            HAS_ASSEMBLE_OUT_FUNC,
            NEEDS_DB_COMMIT)
    },
    {
        CMD_SIMPLE_MULTI_PACKET_TEST_RESP,      CMD_UNUSED,
        "多包测试响应",                            FILTERS_REPEATED_SESSION,
        SET_BODY_CONTAINERS(NO_MULTI_FRAGMENTS, SimpleMultiPacketTestResp, NO_OUTPUT, cafw::Unused),
        SET_HANDLER_FUNCS_ONE_BY_ONE(multi_packet_response_test,
            NO_GROUP_FUNC,
            NO_VALIDATION_FUNC,
            NO_ASSEMBLE_OUT_FUNC,
            NEEDS_DB_COMMIT)
    },
#endif // #ifndef USE_JSON_MSG
    /********************************** Simple tests end **********************************/
    // TODO: Fill in your own components.
    { CMD_UNUSED /* Other fields are not cared. */}
};
