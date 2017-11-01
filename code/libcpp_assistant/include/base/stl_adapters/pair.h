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
 * pair.h
 *
 *  Created on: 2017-09-23
 *      Author: wenxiongchang
 * Description: An imitator of std::pair when CA_USE_STL is not defined,
 *              or exactly std::pair when CA_USE_STL is defined.
 */

#ifndef __CPP_ASSISTANT_STL_PAIR_H__
#define __CPP_ASSISTANT_STL_PAIR_H__

#ifndef CA_USE_STL

namespace std
{

template<class t1, class t2>
struct pair
{
    typedef t1 first_type;
    typedef t2 second_type;

    t1 first;
    t2 second;

    pair() : first(), second() { }
    pair(const t1& a, const t2& b) : first(a), second(b) { }
    //pair(t1& a, t2& b) : first(a), second(b) { }
};

template<typename t1, typename t2>
inline pair<t1, t2> make_pair(const t1 &x, const t2 &y)
{
    return pair<t1, t2>(x, y);
}

} // namespace std

#endif // #ifndef CA_USE_STL

#endif /* __CPP_ASSISTANT_STL_PAIR_H__ */