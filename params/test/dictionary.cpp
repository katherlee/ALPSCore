/*
 * Copyright (C) 1998-2017 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

/** @file dictionary.cpp
    
    @brief Tests the behaviour of dictionary
*/

#include <alps/params_new.hpp>
#include <gtest/gtest.h>

// #include <boost/utility.hpp> // for enable_if<>
#include <boost/type_traits/is_same.hpp>
using boost::is_same;

#include <boost/integer_traits.hpp>
using boost::integer_traits;

#include "./dict_values_test.hpp"

namespace ap=alps::params_new_ns;
using ap::dictionary;
namespace de=ap::exception;
namespace aptest=ap::testing;

class DictionaryTest0 : public ::testing::Test {
    protected:
    dictionary dict_;
    const dictionary& cdict_;

    public:
    DictionaryTest0(): dict_(), cdict_(dict_) {}
};

TEST_F(DictionaryTest0, ctor) {
    EXPECT_TRUE(cdict_.empty());
    EXPECT_EQ(0ul, cdict_.size());
}

TEST_F(DictionaryTest0, access) {
    EXPECT_TRUE(dict_["name"].empty());
    EXPECT_FALSE(cdict_.empty());
    EXPECT_EQ(1ul, cdict_.size());
    EXPECT_TRUE(dict_["name"].empty());
    
    EXPECT_TRUE(dict_["name2"].empty());
    EXPECT_FALSE(cdict_.empty());
    EXPECT_EQ(2ul, cdict_.size());
    EXPECT_TRUE(dict_["name2"].empty());

    EXPECT_TRUE(dict_["name"].empty());
    EXPECT_FALSE(cdict_.empty());
    EXPECT_EQ(2ul, cdict_.size());

    EXPECT_TRUE(dict_["name1"].empty());
    EXPECT_FALSE(cdict_.empty());
    EXPECT_EQ(3ul, cdict_.size());
}

TEST_F(DictionaryTest0, constAccess) {
    EXPECT_THROW(cdict_["name"], de::uninitialized_value);
    EXPECT_TRUE(cdict_.empty());
    EXPECT_EQ(0ul, cdict_.size());

    dict_["name"];
    EXPECT_TRUE(cdict_["name"].empty());
}

TEST_F(DictionaryTest0, charAssign) {
    dict_["name"]='x';
    EXPECT_EQ(1ul, cdict_.size());
}

TEST_F(DictionaryTest0, charGetter) {
    char expected='x';
    dict_["name"]=expected;
    char actual=cdict_["name"];
    EXPECT_EQ(expected, actual);
}

TEST_F(DictionaryTest0, charAsIntGetter) {
    char expected='x';
    dict_["name"]=expected;
    int actual=cdict_["name"];
    EXPECT_EQ(expected, actual);
}


class DictionaryTestIntegrals : public ::testing::Test {
    protected:
    dictionary dict_;
    const dictionary& cdict_;

    static const long neg_long= integer_traits<long>::const_min+5;
    static const int  neg_int=  integer_traits<int>::const_min+7;
    static const int  pos_int=  integer_traits<int>::const_max-7;
    static const long pos_long= integer_traits<long>::const_max-5;
    
    static const unsigned int pos_uint=   integer_traits<unsigned int>::const_max-9;
    static const unsigned long pos_ulong= integer_traits<unsigned long>::const_max-10;

    // FIXME: consider this too
    // static const bool long_is_int=(sizeof(long)==sizeof(int));

