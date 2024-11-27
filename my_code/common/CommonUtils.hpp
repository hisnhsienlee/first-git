/*
 *  Copyright (c) 2017-2020 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

/**
 * @file       CommonUtils.hpp
 * @brief      Common utility class for miscelaneous helper functions
 * @desc       utility functions that don't belong anywhere else
 */

#ifndef COMMONUTILS_HPP
#define COMMONUTILS_HPP

#include <type_traits>
#include <memory>
#include <sstream>
#include <vector>
#include <algorithm>

namespace telux {

namespace common {

/**
 * This function is used to obtain the underlying data type and value for an instance of
 * an enum class. It's cleaner than simply performing a cast.
 *
 * @param [in] t  -  Instance of enum class T
 *
 * @returns The value of t represented in the underlying data type used to internally
 *          represent instances of enum class T.
 */
template<typename T>
auto getUnderlyingValue(T t) -> typename std::underlying_type<T>::type {
    return static_cast<typename std::underlying_type<T>::type>(t);
}

/**
 * This is a utility class to enable shared_from_this() in case when both
 * base class as well as child class wants to use shared_from_this separately
 *
 * For enabling shared_from_this use enable_inheritable_shared_from_this<BaseClass>
 * base class function    - shared_from_this().
 * Use the below function in derived class instead of shared_from_this()
 * derived class function - downcasted_shared_from_this<DerivedClass>().
 *
 */
class SharedFromThis : public std::enable_shared_from_this
        <SharedFromThis> {
public:
    virtual ~SharedFromThis() {
    }
};

template <class T>
class enable_inheritable_shared_from_this : virtual public SharedFromThis {
public:
    std::shared_ptr<T> shared_from_this () {
        return std::dynamic_pointer_cast<T>(SharedFromThis::shared_from_this());
    }

    template <class Down>
    std::shared_ptr<Down> downcasted_shared_from_this() {
        return std::dynamic_pointer_cast<Down>(SharedFromThis::shared_from_this());
    }
};

/**
 * This class encapsulates some miscellaneous utility functions
 */
class CommonUtils {
public:
   /**
    * Dumps a binary buffer to a data stream
    *
    * @param [in] data   - buffer holding data
    * @param [in] length - number of bytes to dump
    */
   static void hexDump(const char *data, int length);

    /**
    * compare two string ignore case and return true if equal.
    *
    * @param [in] str1 - input first string
    * @param [in] str2 - input second string
    */
   static bool isEqualStrings(std::string str1, std::string str2);

    /**
    * split the given string using delimiter criteria.
    *
    * @param [in] str       - input string
    * @param [in] delimiter - input delimiter to split string
    *
    * @returns Vector of string, splited using delimiter.
    */
   static std::vector<std::string> splitString(const std::string &str, char delimiter);
};  // End of class Utils

}  // End of namespace tel

}  // End of namespace telux

#endif
