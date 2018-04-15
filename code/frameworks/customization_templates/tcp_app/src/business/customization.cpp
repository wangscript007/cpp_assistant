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
#include <cpp_assistant/ca_full.h>

#include "framework_public/base/all.h"
#include "framework_public/common_configurations.h"
#include "framework_public/signal_registration.h"
#include "framework_public/timed_task_scheduler.h"
#include "framework_public/handler_component_definitions.h"
#include "framework_public/protocol_common.h"
#include "framework_public/customization.h"

#include "packet_processor.h"

static const cal::command_line::user_option s_kPrivateOptions[]
/*const cal::command_line::user_option g_kPrivateOptions[]*/ = {
    {
        /* .name = */"l,list",
        /* .desc = */"列出所支持的命令码或错误码含义。",
        /* .least_value_count = */1,
        /* .value_count_is_fixed = */true,
        /* .assign_expression = */"=c[ommandcode]|e[rrorcode]",
        /* .default_values = */"commandcode"
    },
    {
        /* .name = */"x,extended-option",
        /* .desc = */"扩展选项。",
        /* .least_value_count = */1,
        /* .value_count_is_fixed = */true,
        /* .assign_expression = */" [选项值1 选项值2 ...]",
        /* .default_values = */NULL
    },
    // TODO: Add your own options here, or do nothing if there is none.
    { NULL }
};

const cal::command_line::user_option *g_kPrivateOptions = s_kPrivateOptions;

static void print_return_code_descriptions(void)
{
    ;
}

const char *get_return_code_description(int retcode)
{
    ; // TODO
    return "UNKNOWN_ERROR";
}