    public:
    DictionaryTestIntegrals(): dict_(), cdict_(dict_) {
        // suffix "_is" means "integer size" (that is, value small enough to fit into signed integer)
        // suffix "_uis" means "unsigned integer size"
        // suffix "_ls" means "long size"
        
        dict_["neg_long"]= +neg_long;

        dict_["neg_int"]= +neg_int;
        dict_["neg_long_is"]= static_cast<long>(+neg_int);
        
        dict_["pos_int"]= +pos_int;
        dict_["uint_is"]= static_cast<unsigned int>(+pos_int);
        dict_["pos_long_is"]= static_cast<long>(+pos_int);
        dict_["ulong_is"]= static_cast<unsigned long>(+pos_int);
        
        dict_["uint"]= +pos_uint;
        dict_["pos_long_uis"]= static_cast<long>(+pos_uint);
        dict_["ulong_uis"]= static_cast<unsigned long>(+pos_uint);

        dict_["pos_long"]= +pos_long;
        dict_["ulong_ls"]= static_cast<unsigned long>(+pos_long);

        dict_["ulong"]= +pos_ulong;
    }
};

TEST_F(DictionaryTestIntegrals, toUlong) {
    unsigned long actual=1;

    EXPECT_THROW(actual=cdict_["neg_long"], de::value_mismatch);
    EXPECT_EQ(1u,actual);

    EXPECT_THROW(actual=cdict_["neg_int"], de::value_mismatch);
    EXPECT_EQ(1u,actual);

    EXPECT_THROW(actual=cdict_["neg_long_is"], de::value_mismatch);
    EXPECT_EQ(1u,actual);

    actual=cdict_["pos_int"];
    EXPECT_EQ(0u, actual-pos_int);

    actual=cdict_["uint_is"];
    EXPECT_EQ(0u, actual-pos_int);

    actual=cdict_["pos_long_is"];
    EXPECT_EQ(0u, actual-pos_int);

    actual=cdict_["ulong_is"];
    EXPECT_EQ(0u, actual-pos_int);
        
    actual=cdict_["uint"];
    EXPECT_EQ(0u, actual-pos_uint);

    actual=cdict_["pos_long_uis"];
    EXPECT_EQ(0u, actual-pos_uint);

    actual=cdict_["ulong_uis"];
    EXPECT_EQ(0u, actual-pos_uint);

    actual=cdict_["pos_long"];
    EXPECT_EQ(0u, actual-pos_long);

    actual=cdict_["ulong_ls"];
    EXPECT_EQ(0u, actual-pos_long);

    actual=cdict_["ulong"];
    EXPECT_EQ(0u, actual-pos_ulong);
}

TEST_F(DictionaryTestIntegrals, toLong) {
    long actual=1;

    EXPECT_THROW(actual=cdict_["neg_long"], de::value_mismatch);
    EXPECT_EQ(1,actual);

    EXPECT_THROW(actual=cdict_["neg_int"], de::value_mismatch);
    EXPECT_EQ(1,actual);

    EXPECT_THROW(actual=cdict_["neg_long_is"], de::value_mismatch);
    EXPECT_EQ(1,actual);

    EXPECT_THROW(actual=cdict_["ulong"], de::value_mismatch);
    EXPECT_EQ(1,actual);

    actual=cdict_["pos_int"];
    EXPECT_EQ(0u, actual-pos_int);

    actual=cdict_["uint_is"];
    EXPECT_EQ(0u, actual-pos_int);

    actual=cdict_["pos_long_is"];
    EXPECT_EQ(0u, actual-pos_int);

    actual=cdict_["ulong_is"];
    EXPECT_EQ(0u, actual-pos_int);
        
    actual=cdict_["uint"];
    EXPECT_EQ(0u, actual-pos_uint);

    actual=cdict_["pos_long_uis"];
    EXPECT_EQ(0u, actual-pos_uint);

    actual=cdict_["ulong_uis"];
    EXPECT_EQ(0u, actual-pos_uint);

    actual=cdict_["pos_long"];
    EXPECT_EQ(0u, actual-pos_long);

    actual=cdict_["ulong_ls"];
    EXPECT_EQ(0u, actual-pos_long);
}


