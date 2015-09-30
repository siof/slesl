#pragma once

#include <asio.hpp>

#include "endiannes.hpp"

namespace snesl
{
    class binaryostream
    {
    public:
        binaryostream() :
            _buffer(), _stream(&_buffer)
        {
        }

        binaryostream(const binaryostream&) = delete;
        binaryostream(const binaryostream&&) = delete;

        binaryostream & operator = (const binaryostream&) = delete;

        asio::const_buffers_1 GetData()
        {
            return _buffer.data();
        }

        binaryostream& operator << (const char& value)
        {
            return EndianConvertAndWriteData(&value, sizeof(char));
        }

        binaryostream& operator << (const char&& value)
        {
            return (*this) << value;
        }

        binaryostream& operator << (const short& value)
        {
            return EndianConvertAndWriteData(&value, sizeof(short));
        }

        binaryostream& operator << (const short&& value)
        {
            return (*this) << value;
        }

        binaryostream& operator << (const unsigned short& value)
        {
            return EndianConvertAndWriteData(&value, sizeof(unsigned short));
        }

        binaryostream& operator << (const unsigned short&& value)
        {
            return (*this) << value;
        }

        binaryostream& operator << (const int& value)
        {
            return EndianConvertAndWriteData(&value, sizeof(int));
        }

        binaryostream& operator << (const int&& value)
        {
            return (*this) << value;
        }

        binaryostream& operator << (const unsigned int& value)
        {
            return EndianConvertAndWriteData(&value, sizeof(unsigned int));
        }

        binaryostream& operator << (const unsigned int&& value)
        {
            return (*this) << value;
        }

        binaryostream& operator << (const long& value)
        {
            return EndianConvertAndWriteData(&value, sizeof(long));
        }

        binaryostream& operator << (const long&& value)
        {
            return (*this) << value;
        }

        binaryostream& operator << (const unsigned long& value)
        {
            return EndianConvertAndWriteData(&value, sizeof(unsigned long));
        }

        binaryostream& operator << (const unsigned long&& value)
        {
            return (*this) << value;
        }

        binaryostream& operator << (const long long& value)
        {
            return EndianConvertAndWriteData(&value, sizeof(long long));
        }

        binaryostream& operator << (const long long&& value)
        {
            return (*this) << value;
        }

        binaryostream& operator << (const unsigned long long& value)
        {
            return EndianConvertAndWriteData(&value, sizeof(unsigned long long));
        }

        binaryostream& operator << (const unsigned long long&& value)
        {
            return (*this) << value;
        }

        binaryostream& operator << (const bool& value)
        {
            return EndianConvertAndWriteData(&value, sizeof(bool));
        }

        binaryostream& operator << (const bool&& value)
        {
            return (*this) << value;
        }

        binaryostream& operator << (const float& value)
        {
            return EndianConvertAndWriteData(&value, sizeof(float));
        }

        binaryostream& operator << (const float&& value)
        {
            return (*this) << value;
        }

        binaryostream& operator << (const double& value)
        {
            return EndianConvertAndWriteData(&value, sizeof(double));
        }

        binaryostream& operator << (const double&& value)
        {
            return (*this) << value;
        }

        binaryostream& operator << (const long double& value)
        {
            return EndianConvertAndWriteData(&value, sizeof(long double));
        }

        binaryostream& operator << (const long double&& value)
        {
            return (*this) << value;
        }

        binaryostream& operator << (const char * value)
        {
            return WriteData(GetBytes(value), sizeof value);
        }

        binaryostream& operator << (const std::string& value)
        {
            return WriteData(value.c_str(), value.size());
        }

        binaryostream& operator << (const std::string&& value)
        {
            return (*this) << value;
        }

    private:
        asio::streambuf _buffer;
        std::ostream _stream;

        template<typename T>
        binaryostream& EndianConvertAndWriteData(T* value, size_t&& size)
        {
            const char* bytes = GetBytes(value);
            endiannes::ConvertToBigEndian(bytes, size);

            return WriteData(bytes, std::move(size));
        }

        template<typename T>
        const char* GetBytes(T* value)
        {
            return reinterpret_cast<const char*>(value);
        }

        binaryostream& WriteData(const char* value, size_t&& size)
        {
            _stream.write(reinterpret_cast<const char *>(value), size);

            return *this;
        }
    };
}
