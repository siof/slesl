#pragma once

namespace snesl
{
    class binaryostream;

    class BinarySerializable
    {
    public:
        virtual binaryostream& operator << (const BinarySerializable& object) = 0;

        //virtual binaryistream& operator >> (const BinarySerializable& object) = 0;
    };
}