TEST_F(DictionaryTestIntegrals, toUint) {
    unsigned int actual=1;

    EXPECT_THROW(actual=cdict_["neg_long"], de::value_mismatch);
    EXPECT_EQ(1u,actual);

    EXPECT_THROW(actual=cdict_["neg_int"], de::value_mismatch);
    EXPECT_EQ(1u,actual);

    EXPECT_THROW(actual=cdict_["neg_long_is"], de::value_mismatch);
    EXPECT_EQ(1u,actual);

    EXPECT_THROW(actual=cdict_["pos_long"], de::value_mismatch);
    EXPECT_EQ(1u,actual);

    EXPECT_THROW(actual=cdict_["ulong_ls"], de::value_mismatch);
    EXPECT_EQ(1u,actual);

    EXPECT_THROW(actual=cdict_["ulong"], de::value_mismatch);
    EXPECT_EQ(1u,actual);


    actual=cdict_["pos_int"];
    EXPECT_EQ(0u, actual-pos_int);

    actual=cdict_["uint_is"];
    EXPECT_EQ(0u, actual-pos_int);

    actual=cdict_["pos_long_is"];
    EXPECT_EQ(0u, actual-pos_int);

    actual=cdict_["ulong_is"];
    EXPECT_EQ(0u, actual-pos_int);
        
    actual=cdict_["uint"];
    EXPECT_EQ(0u, actual-pos_uint);

    actual=cdict_["pos_long_uis"];
    EXPECT_EQ(0u, actual-pos_uint);

    actual=cdict_["ulong_uis"];
    EXPECT_EQ(0u, actual-pos_uint);
}

TEST_F(DictionaryTestIntegrals, toInt) {
    int actual=1;

    EXPECT_THROW(actual=cdict_["neg_long"], de::value_mismatch);
    EXPECT_EQ(1,actual);

    EXPECT_THROW(actual=cdict_["uint"], de::value_mismatch);
    EXPECT_EQ(1,actual);

    EXPECT_THROW(actual=cdict_["pos_long_uis"], de::value_mismatch);
    EXPECT_EQ(1,actual);

    EXPECT_THROW(actual=cdict_["ulong_uis"], de::value_mismatch);
    EXPECT_EQ(1,actual);

    EXPECT_THROW(actual=cdict_["pos_long"], de::value_mismatch);
    EXPECT_EQ(1,actual);

    EXPECT_THROW(actual=cdict_["ulong_ls"], de::value_mismatch);
    EXPECT_EQ(1,actual);

    EXPECT_THROW(actual=cdict_["ulong"], de::value_mismatch);
    EXPECT_EQ(1,actual);


    actual=cdict_["neg_int"];
    EXPECT_EQ(0, actual-neg_int);

    actual=cdict_["neg_long_is"];
    EXPECT_EQ(0, actual-neg_int);
        
    actual=cdict_["pos_int"];
    EXPECT_EQ(0, actual-pos_int);

    actual=cdict_["uint_is"];
    EXPECT_EQ(0, actual-pos_int);

    actual=cdict_["pos_long_is"];
    EXPECT_EQ(0, actual-pos_int);

    actual=cdict_["ulong_is"];
    EXPECT_EQ(0, actual-pos_int);
}


TEST_F(DictionaryTestIntegrals, toFloat) {
    float actual=1.00;
    float eps=1E-3;

    actual=cdict_["neg_long"];
    EXPECT_NEAR(+neg_long, actual, eps);
        
    actual=cdict_["neg_int"];
    EXPECT_NEAR(+neg_int, actual, eps);

    actual=cdict_["neg_long_is"];
    EXPECT_NEAR(+neg_int, actual, eps);
        
    actual=cdict_["pos_int"];
    EXPECT_NEAR(+pos_int, actual, eps);

    actual=cdict_["uint_is"];
    EXPECT_NEAR(+pos_int, actual, eps);

    actual=cdict_["pos_long_is"];
    EXPECT_NEAR(+pos_int, actual, eps);

    actual=cdict_["ulong_is"];
    EXPECT_NEAR(+pos_int, actual, eps);
        
    actual=cdict_["uint"];
    EXPECT_NEAR(+pos_uint, actual, eps);

    actual=cdict_["pos_long_uis"];
    EXPECT_NEAR(+pos_uint, actual, eps);

    actual=cdict_["ulong_uis"];
    EXPECT_NEAR(+pos_uint, actual, eps);

    actual=cdict_["pos_long"];
    EXPECT_NEAR(+pos_long, actual, eps);

    actual=cdict_["ulong_ls"];
    EXPECT_NEAR(+pos_long, actual, eps);

    actual=cdict_["ulong"];
    EXPECT_NEAR(+pos_ulong, actual, eps);
}

