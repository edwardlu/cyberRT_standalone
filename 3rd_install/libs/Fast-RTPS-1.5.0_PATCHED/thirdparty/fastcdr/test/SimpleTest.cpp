// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <fastcdr/Cdr.h>
#include <fastcdr/FastCdr.h>
#include <fastcdr/exceptions/Exception.h>

#include <stdio.h>
#include <limits>
#include <iostream>

#include <gtest/gtest.h>

using namespace eprosima::fastcdr;
using namespace ::exception;

#define BUFFER_LENGTH 2000

static const uint8_t octet_t = 32;
static const char char_t =  'Z';
static const uint16_t ushort_t = 65500;
static const int16_t short_t = -32700;
static const uint32_t ulong_t = 4294967200;
static const int32_t long_t = -2147483600;
static const uint64_t ulonglong_t = 18446744073709551600u;
static const int64_t longlong_t = -9223372036800;
static const float float_tt = std::numeric_limits<float>::min();
static const double double_tt = std::numeric_limits<double>::min();
static const bool bool_t = true;
static const std::string string_t = "Hola a todos, esto es un test";
static const std::string emptystring_t = "";
static const std::array<uint8_t, 5> octet_array_t = {{1, 2, 3, 4, 5}};
static const uint8_t octet_array_2_t[5] = {5, 4, 3, 2, 1};
static const std::array<char, 5> char_array_t = {{'A', 'B', 'C', 'D', 'E'}};
static const char char_array_2_t[5] = {'E', 'D', 'C', 'B', 'A'};
static const std::array<uint16_t, 5> ushort_array_t = {{65500, 65501, 65502, 65503, 65504}};
static const uint16_t ushort_array_2_t[5] = {65504, 65503, 65502, 65501, 65500};
static const std::array<int16_t, 5> short_array_t = {{-32700, -32701, -32702, -32703, -32704}};
static const int16_t short_array_2_t[5] = {-32704, -32703, -32702, -32701, -32700};
static const std::array<uint32_t, 5> ulong_array_t = {{4294967200, 4294967201, 4294967202, 4294967203, 4294967204}};
static const uint32_t ulong_array_2_t[5] = {4294967204, 4294967203, 4294967202, 4294967201, 4294967200};
static const std::array<int32_t, 5> long_array_t = {{-2147483600, -2147483601, -2147483602, -2147483603, -2147483604}};
static const int32_t long_array_2_t[5] = {-2147483604, -2147483603, -2147483602, -2147483601, -2147483600};
static const std::array<uint64_t, 5> ulonglong_array_t = {{18446744073709551600u, 18446744073709551601u, 18446744073709551602u, 18446744073709551603u, 18446744073709551604u}};
static const uint64_t ulonglong_array_2_t[5] = {18446744073709551604u, 18446744073709551603u, 18446744073709551602u, 18446744073709551601u, 18446744073709551600u};
static const std::array<int64_t, 5> longlong_array_t = {{-9223372036800, -9223372036801, -9223372036802, -9223372036803, -9223372036804}};
static const int64_t longlong_array_2_t[5] = {-9223372036804, -9223372036803, -9223372036802, -9223372036801, -9223372036800};
static const std::array<float, 5> float_array_t = {{float_tt, float_tt + 1, float_tt + 2, float_tt + 3, float_tt + 4}};
static const float float_array_2_t[5] = {float_tt + 4, float_tt + 3, float_tt + 2, float_tt + 1, float_tt};
static const std::array<double, 5> double_array_t = {{double_tt, double_tt + 1, double_tt + 2, double_tt + 3, double_tt + 4}};
static const double double_array_2_t[5] = {double_tt + 4, double_tt + 3, double_tt + 2, double_tt + 1, double_tt};
static const std::array<bool, 5> bool_array_t = {{true, false, true, false, true}};
static const bool bool_array_2_t[5] = {false, true, false, true, false};
static const std::array<std::string, 5> string_array_t = {{"HOLA", "ADIOS", "HELLO", "BYE", "GOODBYE"}};
static const std::string string_array_2_t[5] = {"HOLA", "ADIOS", "HELLO", "BYE", "GOODBYE"};
static const std::vector<uint8_t> octet_vector_t(octet_array_2_t, octet_array_2_t + sizeof(octet_array_2_t) / sizeof(uint8_t));
static const std::vector<char> char_vector_t(char_array_2_t, char_array_2_t + sizeof(char_array_2_t) / sizeof(char));
static const std::vector<uint16_t> ushort_vector_t(ushort_array_2_t, ushort_array_2_t + sizeof(ushort_array_2_t) / sizeof(uint16_t));
static const std::vector<int16_t> short_vector_t(short_array_2_t, short_array_2_t + sizeof(short_array_2_t) / sizeof(int16_t));
static const std::vector<uint32_t> ulong_vector_t(ulong_array_2_t, ulong_array_2_t + sizeof(ulong_array_2_t) / sizeof(uint32_t));
static const std::vector<int32_t> long_vector_t(long_array_2_t, long_array_2_t + sizeof(long_array_2_t) / sizeof(int32_t));
static const std::vector<uint64_t> ulonglong_vector_t(ulonglong_array_2_t, ulonglong_array_2_t + sizeof(ulonglong_array_2_t) / sizeof(uint64_t));
static const std::vector<int64_t> longlong_vector_t(longlong_array_2_t, longlong_array_2_t + sizeof(longlong_array_2_t) / sizeof(int64_t));
static const std::vector<float> float_vector_t(float_array_2_t, float_array_2_t + sizeof(float_array_2_t) / sizeof(float));
static const std::vector<double> double_vector_t(double_array_2_t, double_array_2_t + sizeof(double_array_2_t) / sizeof(double));
static const std::vector<bool> bool_vector_t(bool_array_2_t, bool_array_2_t + sizeof(bool_array_2_t) / sizeof(bool));
static const std::vector<std::string> string_vector_t(string_array_2_t, string_array_2_t + sizeof(string_array_2_t) / sizeof(std::string));
static const std::array<std::array<std::array<uint32_t, 3>, 2>, 2> triple_ulong_array_t = {{ {{ {{1, 2, 3}}, {{4, 5, 6}} }}, {{ {{7, 8, 9}}, {{10, 11, 12}} }} }};
// Added because error 336.
static const uint8_t octet_seq_t[5] = {5, 4, 3, 2, 1};
static const char char_seq_t[5] = {'E', 'D', 'C', 'B', 'A'};
static const uint16_t ushort_seq_t[5] = {65504, 65503, 65502, 65501, 65500};
static const int16_t short_seq_t[5] = {-32704, -32703, -32702, -32701, -32700};
static const uint32_t ulong_seq_t[5] = {4294967204, 4294967203, 4294967202, 4294967201, 4294967200};
static const int32_t long_seq_t[5] = {-2147483604, -2147483603, -2147483602, -2147483601, -2147483600};
static const uint64_t ulonglong_seq_t[5] = {18446744073709551604u, 18446744073709551603u, 18446744073709551602u, 18446744073709551601u, 18446744073709551600u};
static const int64_t longlong_seq_t[5] = {-9223372036804, -9223372036803, -9223372036802, -9223372036801, -9223372036800};
static const float float_seq_t[5] = {float_tt + 4, float_tt + 3, float_tt + 2, float_tt + 1, float_tt};
static const double double_seq_t[5] = {double_tt + 4, double_tt + 3, double_tt + 2, double_tt + 1, double_tt};
static const bool bool_seq_t[5] = {true, true, false, false, true};
static const std::string string_seq_t[5] = {"HELLO", "BYE", "GOODBYE", "HOLA", "ADIOS"};
// Added because error 337
static const char *c_string_t = "HOLA";

