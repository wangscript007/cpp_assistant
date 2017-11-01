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
 * ca_signal.h
 *
 *  Created on: 2017/09/23
 *      Author: wenxiongchang
 * Description: Signal capture and handling in single-threading and multi-threading.
 */

#ifndef __CPP_ASSISTANT_SIGNAL_OPERATIONS_H__
#define __CPP_ASSISTANT_SIGNAL_OPERATIONS_H__

#include <signal.h>

#include "base/ca_inner_necessities.h"

CA_LIB_NAMESPACE_BEGIN

enum
{
    MIN_SIGNAL_NUM = SIGHUP,
#ifdef _NSIG // TODO: This macro seems to be a private macro, it may be not safe.
    MAX_SIGNAL_NUM = _NSIG - 1,
#else
    MAX_SIGNAL_NUM = 64, // TODO: It may be not this value if the OS kernel improves in future.
#endif
    SIGNAL_COUNT = MAX_SIGNAL_NUM - MIN_SIGNAL_NUM + 1,

    // Actually, numbers beyond the range of [MIN_SIGNAL_NUM, MAX_SIGNAL_NUM] are all invalid,
    // we specify one for the use of some cases that need a concrete value.
    INVALID_SIGNAL_NUM = -1,
};

enum enum_sigal_status
{
    SIG_NOT_REGISTERED = -1,
    SIG_NOT_TRIGGERED = 0,
    SIG_TRIGGERED = 1
};

// This handler is supposed to return OK on success and return a negative number on failure.
typedef int (*singal_handler)(int sig_num);

typedef struct signal_setting_t
{
    int status;
    singal_handler handler;
    bool exits_after_handling;
    bool handles_now;
}signal_setting_t;

class Signal
{
/* ===================================
 * constructors:
 * =================================== */
private:
    Signal();

/* ===================================
 * copy control:
 * =================================== */
private:
    Signal(const Signal& src);
    Signal& operator=(const Signal& src);

/* ===================================
 * destructor:
 * =================================== */
public:

/* ===================================
 * types:
 * =================================== */
public:

/* ===================================
 * abilities:
 * =================================== */
public:
    // Registers a signal whose number is @sig_num, a handler specified by @sig_handler is used to
    // execute some operations everytime the target signal occurs. Besides, if @exits_after_handling
    // is true, then a flag somewhere will be set true to remind the calling process
    // that it should exit.
    static int Register(int sig_num,
        singal_handler sig_handler,
        bool exits_after_handling = false,
        bool handles_now = false);

    // Cancels the registration of signal @sig_num so that this signal would not be monitored and captured.
    static int Unregister(int sig_num);

    // Handles a single signal with the number @sig_num, and gets a hint
    // whether the calling process should exit.
    // NOTE: A signal is marked everytime it occurs, but is not handled until this function or handle_all()
    // is called.
    static int HandleOne(const int sig_num, bool &should_exit);

    // Handles all pending signals.
    // Returns the number of signals handled on success, or a negative number on failure.
    static int HandleAll(bool &should_exit);

/* ===================================
 * attributes:
 * =================================== */
public:
    DEFINE_CLASS_NAME_FUNC()

/* ===================================
 * status:
 * =================================== */
public:
    static bool is_registered(int sig_num);

    static inline CA_REENTRANT bool is_valid(int sig_num)
    {
        return (sig_num >= MIN_SIGNAL_NUM) && (sig_num <= MAX_SIGNAL_NUM);
    }

/* ===================================
 * operators:
 * =================================== */
public:

/* ===================================
 * private methods:
 * =================================== */
protected:

/* ===================================
 * data:
 * =================================== */
protected:
    DECLARE_CLASS_NAME_VAR();
};

CA_LIB_NAMESPACE_END

#endif // __CPP_ASSISTANT_SIGNAL_OPERATIONS_H__

