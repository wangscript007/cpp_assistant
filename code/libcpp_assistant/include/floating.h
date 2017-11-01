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
 * floating.h
 *
 *  Created on: 2017/09/23
 *      Author: wenxiongchang
 * Description: A template for floating-point types(float, double and long double).
 */

#ifndef __CPP_ASSISTANT_FLOATING_CALCULATION_H__
#define __CPP_ASSISTANT_FLOATING_CALCULATION_H__

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>

#include <typeinfo>

#include "base/ca_inner_necessities.h"
#include "base/ca_return_code.h"
#include "base/stl_adapters/string.h"

CA_LIB_NAMESPACE_BEGIN

#define DECIMAL_PLACE_COUNT_OUT_OF_RANGE(type, decimal_place_count)     \
    (decimal_place_count < 0 || decimal_place_count > MAX_DECIMAL_PLACE_COUNT)

#define FLOAT_TYPE_NAME                                                 "float"
#define DOUBLE_TYPE_NAME                                                "double"
#define LONG_DOUBLE_TYPE_NAME                                           "long double"

#define IS_NOT_FLOATING_TYPE(type)                                      \
    (typeid(float) != typeid(type) \
    && typeid(double) != typeid(type) \
    && typeid(long double) != typeid(type))

#define THROW_EXCEPTION_IF_NOT_FLOATING(type)                           \
    if (IS_NOT_FLOATING_TYPE(type)) \
        throw "Type("#type") is not one of floating point types: float, double, long double."

template<typename FT> class Floating;

/*
 * WARNING: FloatingAccessor is used for testings only, DO NOT use it in any practical projects!
 */
template<typename FT>
class FloatingAccessor
{
/* ===================================
 * constructors:
 * =================================== */
private:
    FloatingAccessor();

public:
    explicit FloatingAccessor(const Floating<FT>& tested_obj) : m_tested_object(tested_obj) {}

/* ===================================
 * copy control:
 * =================================== */
private:
    explicit FloatingAccessor(const FloatingAccessor& src);
    FloatingAccessor& operator=(const FloatingAccessor& src);

/* ===================================
 * destructor:
 * =================================== */
public:
    ~FloatingAccessor(){}

/* ===================================
 * abilities:
 * =================================== */
public:
    FT GetValue(void) const
    {
        return m_tested_object.m_value;
    }

    int GetDecimalPlaceCount(void) const
    {
        return m_tested_object.m_decimal_place_count;
    }

    const std::string* GetString(void) const
    {
        return m_tested_object.m_string;
    }

/* ===================================
 * data:
 * =================================== */
private:
    const Floating<FT>& m_tested_object;
}; // template class FloatingAccessor

// TODO: An exception class for Floating ??

template<typename FT>
class Floating
{
/* ===================================
 * constructors:
 * =================================== */
public:
    Floating()
        : m_value(0)
        , m_decimal_place_count(DEFAULT_DECIMAL_PLACE_COUNT)
        , m_string(NULL)
    {
        THROW_EXCEPTION_IF_NOT_FLOATING(FT);
        ChangePrecision(this->m_decimal_place_count, this->m_value, NULL, NULL);
    }

    Floating(const FT src, const int n_decimal_places = DEFAULT_DECIMAL_PLACE_COUNT)
        : m_decimal_place_count(n_decimal_places)
        , m_string(NULL)
    {
        THROW_EXCEPTION_IF_NOT_FLOATING(FT);
        this->m_value = src;
        if (this->m_decimal_place_count < 0)
            this->m_decimal_place_count = 0;
        ChangePrecision(this->m_decimal_place_count, this->m_value, NULL, NULL);
    }

/* ===================================
 * copy control:
 * =================================== */
public:
    explicit Floating(const Floating& src)
        : m_string(NULL)
    {
        THROW_EXCEPTION_IF_NOT_FLOATING(FT);
        //THROW_EXCEPTION_IF_NOT_FLOATING(src.m_value);
        this->m_value = src.m_value;
        this->m_decimal_place_count = src.m_decimal_place_count;
        if (NULL != src.m_string)
            this->m_string = new std::string(*(src.m_string));
    }

