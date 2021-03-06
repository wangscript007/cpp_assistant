/*
 * Copyright (c) 2016-2019, Wen Xiongchang <udc577 at 126 dot com>
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
 * framework_base_info.h
 *
 *  Created on: 2016/11/14
 *      Author: wenxiongchang(AKA: wxc)
 * Description: Some basic info about casdk framework.
 */

#ifndef __CASDK_FRAMEWORK_BASE_INFO_H__
#define __CASDK_FRAMEWORK_BASE_INFO_H__

/*
 * Change Log of casdk framework (in reversed order !!!)
 *
 * Format is defined as below:
 * Author, Date, Version:
 *      Descriptions.
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 * WARNING: DO NOT change the format of these comments
 *     because commands in Makefiles depend on it to determine some compile info!!!
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 * wxc, 2018/04/21, 0.0.07:
 *      Added a new configuration item: /root/private/server-types/item
 *
 * wxc, 2018/04/19, 0.0.06:
 *      Added support for JSON format message handling, and all header fields were turned into signed type.
 *
 * wxc, 2018/04/15, 0.0.05:
 *      Refactoring under the rules of new code style.
 *
 * wxc, 2017/05/09, 0.0.04:
 *      Finished single-threading packet handling flow.
 *
 * wxc, 2017/04/25, 0.0.03:
 *      Corrected errors in heart beat and identity report operations.
 *
 * wxc, 2016/11/17, 0.0.02:
 *      Added: command line parsing.
 *
 * wxc, 2016/11/14, 0.0.01:
 *      Created.
 */

#ifndef CASDK_FRAMEWORK_NAME
#define CASDK_FRAMEWORK_NAME                "cafw-tcp_app"
#endif

#endif /* __CASDK_FRAMEWORK_BASE_INFO_H__ */