TEST_F(DictionaryTestIntegrals, toDouble) {
    double actual=1.00;
    double eps=1E-3;

    actual=cdict_["neg_long"];
    EXPECT_NEAR(+neg_long, actual, eps);
        
    actual=cdict_["neg_int"];
    EXPECT_NEAR(+neg_int, actual, eps);

    actual=cdict_["neg_long_is"];
    EXPECT_NEAR(+neg_int, actual, eps);
        
    actual=cdict_["pos_int"];
    EXPECT_NEAR(+pos_int, actual, eps);

    actual=cdict_["uint_is"];
    EXPECT_NEAR(+pos_int, actual, eps);

    actual=cdict_["pos_long_is"];
    EXPECT_NEAR(+pos_int, actual, eps);

    actual=cdict_["ulong_is"];
    EXPECT_NEAR(+pos_int, actual, eps);
        
    actual=cdict_["uint"];
    EXPECT_NEAR(+pos_uint, actual, eps);

    actual=cdict_["pos_long_uis"];
    EXPECT_NEAR(+pos_uint, actual, eps);

    actual=cdict_["ulong_uis"];
    EXPECT_NEAR(+pos_uint, actual, eps);

    actual=cdict_["pos_long"];
    EXPECT_NEAR(+pos_long, actual, eps);

    actual=cdict_["ulong_ls"];
    EXPECT_NEAR(+pos_long, actual, eps);

    actual=cdict_["ulong"];
    EXPECT_NEAR(+pos_ulong, actual, eps);
}



/// Helper metafunctions / metapredicates
template <bool V> struct yes_no {};
template <> struct yes_no<true> { typedef bool yes; static const bool value=true; };
template <> struct yes_no<false> { typedef bool no; static const bool value=false; };

template <typename T> struct is_vector : public yes_no<false> {};

template <typename T>
struct is_vector< std::vector<T> > : public yes_no<true> {};

template <typename T>
struct is_string : public yes_no<is_same<T,std::string>::value> {};

template <typename T>
struct is_string_or_vec : public yes_no<is_string<T>::value || is_vector<T>::value> {};

template <typename T>
struct is_bool : public yes_no<is_same<T,bool>::value> {};


// Parametrized on the value type stored in the dictionary
template <typename T>
class DictionaryTest : public ::testing::Test {
    protected:
    dictionary dict_;
    const dictionary& cdict_;
    
    typedef aptest::data_trait<T> trait;
    // typedef typename trait::larger_type larger_type;
    // typedef typename trait::smaller_type smaller_type;
    // typedef typename trait::incompatible_type incompatible_type;
    // static const bool has_larger_type=trait::has_larger_type;
    // static const bool has_smaller_type=trait::has_smaller_type;

    public:
    DictionaryTest(): dict_(), cdict_(dict_) {
        dict_["none"];
        const T expected=trait::get(false);
        dict_["name"]=expected;
    }

    void afterCtor() {
        EXPECT_TRUE(cdict_["none"].empty());
        EXPECT_FALSE(cdict_["name"].empty());
    }


    void assignRetval() {
        const T expected=trait::get(true);
        const T actual=(dict_["name"]=expected);
        EXPECT_EQ(expected, actual);
    }