    Floating& operator=(const Floating& src)
    {
        if (this != &src)
        {
            this->m_value = src.m_value;
            this->m_decimal_place_count = src.m_decimal_place_count;

            if (NULL != this->m_string)
                ToString(src.m_value, src.m_decimal_place_count, *(this->m_string));

        }

        return *this;
    }

    Floating& operator=(const FT src)
    {
        this->m_value = src;
        this->m_decimal_place_count = DEFAULT_DECIMAL_PLACE_COUNT;

        if (NULL != this->m_string)
            ToString(this->m_value, this->m_decimal_place_count, *(this->m_string));

        return *this;
    }

/* ===================================
 * destructor:
 * =================================== */
public:
    ~Floating()
    {
        if (NULL != m_string)
        {
            delete m_string;
            m_string = NULL;
        }
    }

/* ===================================
 * types:
 * =================================== */
    /*
     * Replaces enumerations below with static member constants
     * so that they can take different values depending on their types.
     */
#if 0
    enum
    {
        // see the data part of this class.
    };
#endif

/* ===================================
 * abilities:
 * =================================== */
public:
    static CA_REENTRANT int FromString(const char *str, FT &result, const int n_decimal_places = -1)
    {
        THROW_EXCEPTION_IF_NOT_FLOATING(FT);

        if (NULL == str)
            return CA_RET(NULL_PARAM);

        if (n_decimal_places > MAX_DECIMAL_PLACE_COUNT)
            return CA_RET(VALUE_OUT_OF_RANGE);

        FT original_value = result;

        if (typeid(float) == typeid(FT))
            result = strtof(str, NULL);
        else if (typeid(double) == typeid(FT))
            result = strtod(str, NULL);
        else
            result = strtold(str, NULL);

        if (n_decimal_places >= 0)
        {
            int ret = ChangePrecision(n_decimal_places, result, NULL, NULL);

            if (ret < 0)
            {
                result = original_value;
                return ret;
            }
        }

        return CA_RET_OK;
    }

    static CA_REENTRANT int ToString(const FT num, const int n_decimal_places, std::string& output)
    {
        THROW_EXCEPTION_IF_NOT_FLOATING(FT);

        char result[CHAR_COUNT_OF_MAX_VALUE + 1] = {0};
        int result_size = sizeof(result);
        int ret = CA_RET_GENERAL_FAILURE;

        output.clear();

        if ((ret = ToString(num, n_decimal_places, result, result_size)) < 0)
            return ret;

        output.append(result);

        return CA_RET_OK;
    }

    static CA_REENTRANT int ToString(
        const FT num,
        const int n_decimal_places,
        char *output,
        int &size /* in: max buffer size, out: actual result size*/)
    {
        THROW_EXCEPTION_IF_NOT_FLOATING(FT);

        if (size <= 0
            || NULL == output)
            return CA_RET(NULL_PARAM);

        memset(output, 0, size);

        if (DECIMAL_PLACE_COUNT_OUT_OF_RANGE(FT, n_decimal_places))
        {
            size = 0;
            return CA_RET(VALUE_OUT_OF_RANGE);
        }

        char result[CHAR_COUNT_OF_MAX_VALUE + 1] = {0};
        char fmt[16] = {0};

        snprintf(fmt, sizeof(fmt), "%%.%d%s", n_decimal_places,
            (typeid(float) == typeid(FT)) ? "f"
                : ((typeid(double) == typeid(FT)) ? "lf"
                    : "Lf")); // Actually, "lf" may be the same as "f" for some compilers.
        snprintf(result, sizeof(result), fmt, num);
        strncpy(output, result, size - 1);
        size = strlen(output);

        return CA_RET_OK;
    }

    const std::string *ToString() const
    {
        if (NULL == m_string)
        {
            m_string = new std::string;
            ToString(m_value, m_decimal_place_count, *m_string);
        }

        return m_string;
    }

