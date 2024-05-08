#ifndef __NVS_VALUE_HPP__
#define __NVS_VALUE_HPP__

#include <cstdint>

#pragma pack(push, 1)
class NVS_Value
{
    // Типы значений
    typedef enum {
        TYPE_FLOAT = 1,
        TYPE_INT8,
        TYPE_INT16,
        TYPE_INT32,
        TYPE_INT64,
        TYPE_UINT8,
        TYPE_UINT16,
        TYPE_UINT32,
        TYPE_UINT64,
        TYPE_BOOL,
        TYPE_ARRAY,
        TYPE_DOUBLE
    }NVS_DataType;

    uint32_t StartTag;
    char Key[14];
    uint8_t BlockCount;
    uint8_t Type;
    union __Value
    {
        float F32;
        double Double;
        uint64_t UI64;
        uint32_t UI32;
        uint16_t UI16;
        uint8_t UI8;
        int64_t I64;
        int32_t I32;
        int16_t I16;
        int8_t I8;
        bool BoolVal;
        uint32_t BinarySize;
    }Value;
    uint32_t State;
};

#pragma pack(pop)



#endif /* __NVS_VALUE_HPP__ */
