#pragma once
#include <functional>

#include "exceptions.hpp"

namespace snesl
{
    namespace endiannes
    {
        static inline bool IsBigEndian()
        {
            union
            {
                uint32_t i;
                char c[4];
            } testValue = { 0x01020304 };

            return testValue.c[0] == 1;
        }

        static inline const char * ConvertEndiannes(const char * value, size_t& size, bool sourceIsBigEndian, bool targetIsBigEndian)
        {
            if (value == nullptr)
                throw new ArgumentException("Value can't be null");

            char * convertedValue = const_cast<char *>(value);

            if (sourceIsBigEndian != targetIsBigEndian)
                std::reverse(convertedValue, convertedValue + size);

            return convertedValue;
        }

        static inline const char * ConvertFromLittleEndian(const char * value, size_t& size)
        {
            return ConvertEndiannes(value, size, false, IsBigEndian());
        }

        static inline const char * ConvertFromBigEndian(const char * value, size_t& size)
        {
            return ConvertEndiannes(value, size, true, IsBigEndian());
        }

        static inline const char * ConvertToLittleEndian(const char * value, size_t& size)
        {
            return ConvertEndiannes(value, size, IsBigEndian(), false);
        }

        static inline const char * ConvertToBigEndian(const char * value, size_t& size)
        {
            return ConvertEndiannes(value, size, IsBigEndian(), true);
        }
    }
}