    static CA_REENTRANT int ChangePrecision(
        const int n_decimal_places,
        FT &target, /* in: original value, out: value that has been handled */
        char *result_in_str = NULL,
        int *str_size = NULL /* in: max buffer size, out: actual string size*/)
    {
        THROW_EXCEPTION_IF_NOT_FLOATING(FT);

        bool str_holder_available = ((NULL != result_in_str) && (NULL != str_size) && (*str_size > 0));
        char str_result[CHAR_COUNT_OF_MAX_VALUE + 1] = {0};
        int result_size = sizeof(str_result);
        int ret = CA_RET_GENERAL_FAILURE;

        if (str_holder_available)
            memset(result_in_str, 0, *str_size);

        if ((ret = ToString(target, n_decimal_places, str_result, result_size)) < 0)
            return ret;

        if (typeid(float) == typeid(FT))
            target = strtof(str_result, NULL);
        else if (typeid(double) == typeid(FT))
            target = strtod(str_result, NULL);
        else
            target = strtold(str_result, NULL);

        if (str_holder_available)
        {
            strncpy(result_in_str, str_result, *str_size - 1);
            *str_size = strlen(result_in_str);
        }

        return CA_RET_OK;
    }

    static CA_REENTRANT int ChangePrecision(
        const int n_decimal_places,
        FT &target, /* in: original value, out: value that has been handled */
        std::string *result_in_str = NULL)
    {
        THROW_EXCEPTION_IF_NOT_FLOATING(FT);

        int ret = CA_RET_GENERAL_FAILURE;

        if (NULL != result_in_str)
        {
            char str_result[CHAR_COUNT_OF_MAX_VALUE + 1] = {0};
            int result_size = sizeof(str_result);
            FT tmp_result;

            ret = ChangePrecision(n_decimal_places, tmp_result, str_result, &result_size);
            if (CA_RET_GENERAL_FAILURE != ret)
            {
                target = tmp_result;
                *result_in_str = str_result;
            }
        }
        else
            ret = ChangePrecision(n_decimal_places, target, NULL, NULL);

        return ret;
    }

    int ChangePrecision(const int n_decimal_places)
    {
        FT tmp_value = m_value;
        std::string tmp_str((NULL != m_string) ? (*m_string) : NULL);
        int ret = ChangePrecision(n_decimal_places, tmp_value, m_string);

        if (ret < 0)
        {
            if (NULL != m_string)
                *m_string = tmp_str;

            return ret;
        }

        m_decimal_place_count = n_decimal_places;
        m_value = tmp_value;

        return CA_RET_OK;
    }

    static CA_REENTRANT bool AbsolutelyEqual(const FT compared, const FT base)
    {
        THROW_EXCEPTION_IF_NOT_FLOATING(FT);
        /*
         * TODO: Which one is faster for comparation between two floating point numbers?
         */
        return (compared == base);
        //return (0 == memcmp(&compared, &base, sizeof(FT)));
    }

    /*
     * LessApproximate() functions check if all conditions below are true:
     *     (1) @compared(or @this) is not greater than @base
     *     (2) @compared(or @this) is approximately equal to @base within some certain deviation
     */

    static CA_REENTRANT bool LessApproximate(const FT compared, const FT base, const FT absolute_deviation)
    {
        THROW_EXCEPTION_IF_NOT_FLOATING(FT);

        if (compared > base)
            return false;

        FT difference = base - compared;

        if (typeid(float) == typeid(FT))
            return difference <= fabsf(static_cast<float>(absolute_deviation));
        else if (typeid(double) == typeid(FT))
            return difference <= fabs(static_cast<double>(absolute_deviation));
        else
            return difference <= fabsl(static_cast<long double>(absolute_deviation));
    }

