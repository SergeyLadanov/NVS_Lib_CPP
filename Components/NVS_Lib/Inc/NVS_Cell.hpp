#ifndef __NVS_CELL_HPP__
#define __NVS_CELL_HPP__

#include <cstdint>
#include <cstdio>
#include "NVS_IFlash.hpp"
#include <cstring>



class NVS_Cell
{
public:
    static constexpr uint32_t STATE_ERASED = 0xFFFFFFFF;
    static constexpr uint32_t STATE_VALID = 0xAAAAAAAA;
    static constexpr uint32_t STATE_RELEASED = 0x00000000;

    static constexpr uint32_t TAG_START = 0x35353535;
    static constexpr size_t MAX_BINARY_CELL_NUMBER = 2;
    



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
        TYPE_DOUBLE,
        TYPE_ARRAY
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


    static constexpr size_t MEMORY_CELL_SIZE = (sizeof(Header_t) + sizeof(uint32_t));
    static constexpr size_t MAX_BINARY_BYTES = MAX_BINARY_CELL_NUMBER * MEMORY_CELL_SIZE;

    Header_t Header;
    uint32_t State;
    uint8_t Binary[MAX_BINARY_BYTES];

#pragma pack(pop)


    void Init(NVS_DataType type, const char * key = nullptr)
    {
        SetKey(key);
        Header.StartTag = TAG_START;
        State = STATE_ERASED;
        Header.Type = (uint8_t) type;
        Header.BlockCount = 1;
    }

    uint8_t *GetBinaryPtr(void)
    {   
        return Binary;
    }

    size_t GetMemoryCellSize(void)
    {
        return MEMORY_CELL_SIZE;
    }


    size_t GetTotalSize(void)
    {
        return (GetMemoryCellSize() + (GetMemoryCellSize() * Header.BlockCount));
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
        return (NVS_Cell *) &Binary[GetMemoryCellSize() * Header.BlockCount];
    }

    void SetKey(const char * key)
    {
        if (key)
        {
            snprintf(Header.Key, sizeof(Header.Key), "%s", key);
        }
    }


    uint8_t GetBlockCountBySize(uint32_t size)
    {
        return ((size % GetMemoryCellSize()) + 1);
    }


     // Установка значения
    inline void SetValue(float val)
    {
        Header.Type = TYPE_FLOAT;
        Header.Value.F32 = val;
    }

    inline void SetValue(double val)
    {
        Header.Type = TYPE_DOUBLE;
        Header.Value.Double = val;
    }

    inline void SetValue(int8_t val)
    {
        Header.Type = TYPE_INT8;
        Header.Value.I8 = val;
    }


    inline void SetValue(int16_t val)
    {
        Header.Type = TYPE_INT16;
        Header.Value.I16 = val;
    }

    inline void SetValue(int32_t val)
    {
        Header.Type = TYPE_INT32;
        Header.Value.I32 = val;
    }

    inline void SetValue(int64_t val)
    {

        Header.Type = TYPE_INT64;
        Header.Value.I64 = val;
    }

    inline void SetValue(uint8_t val)
    {
        Header.Type = TYPE_UINT8;
        Header.Value.UI8 = val;
    }

    inline void SetValue(uint16_t val)
    {
        Header.Type = TYPE_UINT16;
        Header.Value.UI16 = val;
    }


    inline void SetValue(uint32_t val)
    {
        Header.Type = TYPE_UINT32;
        Header.Value.UI32 = val;
    }

    inline void SetValue(uint64_t val)
    {
        Header.Type = TYPE_UINT64;
        Header.Value.UI64 = val;
    }

    inline void SetValue(char *str)
    {
        Header.Type = TYPE_ARRAY;
        Header.Value.BinarySize = snprintf((char *) Binary, sizeof(Binary), "%s", str) + 1;
        Header.BlockCount = GetBlockCountBySize(Header.Value.BinarySize);
    }


    inline void SetValue(uint8_t *buf, uint16_t len)
    {
        Header.Type = TYPE_ARRAY;
        if (len > sizeof(Binary))
        {
            len = sizeof(Binary);
        }
        Header.Value.BinarySize = len;
        memcpy(Binary, buf, len);
    }



    inline void SetValue(bool val)
    {
        Header.Type = TYPE_BOOL;
        Header.Value.BoolVal = val;
    }


    template <typename T>
    inline T GetValue(void)
    {
        T* val = (T *) &Header.Value;
        return *val;
    }

    inline char *GetString(void)
    {
    	return (char *) Binary;
    }


    inline uint8_t *GetArray(uint32_t *out_size = nullptr)
    {
        if (out_size)
        {
            *out_size = Header.Value.BinarySize;
        }

    	return Binary;
    }

};



#endif /* __NVS_CELL_HPP__ */