int check_private_commandline_options(cal::command_line &cmdline, bool &should_exit)
{
    const cal::command_line::option_entry *op_val = NULL;

    op_val = cmdline.get_option_entry("list");
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

    op_val = cmdline.get_option_entry("extended-option");
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

// TODO: If you want to define your own customized signal handlers,
//     delete this line and write your handlers.
SET_ALL_CUSTOMIZED_SIG_HANDLERS_TO_DEFAULT();

int init_extra_config(struct extra_config_t **extra_items)
{
    if (NULL == extra_items)
    {
        GLOG_ERROR_NS("", "null param\n");
        return RET_FAILED;
    }

    if (NULL != *extra_items)
    {
        GLOG_INFO_NS("", "extra configuration structure already initialized\n");
        return RET_OK;
    }

    if (NULL == (*extra_items = new extra_config_t))
    {
        GLOG_ERROR_NS("", "new extra_conf_t failed\n");
        return RET_FAILED;
    }

    // TODO: Allocate and initialize resources for your own configuration items here,
    //     or do nothing if unnecessary.

    return RET_OK;
}

int destroy_extra_config(struct extra_config_t **extra_items)
{
    if (NULL == extra_items)
    {
        GLOG_ERROR_NS("", "null param\n");
        return RET_FAILED;
    }

    if (NULL == extra_items)
    {
        GLOG_INFO_NS("", "extra configuration structure already destroyed\n");
        return RET_OK;
    }

    // TODO: Clean or release resources of your own configuration items here,
    //     or do nothing if unnecessary.

    delete (*extra_items);
    *extra_items = NULL;

    return RET_OK;
}

int load_extra_config(struct extra_config_t *extra_items)
{
    // TODO: Load values of your own configuration items here, or do nothing if there is none.
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

const char *g_timed_task_names[] = {
    /********************************** Simple tests begin **********************************/
    "simple_test",
    /********************************** Simple tests end **********************************/
    // TODO: Add your own stuff, or do nothing if there is none.
    NULL
};

// NOTE: Contents below are just for test, feel free to delete them in your project.
/********************************** Simple tests begin **********************************/

enum
{
    CMD_SIMPLE_SINGLE_PACKET_TEST_REQ = 0x00020001,
    CMD_SIMPLE_SINGLE_PACKET_TEST_RESP = 0x00020002,
    CMD_SIMPLE_MULTI_PACKET_TEST_REQ = 0x00020003,
    CMD_SIMPLE_MULTI_PACKET_TEST_RESP = 0x00020004
};

typedef cafw::MinimalBody SimpleSinglePacketTestReq;
typedef cafw::MinimalBody SimpleSinglePacketTestResp;

typedef cafw::MinimalBody SimpleMultiPacketTestResp;

int g_send_packet_count = 0;
int g_recv_packet_count = 0;

void make_test_packets(void)
{
    const uint64_t kRouteIdForSinglePacket = 0;
    const uint64_t kRouteIdForMultiPacket = 10;
    char sid[SID_LEN + 1];
    SimpleSinglePacketTestReq single_packet_req;
    cafw::SimpleMultiPacketTestReq multi_packet_req;
    int send_bytes = 0;

    memset(sid, 0, sizeof(sid));
    make_session_id(&kRouteIdForSinglePacket, sizeof(sid), sid);
    single_packet_req.set_session_id(sid);
    send_bytes = send_lite_packet("casdk_server", 1, CMD_SIMPLE_SINGLE_PACKET_TEST_REQ, PROTO_RET_SUCCESS,
        &single_packet_req, false, cafw::DISPATCHED_RANDOMLY, kRouteIdForSinglePacket);
    if (send_bytes > 0)
        ++g_send_packet_count;

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
}

/********************************** Simple tests end **********************************/

cafw::timed_task_config g_timed_task_settings[] = {
    /********************************** Simple tests begin **********************************/
    { cafw::timed_task_config::TRIGGERED_PERIODICALLY,   true,   {0},    {15000},    make_test_packets },
    /********************************** Simple tests end **********************************/
    // trigger_type, event_time/last_op_time, time_offset/time_interval, operation
    // TODO: Add your own stuff, or do nothing if there is none.
    { cafw::timed_task_config::TRIGGERED_PERIODICALLY,   true,   {0},    {0},    NULL }
};

int init_business(void)
{
    // TODO: Add your own stuff, or do nothing if there is none.
    return RET_OK;
}

int run_private_business(bool &should_exit)
{
    // TODO: Add your own business here

#if 1 // Just for test. Feel free to remove these lines.
    struct timeval tv;
    struct tm now;

    gettimeofday(&tv, NULL);
    localtime_r((time_t *)&(tv.tv_sec), &now);
    if (0 == now.tm_hour && now.tm_min >= 1)
        should_exit = true;
#endif

    return RET_OK;
}

void finalize_business(void)
{
    GLOG_INFO("g_send_packet_count = %d, g_recv_packet_count = %d\n",
        g_send_packet_count, g_recv_packet_count);
    ; // TODO: Add your own stuff, or do nothing if there is none.
}

bool proto_uses_general_prefix(const uint32_t cmd)
{
    // TODO: Add some command codes for protocols that do not use a general body prefix,
    //     if there're any.
    return true;
}

int make_session_id(const void *condition, const int sid_holder_size, char *sid_result)
{
    /*
     * TODO: This is a default version, you can modify it and implement yours.
     */

    uint64_t d1 = cal::time_util::get_utc_microseconds();
    uint64_t d2 = cal::time_util::get_utc_microseconds();
    uint64_t d3 = cal::time_util::get_utc_microseconds();
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

int save_session_info(const struct cal::net_connection *src_conn, const void* buf, int buflen, bool commit_now/* = false*/)
{
    ; // TODO: Implement it!
    return RET_OK;
}

int save_session_info(const struct cal::net_connection *src_conn, const cafw::msg_base *body, const cafw::proto_header_t &header, bool commit_now/* = false*/)
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

bool message_is_time_consuming(const uint32_t cmd)
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
DEFINE_DEFAULT_ASSEMBLE_OUT_FUNC(single_packet_request_test, SimpleSinglePacketTestReq, SimpleSinglePacketTestResp)

SET_GROUP_FUNC_TO_NULL(single_packet_response_test);
SET_VALIDATE_FUNC_TO_NULL(single_packet_response_test);
DECLARE_BUSINESS_FUNC(single_packet_response_test);
SET_ALLOC_FUNC_TO_NULL(single_packet_response_test);
SET_ASSEMBLE_OUT_FUNC_TO_NULL(single_packet_response_test);

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

/*
 * Implementations.
 */

DECLARE_BUSINESS_FUNC(single_packet_request_test)
{
    ++g_recv_packet_count;
    GLOG_INFO_NS("", "g_recv_packet_count = %d\n", g_recv_packet_count);
    return RET_OK;
}

DECLARE_BUSINESS_FUNC(single_packet_response_test)
{
    ++g_recv_packet_count;
    GLOG_INFO_NS("", "g_recv_packet_count = %d\n", g_recv_packet_count);
    return RET_OK;
}

DECLARE_GROUP_FUNC(multi_packet_request_test)
{
    DECLARE_AND_CAST(fragment, converted_frag, const cafw::SimpleMultiPacketTestReq);
    DECLARE_AND_CAST(whole, converted_whole, cafw::SimpleMultiPacketTestReq);

    if (NULL == converted_frag)
    {
        GLOG_ERROR_NS("", "failed to convert \"fragment\" pointer from msg_base* to"
            " SimpleMultiPacketTestReq*\n");
        return RET_FAILED;
    }

    if (NULL == converted_whole)
    {
        GLOG_ERROR_NS("", "failed to convert \"whole\" pointer from msg_base* to"
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
        GLOG_INFO_NS("", "packet[%d] was handled successfully\n", req->packet_number(i));
    }
    g_recv_packet_count += req->packet_number_size();
    GLOG_INFO_NS("", "g_recv_packet_count = %d\n", g_recv_packet_count);

    return RET_OK;
}

DECLARE_BUSINESS_FUNC(multi_packet_response_test)
{
    ++g_recv_packet_count;
    GLOG_INFO_NS("", "g_recv_packet_count = %d\n", g_recv_packet_count);
    return RET_OK;
}

/********************************** Simple tests end **********************************/

cafw::handler_component g_packet_handler_components[] = {
    /********************************** Simple tests begin **********************************/
    {
        CMD_SIMPLE_SINGLE_PACKET_TEST_REQ,      CMD_SIMPLE_SINGLE_PACKET_TEST_RESP,
        "单包测试请求",                            FILTERS_REPEATED_SESSION,
        SET_BODY_CONTAINERS(NO_MULTI_FRAGMENTS, SimpleSinglePacketTestReq, HAS_OUTPUT, SimpleSinglePacketTestResp),
        SET_HANDLER_FUNCS_ONE_BY_ONE(single_packet_request_test,
            NO_GROUP_FUNC,
            NO_VALIDATION_FUNC,
            HAS_ASSEMBLE_OUT_FUNC,
            NEEDS_DB_COMMIT)
    },
    {
        CMD_SIMPLE_SINGLE_PACKET_TEST_RESP,      CMD_UNUSED,
        "单包测试响应",                            FILTERS_REPEATED_SESSION,
        SET_BODY_CONTAINERS(NO_MULTI_FRAGMENTS, SimpleSinglePacketTestResp, NO_OUTPUT, cafw::Unused),
        SET_HANDLER_FUNCS_ONE_BY_ONE(single_packet_response_test,
            NO_GROUP_FUNC,
            NO_VALIDATION_FUNC,
            NO_ASSEMBLE_OUT_FUNC,
            NEEDS_DB_COMMIT)
    },
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
    /********************************** Simple tests end **********************************/
    // TODO: Fill in your own components.
    { CMD_UNUSED /* Other fields are not cared. */}
};