    static CA_REENTRANT bool LessApproximate(const FT compared, const FT base, const int n_decimal_places)
    {
        THROW_EXCEPTION_IF_NOT_FLOATING(FT);

        if (compared > base || DECIMAL_PLACE_COUNT_OUT_OF_RANGE(FT, n_decimal_places))
            return false;

        char deviation_str[CHAR_COUNT_OF_MAX_VALUE + 1] = {0};
        FT deviation_value = 0;

        /*
         * Determines the deviation in string format.
         * For efficiency, we assign the elements of array deviation_str directly
         * instead of using string operation functions such as snprintf(), strncpy(), etc.
         */

        if (0 == n_decimal_places)
            deviation_str[0] = '1';
        else // deviation_str will be 0.0000...1
        {
            deviation_str[0] = '0';
            deviation_str[1] = '.';
            for (int i = 0; i < n_decimal_places - 1; ++i)
            {
                deviation_str[i + 2] = '0'; // NOTE: Not '\0'!!!
            }
            deviation_str[1 + n_decimal_places] = '1';
        }

        if (typeid(float) == typeid(FT))
            deviation_value = strtof(deviation_str, NULL);
        else if (typeid(double) == typeid(FT))
            deviation_value = strtod(deviation_str, NULL);
        else
            deviation_value = strtold(deviation_str, NULL);

        return LessApproximate(compared, base, deviation_value);
    }

    bool LessApproximate(const Floating& base) const
    {
        int n_decimal_places = (this->m_decimal_place_count >= base.m_decimal_place_count)
            ? this->m_decimal_place_count : base.m_decimal_place_count;

        return LessApproximate(this->m_value, base.m_value, n_decimal_places);
    }

    bool LessApproximate(const FT base) const
    {
        return LessApproximate(m_value, base, m_decimal_place_count);
    }

    bool LessApproximate(const FT base, const int n_decimal_places) const
    {
        return LessApproximate(m_value, base, n_decimal_places);
    }

    bool LessApproximate(const FT base, const FT absolute_deviation) const
    {
        return LessApproximate(m_value, base, absolute_deviation);
    }

    /*
     * GreaterApproximate() functions check if all conditions below are true:
     *     (1) @compared(or @this) is not less than @base
     *     (2) @compared(or @this) is approximately equal to @base within some certain deviation
     */

    static CA_REENTRANT bool GreaterApproximate(const FT compared, const FT base, const FT absolute_deviation)
    {
        THROW_EXCEPTION_IF_NOT_FLOATING(FT);

        if (compared < base)
            return false;

        return LessApproximate(base, compared, absolute_deviation);
    }

    static CA_REENTRANT bool GreaterApproximate(const FT compared, const FT base, const int n_decimal_places)
    {
        THROW_EXCEPTION_IF_NOT_FLOATING(FT);

        if (compared < base)
            return false;

        return LessApproximate(base, compared, n_decimal_places);
    }

    bool GreaterApproximate(const Floating& base) const
    {
        int n_decimal_places = (this->m_decimal_place_count >= base.m_decimal_place_count)
            ? this->m_decimal_place_count : base.m_decimal_place_count;

        return GreaterApproximate(this->m_value, base.m_value, n_decimal_places);
    }

    bool GreaterApproximate(const FT base) const
    {
        return GreaterApproximate(m_value, base, m_decimal_place_count);
    }

    bool GreaterApproximate(const FT base, const int n_decimal_places) const
    {
        return GreaterApproximate(m_value, base, n_decimal_places);
    }

    bool GreaterApproximate(const FT base, const FT absolute_deviation) const
    {
        return GreaterApproximate(m_value, base, absolute_deviation);
    }

    static CA_REENTRANT bool Approximate(const FT compared, const FT base, const FT absolute_deviation)
    {
        THROW_EXCEPTION_IF_NOT_FLOATING(FT);

        FT _deviation = absolute_deviation / 2;

        return LessApproximate(compared, base, _deviation) || GreaterApproximate(compared, base, _deviation);
    }

