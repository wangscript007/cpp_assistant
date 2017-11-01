/*
 * Copyright (c) 2017, Wen Xiongchang <udc577 at 126 dot com>
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

/*
 * debug.h
 *
 *  Created on: 2017-09-10
 *      Author: wenxiongchang
 * Description: Providing easy ways of outputting readable and well-formatted info
 *              during debugging.
 */

#ifndef __CPP_ASSISTANT_PRIVATE_DEBUG_H__
#define __CPP_ASSISTANT_PRIVATE_DEBUG_H__

#include <stdio.h>

#include <typeinfo>

#include "base/ca_inner_necessities.h"
#include "base/platforms/threading.h"

CA_LIB_NAMESPACE_BEGIN

#define HIGHLIGHT_COLOR_RED                         "\033[1;31m"
#define HIGHLIGHT_COLOR_YELLOW                      "\033[1;33m"
#define NO_PRINT_ATTRIBUTES                         "\033[0m"

#define WARNING_COLOR                               HIGHLIGHT_COLOR_YELLOW
#define ERROR_COLOR                                 HIGHLIGHT_COLOR_RED
#define FATAL_COLOR                                 HIGHLIGHT_COLOR_RED

void __enable_output_prefix(void);

void __disable_output_prefix(void);

//void __formatted_output(int level, FILE *where, mutex_t *lock, const char *format, va_list args);

void __set_debug_output(const FILE *where);

const FILE* __get_debug_output_holder(void);

namespace debug
{

extern bool g_debug_enabled;

}

inline bool __debug_is_enabled(void)
{
    return debug::g_debug_enabled;
}

bool __debug_macro_is_defined(void);

void __set_debug_lock(const mutex_t *lock);

void __debug(const char *format, ...) CA_NOTNULL(1) CA_PRINTF_CHECK(1, 2);

void __set_error_output(const FILE *where);

const FILE* __get_error_output_holder(void);

bool __error_report_is_enabled(void);

void __set_error_lock(const mutex_t *lock);

void __warn(const char *format, ...) CA_NOTNULL(1) CA_PRINTF_CHECK(1, 2);

void __error(const char *format, ...) CA_NOTNULL(1) CA_PRINTF_CHECK(1, 2);

#define __output(func, class_str, ns_delim, fmt, ...)   \
    func("%s, %s%s%s(): Line %d: "fmt, __FILE__, class_str, ns_delim, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define debug_is_enabled                                __debug_is_enabled
#define debug(fmt, ...)                                 __output(__debug, "", "", fmt, ##__VA_ARGS__)
#define cdebug(fmt, ...)                                __output(__debug, typeid(*this).name(), "::", fmt, ##__VA_ARGS__)
#define csdebug(_class_, fmt, ...)                      __output(__debug, _class_::class_name(), "::", fmt, ##__VA_ARGS__)
#define nsdebug(ns_str, fmt, ...)                       __output(__debug, ns_str, "::", fmt, ##__VA_ARGS__)

#define __error_report_is_enabled                       __error_report_is_enabled

#define warn(fmt, ...)                                  __output(__warn, "", "", fmt, ##__VA_ARGS__)
#define cwarn(fmt, ...)                                 __output(__warn, typeid(*this).name(), "::", fmt, ##__VA_ARGS__)
#define cswarn(_class_, fmt, ...)                       __output(__warn, _class_::class_name(), "::", fmt, ##__VA_ARGS__)
#define nswarn(ns_str, fmt, ...)                        __output(__warn, ns_str, "::", fmt, ##__VA_ARGS__)

#define error(fmt, ...)                                 __output(__error, "", "", fmt, ##__VA_ARGS__)
#define cerror(fmt, ...)                                __output(__error, typeid(*this).name(), "::", fmt, ##__VA_ARGS__)
#define cserror(_class_, fmt, ...)                      __output(__error, _class_::class_name(), "::", fmt, ##__VA_ARGS__)
#define nserror(ns_str, fmt, ...)                       __output(__error, ns_str, "::", fmt, ##__VA_ARGS__)

CA_LIB_NAMESPACE_END

#endif /* __CPP_ASSISTANT_PRIVATE_DEBUG_H__ */