template<typename T>
void EXPECT_ARRAY_EQ(T *array1, const T *array2, size_t size)
{
    for(size_t count = 0; count < size; ++count)
    {
        EXPECT_EQ(array1[count], array2[count]);
    }
}

static void EXPECT_ARRAY_FLOAT_EQ(float *array1, const float *array2, size_t size)
{
    for(size_t count = 0; count < size; ++count)
    {
        EXPECT_FLOAT_EQ(array1[count], array2[count]);
    }
}

static void EXPECT_ARRAY_DOUBLE_EQ(double *array1, const double *array2, size_t size)
{
    for(size_t count = 0; count < size; ++count)
    {
        EXPECT_DOUBLE_EQ(array1[count], array2[count]);
    }
}

TEST(CDRTests, Octet)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << octet_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    uint8_t octet_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> octet_value;
    });

    EXPECT_EQ(octet_value, octet_t);
}

TEST(CDRTests, Char)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << char_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    char char_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> char_value;
    });

    EXPECT_EQ(char_value, char_t);
}

TEST(CDRTests, UnsignedShort)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << ushort_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    uint16_t ushort_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> ushort_value;
    });

    EXPECT_EQ(ushort_value, ushort_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << ushort_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> ushort_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, Short)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << short_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    int16_t short_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> short_value;
    });

    EXPECT_EQ(short_value, short_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << short_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> short_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, UnsignedLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << ulong_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    uint32_t ulong_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> ulong_value;
    });

    EXPECT_EQ(ulong_value, ulong_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << ulong_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> ulong_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, Long)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << long_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    int32_t long_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> long_value;
    });

    EXPECT_EQ(long_value, long_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << long_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> long_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, UnsignedLongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << ulonglong_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    uint64_t ulonglong_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> ulonglong_value;
    });

    EXPECT_EQ(ulonglong_value, ulonglong_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << ulonglong_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> ulonglong_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, LongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << longlong_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    int64_t longlong_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> longlong_value;
    });

    EXPECT_EQ(longlong_value, longlong_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << longlong_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> longlong_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, Float)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << float_tt;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    float float_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> float_value;
    });

    EXPECT_FLOAT_EQ(float_value, float_tt);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << float_tt;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> float_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, Double)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << double_tt;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    double double_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> double_value;
    });

    EXPECT_DOUBLE_EQ(double_value, double_tt);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << double_tt;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> double_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, Boolean)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << bool_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    bool bool_value = false;

    EXPECT_NO_THROW(
    {
        cdr_des >> bool_value;
    });

    EXPECT_EQ(bool_value, bool_t);
}