    static CA_REENTRANT bool Approximate(const FT compared, const FT base, const int n_decimal_places)
    {
        THROW_EXCEPTION_IF_NOT_FLOATING(FT);

        if (DECIMAL_PLACE_COUNT_OUT_OF_RANGE(FT, n_decimal_places))
            return false;

        char result_compared[CHAR_COUNT_OF_MAX_VALUE + 1] = {0};
        int size_compared = sizeof(result_compared);
        char result_base[CHAR_COUNT_OF_MAX_VALUE + 1] = {0};
        int size_base = sizeof(result_base);

        ToString(compared, n_decimal_places, result_compared, size_compared);
        ToString(base, n_decimal_places, result_base, size_base);

        return (0 == memcmp(result_compared, result_base, strlen(result_base)));
    }

    bool Approximate(const Floating& base) const
    {
        int n_decimal_places = (this->m_decimal_place_count >= base.m_decimal_place_count)
            ? this->m_decimal_place_count : base.m_decimal_place_count;

        return Approximate(this->m_value, base.m_value, n_decimal_places);
    }

    bool Approximate(const FT base) const
    {
        return Approximate(m_value, base, m_decimal_place_count);
    }

    bool Approximate(const FT base, const int n_decimal_places) const
    {
        return Approximate(m_value, base, n_decimal_places);
    }

    bool Approximate(const FT base, const FT absolute_deviation) const
    {
        return Approximate(m_value, base, absolute_deviation);
    }

/* ===================================
 * attributes:
 * =================================== */
public:
    DEFINE_CLASS_NAME_FUNC()

    FT value(void) const
    {
        return m_value;
    }

    int decimal_place_count(void) const
    {
        return m_decimal_place_count;
    }

    static CA_REENTRANT const char* type(void)/* const*/
    {
        if (typeid(float) == typeid(FT))
            return FLOAT_TYPE_NAME;
        else if (typeid(double) == typeid(FT))
            return DOUBLE_TYPE_NAME;
        else if (typeid(long double) == typeid(FT))
            return LONG_DOUBLE_TYPE_NAME;
        else
            return typeid(FT).name();
    }

    static CA_REENTRANT int type_size(void)/* const*/
    {
        return sizeof(FT);
    }

    friend class FloatingAccessor<FT>;

/* ===================================
 * status:
 * =================================== */
public:

/* ===================================
 * operators:
 * =================================== */
public:
    bool operator==(const Floating& compared) const
    {
        return (this->m_decimal_place_count == compared.m_decimal_place_count)
            && (this->m_value == compared.m_value);
    }

    bool operator==(const FT compared) const
    {
        FT tmp_value = compared;

        //ChangePrecision(m_decimal_place_count, tmp_value);// should not do this!

        return m_value == tmp_value;
    }

    bool operator!=(const Floating& compared) const
    {
        return !(*this == compared);
    }

    bool operator!=(const FT compared) const
    {
        return !(this->m_value == compared);
    }

    bool operator<(const Floating& compared) const
    {
        return this->m_value < compared.m_value;
    }

    bool operator<=(const FT compared) const
    {
        return this->m_value <= compared;
    }

    bool operator>(const Floating& compared) const
    {
        return this->m_value > compared.m_value;
    }

    bool operator>=(const FT compared) const
    {
        return this->m_value >= compared;
    }

    // TODO: +, +=, -, -=, *, *=, /, /=. And xx= should modify m_string.

/* ===================================
 * private methods:
 * =================================== */
protected:

/* ===================================
 * data:
 * =================================== */
public:
    static const int DEFAULT_DECIMAL_PLACE_COUNT;
    static const int MAX_DECIMAL_PLACE_COUNT;
    static const int CHAR_COUNT_OF_MAX_VALUE;
    static const FT MAX_VALUE;
    static const FT MIN_VALUE;
    static const FT EPSILON;

protected:
    DECLARE_CLASS_NAME_VAR();
    FT m_value;
    int m_decimal_place_count;
    std::string *m_string;
}; // template class Floating

typedef Floating<float> Float;
typedef Floating<double> Double;
typedef Floating<long double> LongDouble;

CA_LIB_NAMESPACE_END

#endif /* __CPP_ASSISTANT_FLOATING_CALCULATION_H__ */