    // assignment is done in ctor; check if it worked
    template <typename X>
    void explicitAssignSameType(typename is_string_or_vec<X>::no =true) {
        const T expected=trait::get(false);
        const T actual=static_cast<T>(cdict_["name"]);
        EXPECT_EQ(expected, actual) << "Explicit conversion";
    }
    
    // assignment is done in ctor; check if it worked
    template <typename X>
    void explicitAssignSameType(typename is_string_or_vec<X>::yes =true) {
        // The following does not compile, due to ambiguous string/vector ctor call.
        /*
          const T expected=trait::get(false);
          const T actual=static_cast<T>(cdict_["name"]);
          EXPECT_EQ(expected, actual) << "Explicit conversion";
        */
    }
    

    void implicitAssignSameType() {
        const T expected=trait::get(false);
        const T actual=cdict_["name"];
        EXPECT_EQ(expected, actual) << "Implicit conversion";
    }
    

    void asSameType() {
        const T expected=trait::get(false);
        const T actual=cdict_["name"].template as<T>();
        EXPECT_EQ(expected, actual) << "Shortcut conversion";
    }
    

    void reassignSameType() {
        const T expected=trait::get(true);
        dict_["name"]=expected;
        const T actual=cdict_["name"];
        EXPECT_EQ(expected, actual);
    }

    template <typename X>
    void assignFromNone(typename is_string<X>::no =true) {
        const T expected=trait::get(false);
        T actual=expected;
        ASSERT_THROW(actual=cdict_["none"], de::uninitialized_value);
        EXPECT_EQ(expected,actual);
    }

    template <typename X>
    void assignFromNone(typename is_string<X>::yes =true) {
        // The following does not compile due to ambiguous string assignment
        /*
          const T expected=trait::get(false);
          T actual=expected;
          ASSERT_THROW(actual=cdict_["none"], de::uninitialized_value);
          EXPECT_EQ(expected,actual);
        */
    }

    void convertFromNoneExplicit() {
        const T expected=trait::get(false);
        T actual=expected;
        ASSERT_THROW(actual=cdict_["none"].template as<T>(), de::uninitialized_value);
        EXPECT_EQ(expected,actual);
    }
    
    void setToNone() {
        dict_["name"].clear();
        EXPECT_TRUE(cdict_["name"].empty());
    }

    // Nothing (except bool) can be converted to bool
    template <typename X>
    void toBool(typename is_bool<X>::no =true) {
        bool actual=true;
        EXPECT_THROW(actual=cdict_["name"], de::type_mismatch);
        EXPECT_TRUE(actual);
    }

    template <typename X>
    void toBool(typename is_bool<X>::yes =true) {}

};

typedef ::testing::Types<
    bool
    ,
    int
    ,
    long
    ,
    unsigned long int
    ,
    float
    ,
    double
    ,
    std::string
    ,
    std::vector<bool>
    ,
    std::vector<int>
    ,
    std::vector<long>
    ,
    std::vector<unsigned long int>
    ,
    std::vector<double>
    ,
    std::vector<std::string>
    
    > my_all_types;

TYPED_TEST_CASE(DictionaryTest, my_all_types);

#define MAKE_TEST(_name_) TYPED_TEST(DictionaryTest,_name_) { this->_name_(); }
#define MAKE_TEST_TMPL(_name_) TYPED_TEST(DictionaryTest,_name_) { this->template _name_<TypeParam>(); }

MAKE_TEST(afterCtor);
MAKE_TEST(assignRetval);
MAKE_TEST_TMPL(explicitAssignSameType);
MAKE_TEST(implicitAssignSameType);
MAKE_TEST(asSameType);
MAKE_TEST(reassignSameType);
MAKE_TEST_TMPL(assignFromNone);
MAKE_TEST(convertFromNoneExplicit);
MAKE_TEST(setToNone);
MAKE_TEST_TMPL(toBool);

#undef MAKE_TEST

// Parametrized on the value type stored in the dictionary
template <typename T>
class DictionaryTestIntegral2 : public ::testing::Test {
    protected:
    dictionary dict_;
    const dictionary& cdict_;
    