TEST(CDRTests, String)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << string_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::string string_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> string_value;
    });

    EXPECT_EQ(string_value, string_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << string_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> string_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, EmptyString)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << emptystring_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::string string_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> string_value;
    });

    EXPECT_EQ(string_value, emptystring_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << emptystring_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> string_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDArrayOctet)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << octet_array_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::array<uint8_t, 5> octet_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> octet_array_value;
    });

    EXPECT_EQ(octet_array_value, octet_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << octet_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> octet_array_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDArrayChar)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << char_array_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::array<char, 5> char_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> char_array_value;
    });

    EXPECT_EQ(char_array_value, char_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << char_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> char_array_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDArrayUnsignedShort)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << ushort_array_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::array<uint16_t, 5> ushort_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> ushort_array_value;
    });

    EXPECT_EQ(ushort_array_value, ushort_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << ushort_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> ushort_array_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDArrayShort)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << short_array_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::array<int16_t, 5> short_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> short_array_value;
    });

    EXPECT_EQ(short_array_value, short_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << short_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> short_array_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDArrayUnsignedLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << ulong_array_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::array<uint32_t, 5> ulong_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> ulong_array_value;
    });

    EXPECT_EQ(ulong_array_value, ulong_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << ulong_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> ulong_array_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDArrayLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << long_array_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::array<int32_t, 5> long_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> long_array_value;
    });

    EXPECT_EQ(long_array_value, long_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << long_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> long_array_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDArrayUnsignedLongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << ulonglong_array_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::array<uint64_t, 5> ulonglong_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> ulonglong_array_value;
    });

    EXPECT_EQ(ulonglong_array_value, ulonglong_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << ulonglong_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> ulonglong_array_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDArrayLongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << longlong_array_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::array<int64_t, 5> longlong_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> longlong_array_value;
    });

    EXPECT_EQ(longlong_array_value, longlong_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << longlong_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> longlong_array_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDArrayFloat)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << float_array_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::array<float, 5> float_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> float_array_value;
    });

    EXPECT_EQ(float_array_value, float_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << float_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> float_array_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDArrayDouble)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << double_array_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::array<double, 5> double_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> double_array_value;
    });

    EXPECT_EQ(double_array_value, double_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << double_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> double_array_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDArrayBoolean)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << bool_array_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::array<bool, 5> bool_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> bool_array_value;
    });

    EXPECT_EQ(bool_array_value, bool_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << bool_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> bool_array_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDArrayString)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << string_array_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::array<std::string, 5> string_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> string_array_value;
    });

    EXPECT_EQ(string_array_value, string_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << string_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> string_array_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, ArrayOctet)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(octet_array_2_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    uint8_t octet_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(octet_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(octet_array_2_value, octet_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(octet_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(octet_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, ArrayChar)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(char_array_2_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    char char_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(char_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(char_array_2_value, char_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(char_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(char_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, ArrayUnsignedShort)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(ushort_array_2_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    uint16_t ushort_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(ushort_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(ushort_array_2_value, ushort_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(ushort_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(ushort_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, ArrayShort)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(short_array_2_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    int16_t short_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(short_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(short_array_2_value, short_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(short_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(short_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, ArrayUnsignedLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(ulong_array_2_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    uint32_t ulong_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(ulong_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(ulong_array_2_value, ulong_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(ulong_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(ulong_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, ArrayLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(long_array_2_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    int32_t long_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(long_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(long_array_2_value, long_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(long_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(long_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, ArrayUnsignedLongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(ulonglong_array_2_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    uint64_t ulonglong_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(ulonglong_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(ulonglong_array_2_value, ulonglong_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(ulonglong_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(ulonglong_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, ArrayLongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(longlong_array_2_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    int64_t longlong_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(longlong_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(longlong_array_2_value, longlong_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(longlong_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(longlong_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, ArrayFloat)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(float_array_2_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    float float_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(float_array_2_value, 5);
    });

    EXPECT_ARRAY_FLOAT_EQ(float_array_2_value, float_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(float_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(float_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, ArrayDouble)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(double_array_2_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    double double_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(double_array_2_value, 5);
    });

    EXPECT_ARRAY_DOUBLE_EQ(double_array_2_value, double_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(double_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(double_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, ArrayBoolean)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(bool_array_2_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    bool bool_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(bool_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(bool_array_2_value, bool_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(bool_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(bool_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, ArrayString)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(string_array_2_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::string string_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(string_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(string_array_2_value, string_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(string_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(string_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDVectorOctet)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << octet_vector_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::vector<uint8_t> octet_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> octet_vector_value;
    });

    EXPECT_EQ(octet_vector_value, octet_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << octet_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> octet_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDVectorChar)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << char_vector_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::vector<char> char_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> char_vector_value;
    });

    EXPECT_EQ(char_vector_value, char_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << char_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> char_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDVectorUnsignedShort)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << ushort_vector_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::vector<uint16_t> ushort_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> ushort_vector_value;
    });

    EXPECT_EQ(ushort_vector_value, ushort_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << ushort_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> ushort_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDVectorShort)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << short_vector_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::vector<int16_t> short_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> short_vector_value;
    });

    EXPECT_EQ(short_vector_value, short_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << short_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> short_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDVectorUnsignedLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << ulong_vector_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::vector<uint32_t> ulong_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> ulong_vector_value;
    });

    EXPECT_EQ(ulong_vector_value, ulong_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << ulong_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> ulong_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDVectorLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << long_vector_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::vector<int32_t> long_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> long_vector_value;
    });

    EXPECT_EQ(long_vector_value, long_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << long_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> long_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDVectorUnsignedLongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << ulonglong_vector_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::vector<uint64_t> ulonglong_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> ulonglong_vector_value;
    });

    EXPECT_EQ(ulonglong_vector_value, ulonglong_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << ulonglong_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> ulonglong_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDVectorLongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << longlong_vector_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::vector<int64_t> longlong_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> longlong_vector_value;
    });

    EXPECT_EQ(longlong_vector_value, longlong_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << longlong_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> longlong_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDVectorFloat)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << float_vector_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::vector<float> float_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> float_vector_value;
    });

    EXPECT_EQ(float_vector_value, float_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << float_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> float_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDVectorDouble)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << double_vector_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::vector<double> double_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> double_vector_value;
    });

    EXPECT_EQ(double_vector_value, double_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << double_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> double_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDVectorBoolean)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << bool_vector_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::vector<bool> bool_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> bool_vector_value;
    });

    EXPECT_EQ(bool_vector_value, bool_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << bool_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> bool_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDVectorString)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << string_vector_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::vector<std::string> string_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> string_vector_value;
    });

    EXPECT_EQ(string_vector_value, string_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << string_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> string_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, STDTripleArrayUnsignedLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << triple_ulong_array_t;
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::array<std::array<std::array<uint32_t, 3>, 2>, 2> triple_ulong_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> triple_ulong_array_value;
    });

    EXPECT_EQ(triple_ulong_array_value, triple_ulong_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << triple_ulong_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> triple_ulong_array_value;
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, SequenceOctet)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(octet_seq_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    uint8_t *octet_seq_value = NULL; size_t octet_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(octet_seq_value, octet_seq_len);
    });

    EXPECT_EQ(octet_seq_len, 5);
    EXPECT_ARRAY_EQ(octet_seq_value, octet_seq_t, octet_seq_len);

    free(octet_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(octet_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(octet_seq_value, octet_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, SequenceChar)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(char_seq_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    char *char_seq_value = NULL; size_t char_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(char_seq_value, char_seq_len);
    });

    EXPECT_EQ(char_seq_len, 5);
    EXPECT_ARRAY_EQ(char_seq_value, char_seq_t, char_seq_len);

    free(char_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(char_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(char_seq_value, char_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, SequenceUnsignedShort)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(ushort_seq_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    uint16_t *ushort_seq_value = NULL; size_t ushort_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(ushort_seq_value, ushort_seq_len);
    });

    EXPECT_EQ(ushort_seq_len, 5);
    EXPECT_ARRAY_EQ(ushort_seq_value, ushort_seq_t, ushort_seq_len);

    free(ushort_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(ushort_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(ushort_seq_value, ushort_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, SequenceShort)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(short_seq_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    int16_t *short_seq_value = NULL; size_t short_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(short_seq_value, short_seq_len);
    });

    EXPECT_EQ(short_seq_len, 5);
    EXPECT_ARRAY_EQ(short_seq_value, short_seq_t, short_seq_len);

    free(short_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(short_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(short_seq_value, short_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, SequenceUnsignedLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(ulong_seq_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    uint32_t *ulong_seq_value = NULL; size_t ulong_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(ulong_seq_value, ulong_seq_len);
    });

    EXPECT_EQ(ulong_seq_len, 5);
    EXPECT_ARRAY_EQ(ulong_seq_value, ulong_seq_t, ulong_seq_len);

    free(ulong_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(ulong_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(ulong_seq_value, ulong_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, SequenceLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(long_seq_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    int32_t *long_seq_value = NULL; size_t long_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(long_seq_value, long_seq_len);
    });

    EXPECT_EQ(long_seq_len, 5);
    EXPECT_ARRAY_EQ(long_seq_value, long_seq_t, long_seq_len);

    free(long_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(long_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(long_seq_value, long_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, SequenceUnsignedLongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(ulonglong_seq_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    uint64_t *ulonglong_seq_value = NULL; size_t ulonglong_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(ulonglong_seq_value, ulonglong_seq_len);
    });

    EXPECT_EQ(ulonglong_seq_len, 5);
    EXPECT_ARRAY_EQ(ulonglong_seq_value, ulonglong_seq_t, ulonglong_seq_len);

    free(ulonglong_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(ulonglong_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(ulonglong_seq_value, ulonglong_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, SequenceLongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(longlong_seq_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    int64_t *longlong_seq_value = NULL; size_t longlong_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(longlong_seq_value, longlong_seq_len);
    });

    EXPECT_EQ(longlong_seq_len, 5);
    EXPECT_ARRAY_EQ(longlong_seq_value, longlong_seq_t, longlong_seq_len);

    free(longlong_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(longlong_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(longlong_seq_value, longlong_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, SequenceFloat)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(float_seq_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    float *float_seq_value = NULL; size_t float_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(float_seq_value, float_seq_len);
    });

    EXPECT_EQ(float_seq_len, 5);
    EXPECT_ARRAY_FLOAT_EQ(float_seq_value, float_seq_t, float_seq_len);

    free(float_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(float_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(float_seq_value, float_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, SequenceDouble)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(double_seq_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    double *double_seq_value = NULL; size_t double_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(double_seq_value, double_seq_len);
    });

    EXPECT_EQ(double_seq_len, 5);
    EXPECT_ARRAY_DOUBLE_EQ(double_seq_value, double_seq_t, double_seq_len);

    free(double_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(double_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(double_seq_value, double_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, SequenceBoolean)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(bool_seq_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    bool *bool_seq_value = NULL; size_t bool_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(bool_seq_value, bool_seq_len);
    });

    EXPECT_EQ(bool_seq_len, 5);
    EXPECT_ARRAY_EQ(bool_seq_value, bool_seq_t, bool_seq_len);

    free(bool_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(bool_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(bool_seq_value, bool_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, SequenceString)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(string_seq_t, 5);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    std::string *string_seq_value = NULL; size_t string_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(string_seq_value, string_seq_len);
    });

    EXPECT_EQ(string_seq_len, 5);
    EXPECT_ARRAY_EQ(string_seq_value, string_seq_t, string_seq_len);

    for(size_t count = 0; count < string_seq_len; ++count)
        string_seq_value[count].~basic_string();

    free(string_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(string_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(string_seq_value, string_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, CString)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serialize(c_string_t);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    char *c_string_value = NULL;

    EXPECT_NO_THROW(
    {
        cdr_des.deserialize(c_string_value);
    });

    EXPECT_EQ(strcmp(c_string_value, c_string_t) , 0);

    free(c_string_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    Cdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serialize(c_string_t);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    Cdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserialize(c_string_value);
    },
    NotEnoughMemoryException);
}

TEST(CDRTests, Complete)
{
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    Cdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << octet_t << char_t << ushort_t << short_t << ulong_t << long_t << ulonglong_t << longlong_t << float_tt << double_tt << bool_t << string_t;
        cdr_ser << octet_array_t << char_array_t << ushort_array_t << short_array_t << ulong_array_t << long_array_t << ulonglong_array_t << longlong_array_t << float_array_t << double_array_t << bool_array_t;
        cdr_ser.serializeArray(octet_array_2_t, 5).serializeArray(char_array_2_t, 5).serializeArray(ushort_array_2_t, 5).serializeArray(short_array_2_t, 5).serializeArray(ulong_array_2_t, 5);
        cdr_ser.serializeArray(long_array_2_t, 5).serializeArray(ulonglong_array_2_t, 5).serializeArray(longlong_array_2_t, 5).serializeArray(float_array_2_t, 5).serializeArray(double_array_2_t, 5).serializeArray(bool_array_2_t, 5);
        cdr_ser << octet_vector_t << char_vector_t << ushort_vector_t << short_vector_t << ulong_vector_t << long_vector_t << ulonglong_vector_t << longlong_vector_t << float_vector_t << double_vector_t << bool_vector_t;
        cdr_ser << triple_ulong_array_t;
        cdr_ser.serializeSequence(octet_seq_t, 5).serializeSequence(char_seq_t, 5).serializeSequence(ushort_seq_t, 5).serializeSequence(short_seq_t, 5);
        cdr_ser.serializeSequence(ulong_seq_t, 5).serializeSequence(long_seq_t, 5).serializeSequence(ulonglong_seq_t, 5).serializeSequence(longlong_seq_t, 5);
        cdr_ser.serializeSequence(float_seq_t, 5).serializeSequence(double_seq_t, 5);
        cdr_ser.serialize(c_string_t);
    });

    // Deseriazliation.
    Cdr cdr_des(cdrbuffer);

    uint8_t octet_value = 0;
    char char_value = 0;
    uint16_t ushort_value =0;
    int16_t short_value = 0;
    uint32_t ulong_value = 0;
    int32_t long_value = 0;
    uint64_t ulonglong_value = 0;
    int64_t longlong_value = 0;
    float float_value = 0;
    double double_value = 0;
    bool bool_value = false;
    std::string string_value = "";
    std::array<uint8_t, 5> octet_array_value;
    uint8_t octet_array_2_value[5];
    std::array<char, 5> char_array_value;
    char char_array_2_value[5];
    std::array<uint16_t, 5> ushort_array_value;
    uint16_t ushort_array_2_value[5];
    std::array<int16_t, 5> short_array_value;
    int16_t short_array_2_value[5];
    std::array<uint32_t, 5> ulong_array_value;
    uint32_t ulong_array_2_value[5];
    std::array<int32_t, 5> long_array_value;
    int32_t long_array_2_value[5];
    std::array<uint64_t, 5> ulonglong_array_value;
    uint64_t ulonglong_array_2_value[5];
    std::array<int64_t, 5> longlong_array_value;
    int64_t longlong_array_2_value[5];
    std::array<float, 5> float_array_value;
    float float_array_2_value[5];
    std::array<double, 5> double_array_value;
    double double_array_2_value[5];
    std::array<bool, 5> bool_array_value;
    bool bool_array_2_value[5];
    std::vector<uint8_t> octet_vector_value;
    std::vector<char> char_vector_value;
    std::vector<uint16_t> ushort_vector_value;
    std::vector<int16_t> short_vector_value;
    std::vector<uint32_t> ulong_vector_value;
    std::vector<int32_t> long_vector_value;
    std::vector<uint64_t> ulonglong_vector_value;
    std::vector<int64_t> longlong_vector_value;
    std::vector<float> float_vector_value;
    std::vector<double> double_vector_value;
    std::vector<bool> bool_vector_value;
    std::array<std::array<std::array<uint32_t, 3>, 2>, 2> triple_ulong_array_value;
    uint8_t *octet_seq_value = NULL; size_t octet_seq_len;
    char *char_seq_value = NULL; size_t char_seq_len;
    uint16_t *ushort_seq_value = NULL; size_t ushort_seq_len;
    int16_t *short_seq_value = NULL; size_t short_seq_len;
    uint32_t *ulong_seq_value = NULL; size_t ulong_seq_len;
    int32_t *long_seq_value = NULL; size_t long_seq_len;
    uint64_t *ulonglong_seq_value = NULL; size_t ulonglong_seq_len;
    int64_t *longlong_seq_value = NULL; size_t longlong_seq_len;
    float *float_seq_value = NULL; size_t float_seq_len;
    double *double_seq_value = NULL; size_t double_seq_len;
    char *c_string_value = NULL;

    EXPECT_NO_THROW(
    {
        cdr_des >> octet_value >> char_value >> ushort_value >> short_value >> ulong_value >> long_value >> ulonglong_value >> longlong_value >> float_value >> double_value >> bool_value >> string_value;
        cdr_des >> octet_array_value >> char_array_value >> ushort_array_value >> short_array_value >> ulong_array_value >> long_array_value >> ulonglong_array_value >> longlong_array_value >> float_array_value >> double_array_value >> bool_array_value;
        cdr_des.deserializeArray(octet_array_2_value, 5).deserializeArray(char_array_2_value, 5).deserializeArray(ushort_array_2_value, 5).deserializeArray(short_array_2_value, 5).deserializeArray(ulong_array_2_value, 5);
        cdr_des.deserializeArray(long_array_2_value, 5).deserializeArray(ulonglong_array_2_value, 5).deserializeArray(longlong_array_2_value, 5).deserializeArray(float_array_2_value, 5).deserializeArray(double_array_2_value, 5).deserializeArray(bool_array_2_value, 5);
        cdr_des >> octet_vector_value >> char_vector_value >> ushort_vector_value >> short_vector_value >> ulong_vector_value >> long_vector_value >> ulonglong_vector_value >> longlong_vector_value >> float_vector_value >> double_vector_value >> bool_vector_value;
        cdr_des >> triple_ulong_array_value;
        cdr_des.deserializeSequence(octet_seq_value, octet_seq_len).deserializeSequence(char_seq_value, char_seq_len).deserializeSequence(ushort_seq_value, ushort_seq_len); 
        cdr_des.deserializeSequence(short_seq_value, short_seq_len).deserializeSequence(ulong_seq_value, ulong_seq_len).deserializeSequence(long_seq_value, long_seq_len); 
        cdr_des.deserializeSequence(ulonglong_seq_value, ulonglong_seq_len).deserializeSequence(longlong_seq_value, longlong_seq_len).deserializeSequence(float_seq_value, float_seq_len); 
        cdr_des.deserializeSequence(double_seq_value, double_seq_len);
        cdr_des.deserialize(c_string_value);
    });

    EXPECT_EQ(octet_value, octet_t);
    EXPECT_EQ(char_value, char_t);
    EXPECT_EQ(ushort_value, ushort_t);
    EXPECT_EQ(short_value, short_t);
    EXPECT_EQ(ulong_value, ulong_t);
    EXPECT_EQ(long_value, long_t);
    EXPECT_EQ(ulonglong_value, ulonglong_t);
    EXPECT_EQ(longlong_value, longlong_t);
    EXPECT_FLOAT_EQ(float_value, float_tt);
    EXPECT_DOUBLE_EQ(double_value, double_tt);
    EXPECT_EQ(bool_value, bool_t);
    EXPECT_EQ(string_value, string_t);

    EXPECT_EQ(octet_array_value, octet_array_t);
    EXPECT_EQ(char_array_value, char_array_t);
    EXPECT_EQ(ushort_array_value, ushort_array_t);
    EXPECT_EQ(short_array_value, short_array_t);
    EXPECT_EQ(ulong_array_value, ulong_array_t);
    EXPECT_EQ(long_array_value, long_array_t);
    EXPECT_EQ(ulonglong_array_value, ulonglong_array_t);
    EXPECT_EQ(longlong_array_value, longlong_array_t);
    EXPECT_EQ(float_array_value, float_array_t);
    EXPECT_EQ(double_array_value, double_array_t);
    EXPECT_EQ(bool_array_value, bool_array_t);

    EXPECT_ARRAY_EQ(octet_array_2_value, octet_array_2_t, 5);
    EXPECT_ARRAY_EQ(char_array_2_value, char_array_2_t, 5);
    EXPECT_ARRAY_EQ(ushort_array_2_value, ushort_array_2_t, 5);
    EXPECT_ARRAY_EQ(short_array_2_value, short_array_2_t, 5);
    EXPECT_ARRAY_EQ(ulong_array_2_value, ulong_array_2_t, 5);
    EXPECT_ARRAY_EQ(long_array_2_value, long_array_2_t, 5);
    EXPECT_ARRAY_EQ(ulonglong_array_2_value, ulonglong_array_2_t, 5);
    EXPECT_ARRAY_EQ(longlong_array_2_value, longlong_array_2_t, 5);
    EXPECT_ARRAY_FLOAT_EQ(float_array_2_value, float_array_2_t, 5);
    EXPECT_ARRAY_DOUBLE_EQ(double_array_2_value, double_array_2_t, 5);
    EXPECT_ARRAY_EQ(bool_array_2_value, bool_array_2_t, 5);

    EXPECT_EQ(octet_vector_value, octet_vector_t);
    EXPECT_EQ(char_vector_value, char_vector_t);
    EXPECT_EQ(ushort_vector_value, ushort_vector_t);
    EXPECT_EQ(short_vector_value, short_vector_t);
    EXPECT_EQ(ulong_vector_value, ulong_vector_t);
    EXPECT_EQ(long_vector_value, long_vector_t);
    EXPECT_EQ(ulonglong_vector_value, ulonglong_vector_t);
    EXPECT_EQ(longlong_vector_value, longlong_vector_t);
    EXPECT_EQ(float_vector_value, float_vector_t);
    EXPECT_EQ(double_vector_value, double_vector_t);
    EXPECT_EQ(bool_vector_value, bool_vector_t);

    EXPECT_EQ(triple_ulong_array_t, triple_ulong_array_value);
    EXPECT_EQ(octet_seq_len, 5);
    EXPECT_ARRAY_EQ(octet_seq_value, octet_seq_t, 5);
    EXPECT_EQ(char_seq_len, 5);
    EXPECT_ARRAY_EQ(char_seq_value, char_seq_t, 5);
    EXPECT_EQ(ushort_seq_len, 5);
    EXPECT_ARRAY_EQ(ushort_seq_value, ushort_seq_t, 5);
    EXPECT_EQ(short_seq_len, 5);
    EXPECT_ARRAY_EQ(short_seq_value, short_seq_t, 5);
    EXPECT_EQ(ulong_seq_len, 5);
    EXPECT_ARRAY_EQ(ulong_seq_value, ulong_seq_t, 5);
    EXPECT_EQ(long_seq_len, 5);
    EXPECT_ARRAY_EQ(long_seq_value, long_seq_t, 5);
    EXPECT_EQ(ulonglong_seq_len, 5);
    EXPECT_ARRAY_EQ(ulonglong_seq_value, ulonglong_seq_t, 5);
    EXPECT_EQ(longlong_seq_len, 5);
    EXPECT_ARRAY_EQ(longlong_seq_value, longlong_seq_t, 5);
    EXPECT_EQ(float_seq_len, 5);
    EXPECT_ARRAY_FLOAT_EQ(float_seq_value, float_seq_t, 5);
    EXPECT_EQ(double_seq_len, 5);
    EXPECT_ARRAY_DOUBLE_EQ(double_seq_value, double_seq_t, 5);

    EXPECT_EQ(strcmp(c_string_t, c_string_value), 0);

    free(octet_seq_value);
    free(char_seq_value);
    free(ushort_seq_value);
    free(short_seq_value);
    free(ulong_seq_value);
    free(long_seq_value);
    free(ulonglong_seq_value);
    free(longlong_seq_value);
    free(float_seq_value);
    free(double_seq_value);
    free(c_string_value);
}

TEST(FastCDRTests, Octet)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << octet_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    uint8_t octet_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> octet_value;
    });

    EXPECT_EQ(octet_value, octet_t);
}

TEST(FastCDRTests, Char)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << char_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    char char_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> char_value;
    });

    EXPECT_EQ(char_value, char_t);
}

TEST(FastCDRTests, UnsignedShort)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << ushort_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    uint16_t ushort_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> ushort_value;
    });

    EXPECT_EQ(ushort_value, ushort_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << ushort_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> ushort_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, Short)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << short_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    int16_t short_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> short_value;
    });

    EXPECT_EQ(short_value, short_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << short_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> short_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, UnsignedLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << ulong_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    uint32_t ulong_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> ulong_value;
    });

    EXPECT_EQ(ulong_value, ulong_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << ulong_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> ulong_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, Long)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << long_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    int32_t long_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> long_value;
    });

    EXPECT_EQ(long_value, long_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << long_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> long_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, UnsignedLongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << ulonglong_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    uint64_t ulonglong_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> ulonglong_value;
    });

    EXPECT_EQ(ulonglong_value, ulonglong_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << ulonglong_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> ulonglong_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, LongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << longlong_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    int64_t longlong_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> longlong_value;
    });

    EXPECT_EQ(longlong_value, longlong_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << longlong_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> longlong_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, Float)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << float_tt;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    float float_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> float_value;
    });

    EXPECT_FLOAT_EQ(float_value, float_tt);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << float_tt;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> float_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, Double)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << double_tt;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    double double_value = 0;

    EXPECT_NO_THROW(
    {
        cdr_des >> double_value;
    });

    EXPECT_DOUBLE_EQ(double_value, double_tt);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << double_tt;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> double_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, Boolean)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << bool_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    bool bool_value = false;

    EXPECT_NO_THROW(
    {
        cdr_des >> bool_value;
    });

    EXPECT_EQ(bool_value, bool_t);
}

TEST(FastCDRTests, String)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << string_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::string string_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> string_value;
    });

    EXPECT_EQ(string_value, string_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << string_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> string_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, EmptyString)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << emptystring_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::string string_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> string_value;
    });

    EXPECT_EQ(string_value, emptystring_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << emptystring_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> string_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDArrayOctet)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << octet_array_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::array<uint8_t, 5> octet_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> octet_array_value;
    });

    EXPECT_EQ(octet_array_value, octet_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << octet_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> octet_array_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDArrayChar)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << char_array_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::array<char, 5> char_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> char_array_value;
    });

    EXPECT_EQ(char_array_value, char_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << char_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> char_array_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDArrayUnsignedShort)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << ushort_array_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::array<uint16_t, 5> ushort_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> ushort_array_value;
    });

    EXPECT_EQ(ushort_array_value, ushort_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << ushort_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> ushort_array_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDArrayShort)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << short_array_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::array<int16_t, 5> short_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> short_array_value;
    });

    EXPECT_EQ(short_array_value, short_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << short_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> short_array_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDArrayUnsignedLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << ulong_array_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::array<uint32_t, 5> ulong_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> ulong_array_value;
    });

    EXPECT_EQ(ulong_array_value, ulong_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << ulong_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> ulong_array_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDArrayLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << long_array_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::array<int32_t, 5> long_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> long_array_value;
    });

    EXPECT_EQ(long_array_value, long_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << long_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> long_array_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDArrayUnsignedLongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << ulonglong_array_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::array<uint64_t, 5> ulonglong_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> ulonglong_array_value;
    });

    EXPECT_EQ(ulonglong_array_value, ulonglong_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << ulonglong_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> ulonglong_array_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDArrayLongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << longlong_array_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::array<int64_t, 5> longlong_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> longlong_array_value;
    });

    EXPECT_EQ(longlong_array_value, longlong_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << longlong_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> longlong_array_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDArrayFloat)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << float_array_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::array<float, 5> float_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> float_array_value;
    });

    EXPECT_EQ(float_array_value, float_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << float_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> float_array_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDArrayDouble)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << double_array_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::array<double, 5> double_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> double_array_value;
    });

    EXPECT_EQ(double_array_value, double_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << double_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> double_array_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDArrayBoolean)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << bool_array_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::array<bool, 5> bool_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> bool_array_value;
    });

    EXPECT_EQ(bool_array_value, bool_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << bool_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> bool_array_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDArrayString)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << string_array_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::array<std::string, 5> string_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> string_array_value;
    });

    EXPECT_EQ(string_array_value, string_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << string_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> string_array_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, ArrayOctet)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(octet_array_2_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    uint8_t octet_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(octet_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(octet_array_2_value, octet_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(octet_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(octet_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, ArrayChar)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(char_array_2_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    char char_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(char_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(char_array_2_value, char_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(char_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(char_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, ArrayUnsignedShort)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(ushort_array_2_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    uint16_t ushort_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(ushort_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(ushort_array_2_value, ushort_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(ushort_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(ushort_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, ArrayShort)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(short_array_2_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    int16_t short_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(short_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(short_array_2_value, short_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(short_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(short_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, ArrayUnsignedLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(ulong_array_2_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    uint32_t ulong_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(ulong_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(ulong_array_2_value, ulong_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(ulong_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(ulong_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, ArrayLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(long_array_2_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    int32_t long_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(long_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(long_array_2_value, long_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(long_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(long_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, ArrayUnsignedLongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(ulonglong_array_2_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    uint64_t ulonglong_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(ulonglong_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(ulonglong_array_2_value, ulonglong_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(ulonglong_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(ulonglong_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, ArrayLongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(longlong_array_2_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    int64_t longlong_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(longlong_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(longlong_array_2_value, longlong_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(longlong_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(longlong_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, ArrayFloat)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(float_array_2_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    float float_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(float_array_2_value, 5);
    });

    EXPECT_ARRAY_FLOAT_EQ(float_array_2_value, float_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(float_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(float_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, ArrayDouble)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(double_array_2_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    double double_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(double_array_2_value, 5);
    });

    EXPECT_ARRAY_DOUBLE_EQ(double_array_2_value, double_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(double_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(double_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, ArrayBoolean)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(bool_array_2_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    bool bool_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(bool_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(bool_array_2_value, bool_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(bool_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(bool_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, ArrayString)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeArray(string_array_2_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::string string_array_2_value[5];

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeArray(string_array_2_value, 5);
    });

    EXPECT_ARRAY_EQ(string_array_2_value, string_array_2_t, 5);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeArray(string_array_2_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeArray(string_array_2_value, 5);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDVectorOctet)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << octet_vector_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::vector<uint8_t> octet_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> octet_vector_value;
    });

    EXPECT_EQ(octet_vector_value, octet_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << octet_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> octet_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDVectorChar)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << char_vector_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::vector<char> char_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> char_vector_value;
    });

    EXPECT_EQ(char_vector_value, char_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << char_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> char_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDVectorUnsignedShort)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << ushort_vector_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::vector<uint16_t> ushort_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> ushort_vector_value;
    });

    EXPECT_EQ(ushort_vector_value, ushort_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << ushort_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> ushort_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDVectorShort)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << short_vector_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::vector<int16_t> short_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> short_vector_value;
    });

    EXPECT_EQ(short_vector_value, short_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << short_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> short_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDVectorUnsignedLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << ulong_vector_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::vector<uint32_t> ulong_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> ulong_vector_value;
    });

    EXPECT_EQ(ulong_vector_value, ulong_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << ulong_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> ulong_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDVectorLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << long_vector_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::vector<int32_t> long_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> long_vector_value;
    });

    EXPECT_EQ(long_vector_value, long_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << long_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> long_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDVectorUnsignedLongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << ulonglong_vector_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::vector<uint64_t> ulonglong_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> ulonglong_vector_value;
    });

    EXPECT_EQ(ulonglong_vector_value, ulonglong_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << ulonglong_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> ulonglong_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDVectorLongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << longlong_vector_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::vector<int64_t> longlong_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> longlong_vector_value;
    });

    EXPECT_EQ(longlong_vector_value, longlong_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << longlong_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> longlong_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDVectorFloat)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << float_vector_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::vector<float> float_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> float_vector_value;
    });

    EXPECT_EQ(float_vector_value, float_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << float_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> float_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDVectorDouble)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << double_vector_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::vector<double> double_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> double_vector_value;
    });

    EXPECT_EQ(double_vector_value, double_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << double_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> double_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDVectorBoolean)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << bool_vector_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::vector<bool> bool_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> bool_vector_value;
    });

    EXPECT_EQ(bool_vector_value, bool_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << bool_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> bool_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDVectorString)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << string_vector_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::vector<std::string> string_vector_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> string_vector_value;
    });

    EXPECT_EQ(string_vector_value, string_vector_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << string_vector_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> string_vector_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, STDTripleArrayUnsignedLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << triple_ulong_array_t;
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::array<std::array<std::array<uint32_t, 3>, 2>, 2> triple_ulong_array_value;

    EXPECT_NO_THROW(
    {
        cdr_des >> triple_ulong_array_value;
    });

    EXPECT_EQ(triple_ulong_array_value, triple_ulong_array_t);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad << triple_ulong_array_t;
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad >> triple_ulong_array_value;
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, SequenceOctet)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(octet_seq_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    uint8_t *octet_seq_value = NULL; size_t octet_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(octet_seq_value, octet_seq_len);
    });

    EXPECT_EQ(octet_seq_len, 5);
    EXPECT_ARRAY_EQ(octet_seq_value, octet_seq_t, octet_seq_len);

    free(octet_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(octet_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(octet_seq_value, octet_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, SequenceChar)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(char_seq_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    char *char_seq_value = NULL; size_t char_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(char_seq_value, char_seq_len);
    });

    EXPECT_EQ(char_seq_len, 5);
    EXPECT_ARRAY_EQ(char_seq_value, char_seq_t, char_seq_len);

    free(char_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(char_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(char_seq_value, char_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, SequenceUnsignedShort)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(ushort_seq_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    uint16_t *ushort_seq_value = NULL; size_t ushort_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(ushort_seq_value, ushort_seq_len);
    });

    EXPECT_EQ(ushort_seq_len, 5);
    EXPECT_ARRAY_EQ(ushort_seq_value, ushort_seq_t, ushort_seq_len);

    free(ushort_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(ushort_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(ushort_seq_value, ushort_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, SequenceShort)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(short_seq_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    int16_t *short_seq_value = NULL; size_t short_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(short_seq_value, short_seq_len);
    });

    EXPECT_EQ(short_seq_len, 5);
    EXPECT_ARRAY_EQ(short_seq_value, short_seq_t, short_seq_len);

    free(short_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(short_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(short_seq_value, short_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, SequenceUnsignedLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(ulong_seq_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    uint32_t *ulong_seq_value = NULL; size_t ulong_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(ulong_seq_value, ulong_seq_len);
    });

    EXPECT_EQ(ulong_seq_len, 5);
    EXPECT_ARRAY_EQ(ulong_seq_value, ulong_seq_t, ulong_seq_len);

    free(ulong_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(ulong_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(ulong_seq_value, ulong_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, SequenceLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(long_seq_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    int32_t *long_seq_value = NULL; size_t long_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(long_seq_value, long_seq_len);
    });

    EXPECT_EQ(long_seq_len, 5);
    EXPECT_ARRAY_EQ(long_seq_value, long_seq_t, long_seq_len);

    free(long_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(long_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(long_seq_value, long_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, SequenceUnsignedLongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(ulonglong_seq_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    uint64_t *ulonglong_seq_value = NULL; size_t ulonglong_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(ulonglong_seq_value, ulonglong_seq_len);
    });

    EXPECT_EQ(ulonglong_seq_len, 5);
    EXPECT_ARRAY_EQ(ulonglong_seq_value, ulonglong_seq_t, ulonglong_seq_len);

    free(ulonglong_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(ulonglong_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(ulonglong_seq_value, ulonglong_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, SequenceLongLong)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(longlong_seq_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    int64_t *longlong_seq_value = NULL; size_t longlong_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(longlong_seq_value, longlong_seq_len);
    });

    EXPECT_EQ(longlong_seq_len, 5);
    EXPECT_ARRAY_EQ(longlong_seq_value, longlong_seq_t, longlong_seq_len);

    free(longlong_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(longlong_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(longlong_seq_value, longlong_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, SequenceFloat)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(float_seq_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    float *float_seq_value = NULL; size_t float_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(float_seq_value, float_seq_len);
    });

    EXPECT_EQ(float_seq_len, 5);
    EXPECT_ARRAY_FLOAT_EQ(float_seq_value, float_seq_t, float_seq_len);

    free(float_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(float_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(float_seq_value, float_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, SequenceDouble)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(double_seq_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    double *double_seq_value = NULL; size_t double_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(double_seq_value, double_seq_len);
    });

    EXPECT_EQ(double_seq_len, 5);
    EXPECT_ARRAY_DOUBLE_EQ(double_seq_value, double_seq_t, double_seq_len);

    free(double_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(double_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(double_seq_value, double_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, SequenceBoolean)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(bool_seq_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    bool *bool_seq_value = NULL; size_t bool_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(bool_seq_value, bool_seq_len);
    });

    EXPECT_EQ(bool_seq_len, 5);
    EXPECT_ARRAY_EQ(bool_seq_value, bool_seq_t, bool_seq_len);

    free(bool_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(bool_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(bool_seq_value, bool_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, SequenceString)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serializeSequence(string_seq_t, 5);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    std::string *string_seq_value = NULL; size_t string_seq_len = 0;

    EXPECT_NO_THROW(
    {
        cdr_des.deserializeSequence(string_seq_value, string_seq_len);
    });

    EXPECT_EQ(string_seq_len, 5);
    EXPECT_ARRAY_EQ(string_seq_value, string_seq_t, string_seq_len);

    for(size_t count = 0; count < string_seq_len; ++count)
        string_seq_value[count].~basic_string();

    free(string_seq_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serializeSequence(string_seq_t, 5);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserializeSequence(string_seq_value, string_seq_len);
    },
    NotEnoughMemoryException);
}

TEST(FastCDRTests, CString)
{
    // Check good case.
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser.serialize(c_string_t);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    char *c_string_value = NULL;

    EXPECT_NO_THROW(
    {
        cdr_des.deserialize(c_string_value);
    });

    EXPECT_EQ(strcmp(c_string_value, c_string_t) , 0);

    free(c_string_value);

    // Check bad case without space
    char buffer_bad[1];

    // Serialization.
    FastBuffer cdrbuffer_bad(buffer_bad, 1);
    FastCdr cdr_ser_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_ser_bad.serialize(c_string_t);
    },
    NotEnoughMemoryException);

    // Deseriazliation.
    FastCdr cdr_des_bad(cdrbuffer_bad);

    EXPECT_THROW(
    {
        cdr_des_bad.deserialize(c_string_value);
    },
    NotEnoughMemoryException);
}


TEST(FastCDRTests, Complete)
{
    char buffer[BUFFER_LENGTH];

    // Serialization.
    FastBuffer cdrbuffer(buffer, BUFFER_LENGTH);
    FastCdr cdr_ser(cdrbuffer);

    EXPECT_NO_THROW(
    {
        cdr_ser << octet_t << char_t << ushort_t << short_t << ulong_t << long_t << ulonglong_t << longlong_t << float_tt << double_tt << bool_t << string_t;
        cdr_ser << octet_array_t << char_array_t << ushort_array_t << short_array_t << ulong_array_t << long_array_t << ulonglong_array_t << longlong_array_t << float_array_t << double_array_t << bool_array_t;
        cdr_ser.serializeArray(octet_array_2_t, 5).serializeArray(char_array_2_t, 5).serializeArray(ushort_array_2_t, 5).serializeArray(short_array_2_t, 5).serializeArray(ulong_array_2_t, 5);
        cdr_ser.serializeArray(long_array_2_t, 5).serializeArray(ulonglong_array_2_t, 5).serializeArray(longlong_array_2_t, 5).serializeArray(float_array_2_t, 5).serializeArray(double_array_2_t, 5).serializeArray(bool_array_2_t, 5);
        cdr_ser << octet_vector_t << char_vector_t << ushort_vector_t << short_vector_t << ulong_vector_t << long_vector_t << ulonglong_vector_t << longlong_vector_t << float_vector_t << double_vector_t << bool_vector_t;
        cdr_ser << triple_ulong_array_t;
        cdr_ser.serializeSequence(octet_seq_t, 5).serializeSequence(char_seq_t, 5).serializeSequence(ushort_seq_t, 5).serializeSequence(short_seq_t, 5);
        cdr_ser.serializeSequence(ulong_seq_t, 5).serializeSequence(long_seq_t, 5).serializeSequence(ulonglong_seq_t, 5).serializeSequence(longlong_seq_t, 5);
        cdr_ser.serializeSequence(float_seq_t, 5).serializeSequence(double_seq_t, 5);
        cdr_ser.serialize(c_string_t);
    });

    // Deseriazliation.
    FastCdr cdr_des(cdrbuffer);

    uint8_t octet_value = 0;
    char char_value = 0;
    uint16_t ushort_value =0;
    int16_t short_value = 0;
    uint32_t ulong_value = 0;
    int32_t long_value = 0;
    uint64_t ulonglong_value = 0;
    int64_t longlong_value = 0;
    float float_value = 0;
    double double_value = 0;
    bool bool_value = false;
    std::string string_value = "";
    std::array<uint8_t, 5> octet_array_value;
    uint8_t octet_array_2_value[5];
    std::array<char, 5> char_array_value;
    char char_array_2_value[5];
    std::array<uint16_t, 5> ushort_array_value;
    uint16_t ushort_array_2_value[5];
    std::array<int16_t, 5> short_array_value;
    int16_t short_array_2_value[5];
    std::array<uint32_t, 5> ulong_array_value;
    uint32_t ulong_array_2_value[5];
    std::array<int32_t, 5> long_array_value;
    int32_t long_array_2_value[5];
    std::array<uint64_t, 5> ulonglong_array_value;
    uint64_t ulonglong_array_2_value[5];
    std::array<int64_t, 5> longlong_array_value;
    int64_t longlong_array_2_value[5];
    std::array<float, 5> float_array_value;
    float float_array_2_value[5];
    std::array<double, 5> double_array_value;
    double double_array_2_value[5];
    std::array<bool, 5> bool_array_value;
    bool bool_array_2_value[5];
    std::vector<uint8_t> octet_vector_value;
    std::vector<char> char_vector_value;
    std::vector<uint16_t> ushort_vector_value;
    std::vector<int16_t> short_vector_value;
    std::vector<uint32_t> ulong_vector_value;
    std::vector<int32_t> long_vector_value;
    std::vector<uint64_t> ulonglong_vector_value;
    std::vector<int64_t> longlong_vector_value;
    std::vector<float> float_vector_value;
    std::vector<double> double_vector_value;
    std::vector<bool> bool_vector_value;
    std::array<std::array<std::array<uint32_t, 3>, 2>, 2> triple_ulong_array_value;
    uint8_t *octet_seq_value = NULL; size_t octet_seq_len;
    char *char_seq_value = NULL; size_t char_seq_len;
    uint16_t *ushort_seq_value = NULL; size_t ushort_seq_len;
    int16_t *short_seq_value = NULL; size_t short_seq_len;
    uint32_t *ulong_seq_value = NULL; size_t ulong_seq_len;
    int32_t *long_seq_value = NULL; size_t long_seq_len;
    uint64_t *ulonglong_seq_value = NULL; size_t ulonglong_seq_len;
    int64_t *longlong_seq_value = NULL; size_t longlong_seq_len;
    float *float_seq_value = NULL; size_t float_seq_len;
    double *double_seq_value = NULL; size_t double_seq_len;
    char *c_string_value = NULL;

    EXPECT_NO_THROW(
    {
        cdr_des >> octet_value >> char_value >> ushort_value >> short_value >> ulong_value >> long_value >> ulonglong_value >> longlong_value >> float_value >> double_value >> bool_value >> string_value;
        cdr_des >> octet_array_value >> char_array_value >> ushort_array_value >> short_array_value >> ulong_array_value >> long_array_value >> ulonglong_array_value >> longlong_array_value >> float_array_value >> double_array_value >> bool_array_value;
        cdr_des.deserializeArray(octet_array_2_value, 5).deserializeArray(char_array_2_value, 5).deserializeArray(ushort_array_2_value, 5).deserializeArray(short_array_2_value, 5).deserializeArray(ulong_array_2_value, 5);
        cdr_des.deserializeArray(long_array_2_value, 5).deserializeArray(ulonglong_array_2_value, 5).deserializeArray(longlong_array_2_value, 5).deserializeArray(float_array_2_value, 5).deserializeArray(double_array_2_value, 5).deserializeArray(bool_array_2_value, 5);
        cdr_des >> octet_vector_value >> char_vector_value >> ushort_vector_value >> short_vector_value >> ulong_vector_value >> long_vector_value >> ulonglong_vector_value >> longlong_vector_value >> float_vector_value >> double_vector_value >> bool_vector_value;
        cdr_des >> triple_ulong_array_value;
        cdr_des.deserializeSequence(octet_seq_value, octet_seq_len).deserializeSequence(char_seq_value, char_seq_len).deserializeSequence(ushort_seq_value, ushort_seq_len); 
        cdr_des.deserializeSequence(short_seq_value, short_seq_len).deserializeSequence(ulong_seq_value, ulong_seq_len).deserializeSequence(long_seq_value, long_seq_len); 
        cdr_des.deserializeSequence(ulonglong_seq_value, ulonglong_seq_len).deserializeSequence(longlong_seq_value, longlong_seq_len).deserializeSequence(float_seq_value, float_seq_len); 
        cdr_des.deserializeSequence(double_seq_value, double_seq_len);
        cdr_des.deserialize(c_string_value);
    });

    EXPECT_EQ(octet_value, octet_t);
    EXPECT_EQ(char_value, char_t);
    EXPECT_EQ(ushort_value, ushort_t);
    EXPECT_EQ(short_value, short_t);
    EXPECT_EQ(ulong_value, ulong_t);
    EXPECT_EQ(long_value, long_t);
    EXPECT_EQ(ulonglong_value, ulonglong_t);
    EXPECT_EQ(longlong_value, longlong_t);
    EXPECT_FLOAT_EQ(float_value, float_tt);
    EXPECT_DOUBLE_EQ(double_value, double_tt);
    EXPECT_EQ(bool_value, bool_t);
    EXPECT_EQ(string_value, string_t);

    EXPECT_EQ(octet_array_value, octet_array_t);
    EXPECT_EQ(char_array_value, char_array_t);
    EXPECT_EQ(ushort_array_value, ushort_array_t);
    EXPECT_EQ(short_array_value, short_array_t);
    EXPECT_EQ(ulong_array_value, ulong_array_t);
    EXPECT_EQ(long_array_value, long_array_t);
    EXPECT_EQ(ulonglong_array_value, ulonglong_array_t);
    EXPECT_EQ(longlong_array_value, longlong_array_t);
    EXPECT_EQ(float_array_value, float_array_t);
    EXPECT_EQ(double_array_value, double_array_t);
    EXPECT_EQ(bool_array_value, bool_array_t);

    EXPECT_ARRAY_EQ(octet_array_2_value, octet_array_2_t, 5);
    EXPECT_ARRAY_EQ(char_array_2_value, char_array_2_t, 5);
    EXPECT_ARRAY_EQ(ushort_array_2_value, ushort_array_2_t, 5);
    EXPECT_ARRAY_EQ(short_array_2_value, short_array_2_t, 5);
    EXPECT_ARRAY_EQ(ulong_array_2_value, ulong_array_2_t, 5);
    EXPECT_ARRAY_EQ(long_array_2_value, long_array_2_t, 5);
    EXPECT_ARRAY_EQ(ulonglong_array_2_value, ulonglong_array_2_t, 5);
    EXPECT_ARRAY_EQ(longlong_array_2_value, longlong_array_2_t, 5);
    EXPECT_ARRAY_FLOAT_EQ(float_array_2_value, float_array_2_t, 5);
    EXPECT_ARRAY_DOUBLE_EQ(double_array_2_value, double_array_2_t, 5);
    EXPECT_ARRAY_EQ(bool_array_2_value, bool_array_2_t, 5);

    EXPECT_EQ(octet_vector_value, octet_vector_t);
    EXPECT_EQ(char_vector_value, char_vector_t);
    EXPECT_EQ(ushort_vector_value, ushort_vector_t);
    EXPECT_EQ(short_vector_value, short_vector_t);
    EXPECT_EQ(ulong_vector_value, ulong_vector_t);
    EXPECT_EQ(long_vector_value, long_vector_t);
    EXPECT_EQ(ulonglong_vector_value, ulonglong_vector_t);
    EXPECT_EQ(longlong_vector_value, longlong_vector_t);
    EXPECT_EQ(float_vector_value, float_vector_t);
    EXPECT_EQ(double_vector_value, double_vector_t);
    EXPECT_EQ(bool_vector_value, bool_vector_t);

    EXPECT_EQ(triple_ulong_array_t, triple_ulong_array_value);
    EXPECT_EQ(octet_seq_len, 5);
    EXPECT_ARRAY_EQ(octet_seq_value, octet_seq_t, 5);
    EXPECT_EQ(char_seq_len, 5);
    EXPECT_ARRAY_EQ(char_seq_value, char_seq_t, 5);
    EXPECT_EQ(ushort_seq_len, 5);
    EXPECT_ARRAY_EQ(ushort_seq_value, ushort_seq_t, 5);
    EXPECT_EQ(short_seq_len, 5);
    EXPECT_ARRAY_EQ(short_seq_value, short_seq_t, 5);
    EXPECT_EQ(ulong_seq_len, 5);
    EXPECT_ARRAY_EQ(ulong_seq_value, ulong_seq_t, 5);
    EXPECT_EQ(long_seq_len, 5);
    EXPECT_ARRAY_EQ(long_seq_value, long_seq_t, 5);
    EXPECT_EQ(ulonglong_seq_len, 5);
    EXPECT_ARRAY_EQ(ulonglong_seq_value, ulonglong_seq_t, 5);
    EXPECT_EQ(longlong_seq_len, 5);
    EXPECT_ARRAY_EQ(longlong_seq_value, longlong_seq_t, 5);
    EXPECT_EQ(float_seq_len, 5);
    EXPECT_ARRAY_FLOAT_EQ(float_seq_value, float_seq_t, 5);
    EXPECT_EQ(double_seq_len, 5);
    EXPECT_ARRAY_DOUBLE_EQ(double_seq_value, double_seq_t, 5);

    EXPECT_EQ(strcmp(c_string_t, c_string_value), 0);

    free(octet_seq_value);
    free(char_seq_value);
    free(ushort_seq_value);
    free(short_seq_value);
    free(ulong_seq_value);
    free(long_seq_value);
    free(ulonglong_seq_value);
    free(longlong_seq_value);
    free(float_seq_value);
    free(double_seq_value);
    free(c_string_value);
}
