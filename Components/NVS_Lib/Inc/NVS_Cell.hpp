#ifndef __NVS_CELL_HPP__
#define __NVS_CELL_HPP__

#include <cstdint>
#include <cstdio>
#include "NVS_IFlash.hpp"
#include <cstring>



class NVS_Cell
{
public:
    static constexpr uint32_t TAG_START = 0x35353535;
    static constexpr uint32_t STATE_ERASED = 0xFFFFFFFF;
    static constexpr uint32_t STATE_VALID = 0xAAAAAAAA;
    static constexpr uint32_t STATE_CLEARED = 0x00000000;
    static constexpr size_t MAX_BINARY_BLOCK_NUMBER = 2;



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

#pragma pack(push, 1)


    struct Header_t
    {
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


        static constexpr size_t GetSize(void)
        {
            return (sizeof(Header_t));
        }
    };


    Header_t Header;
    uint32_t State;
    uint8_t Binary[];



#pragma pack(pop)

    void Init(NVS_DataType type, const char * key = nullptr)
    {
        SetKey(key);
        Header.StartTag = TAG_START;
        State = STATE_ERASED;
        Header.Type = (uint8_t) type;
    }

    uint8_t *GetBinaryPtr(void)
    {   
        return Binary;
    }

    size_t GetDescriptorSize(void)
    {
        return GetHeaderSize() + GetStateSize();
    }


    size_t GetTotalSize(void)
    {
        return (GetDescriptorSize() + GetDescriptorSize() * Header.BlockCount);
    }


    size_t GetHeaderSize(void)
    {
        return Header.GetSize();
    }


    size_t GetStateSize(void)
    {
        return sizeof(State);
    }

    
    bool IsKey(const char *key)
    {   
        if (strstr(key, Header.Key))
        {
            return true;
        }

        return false;
    }

    NVS_Cell *GetNext(void)
    {
        return (NVS_Cell *) &Binary[GetDescriptorSize() * Header.BlockCount];
    }

    void SetKey(const char * key)
    {
        if (key)
        {
            snprintf(Header.Key, sizeof(Header.Key), "%s", key);
        }
    }

};



#endif /* __NVS_CELL_HPP__ */