    typedef aptest::data_trait<T> trait;

    public:
    DictionaryTestIntegral2(): dict_(), cdict_(dict_) {
        dict_["true"]=true;
        dict_["false"]=false;
    }

    // Bool can be converted to any integral type
    void boolToIntegral() {
        {
            T expected=true;
            T actual=cdict_["true"];
            EXPECT_EQ(expected, actual) << "true value test";
        }
        {
            T expected=false;
            T actual=cdict_["false"];
            EXPECT_EQ(expected, actual) << "false value test";
        }
    }

    // Floating point cannot be converted to an integral type
    void fpToIntegral() {
        T expected=trait::get(true);
        dict_["integral"]=expected;

        {
            float f=1.25;
            EXPECT_THROW(dict_["integral"]=f, de::type_mismatch);
            T actual=cdict_["integral"];
            EXPECT_EQ(expected, actual);
        }
        {
            double d=3.75;
            EXPECT_THROW(dict_["integral"]=d, de::type_mismatch);
            T actual=cdict_["integral"];
            EXPECT_EQ(expected, actual);
        }
    }
};

typedef ::testing::Types<
    // char
    // ,
    int
    ,
    unsigned int
    ,
    long
    ,
    unsigned long
    > my_integral_types;

TYPED_TEST_CASE(DictionaryTestIntegral2, my_integral_types);

#define MAKE_TEST(_name_) TYPED_TEST(DictionaryTestIntegral2,_name_) { this->_name_(); }

MAKE_TEST(boolToIntegral);
MAKE_TEST(fpToIntegral);

#undef MAKE_TEST


// Parametrized on the value type stored in the dictionary
template <typename T>
class DictionaryTestNonnumeric : public ::testing::Test {
    protected:
    dictionary dict_;
    const dictionary& cdict_;
    
    typedef aptest::data_trait<T> trait;

    public:
    DictionaryTestNonnumeric(): dict_(), cdict_(dict_) {
    }

    // Nothing (numeric) can be converted to a non-integral type
    template <typename X>
    void ToNonIntegral() {
        dict_["integral"]=aptest::data_trait<X>::get(true);
        T expected=trait::get(false);
        T actual=expected;
        EXPECT_THROW(actual=cdict_["integral"].template as<T>(), de::type_mismatch);
        EXPECT_EQ(expected, actual);
    }

    // Nothing can be converted from a non-integral type
    template <typename X>
    void FromNonIntegral() {
        X expected=aptest::data_trait<X>::get(true);
        X actual=expected;
        dict_["nonint"]=trait::get(false);
        EXPECT_THROW(actual=cdict_["nonint"], de::type_mismatch);
        EXPECT_EQ(expected, actual);
    }

};



typedef ::testing::Types<
    std::string
    ,
    std::vector<bool>
    ,
    std::vector<int>
    ,
    std::vector<long>
    ,
    std::vector<unsigned long int>
    ,
    std::vector<double>
    ,
    std::vector<std::string>
    > my_nonintegral_types;

TYPED_TEST_CASE(DictionaryTestNonnumeric, my_nonintegral_types);

#define MAKE_TEST(_name_) TYPED_TEST(DictionaryTestNonnumeric,_name_) { this->_name_(); }
#define MAKE_TEST_X(_name_,_type_) TYPED_TEST(DictionaryTestNonnumeric,_type_ ## _name_) { this->template _name_<_type_>(); }

MAKE_TEST_X(ToNonIntegral, bool);
MAKE_TEST_X(ToNonIntegral, int);
MAKE_TEST_X(ToNonIntegral, long);
MAKE_TEST_X(ToNonIntegral, double);

MAKE_TEST_X(FromNonIntegral, bool);
MAKE_TEST_X(FromNonIntegral, int);
MAKE_TEST_X(FromNonIntegral, long);
MAKE_TEST_X(FromNonIntegral, double);

#undef MAKE_TEST
