#pragma once

#include <exception>
#include <string>

namespace snesl
{
    class ArgumentException : public std::exception
    {
    public:

        ArgumentException() :
            std::exception()
        {
        }

        explicit ArgumentException(const char * message) :
            std::exception(message)
        {
        }

        explicit ArgumentException(std::string & message) :
            ArgumentException(message.c_str())
        {
        }

    private:
    };
}
