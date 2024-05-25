#ifndef __NVS_CELL_HPP__
#define __NVS_CELL_HPP__

#include <cstdint>
#include <cstdio>
#include "NVS_IFlash.hpp"
#include <cstring>
#include "NVS_Config.h"



class NVS_Cell
{
public:
    static constexpr NVS_State_t STATE_ERASED = (NVS_State_t) NVS_CONF_ERASED_STATE_VALUE;
    static constexpr NVS_State_t STATE_VALID = (NVS_State_t) NVS_CONF_VALID_STATE_VALUE;
    static constexpr NVS_State_t STATE_RELEASED = (NVS_State_t) NVS_CONF_RELEASED_STATE_VALUE;

    static constexpr NVS_Tag_t TAG_START = (NVS_Tag_t) NVS_CONF_TAG_START_VALUE;
    static constexpr size_t MAX_BINARY_CELL_NUMBER = NVS_CONF_MAXBINARYCELLS_COUNT;
    



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
        NVS_Tag_t StartTag;
        char Key[NVS_CONF_KEY_SIZE];
        uint8_t BlockCount;
        uint8_t Type;
        union __Value
        {
            uint16_t UI16;
            uint8_t UI8;
            int16_t I16;
            int8_t I8;
            bool BoolVal;
            uint16_t BinarySize;
    #if NVS_CONF_MAXSIZE_OF_CELL_VALUES >=4
            float F32;
            uint32_t UI32;
            int32_t I32;
    #endif
    #if NVS_CONF_MAXSIZE_OF_CELL_VALUES >= 8
            uint64_t UI64;
            int64_t I64;
            double Double;
    #endif
        }Value;


        static constexpr size_t GetSize(void)
        {
            return (sizeof(Header_t));
        }
    };


    static constexpr size_t MEMORY_CELL_SIZE = (sizeof(Header_t) + sizeof(NVS_State_t));
    static constexpr size_t MAX_BINARY_BYTES = MAX_BINARY_CELL_NUMBER * MEMORY_CELL_SIZE;

    Header_t Header;
    NVS_State_t State;
    uint8_t Binary[MAX_BINARY_BYTES];

#pragma pack(pop)


    void Init(const char * key = nullptr)
    {
        SetKey(key);
        Header.StartTag = TAG_START;
        State = STATE_ERASED;
        //Header.Type = (uint8_t) type;
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

        if (IsEmpty())
        {
            return 0;
        }

        if (Header.BlockCount == 0xFF)
        {
            return GetMemoryCellSize();
        }

        return (GetMemoryCellSize() * Header.BlockCount);
    }


    size_t GetBinaryBlockCount(void)
    {
        return Header.BlockCount - 1;
    }


    size_t GetBinaryBlockSizeInBytes(void)
    {
        return (GetBinaryBlockCount() * GetMemoryCellSize());
    }


    size_t GetHeaderSize(void)
    {
        return Header.GetSize();
    }
    


    bool IsEmpty(void)
    {
        return (Header.StartTag == STATE_ERASED);
    }


    bool IsValid(void)
    {
        return ((Header.StartTag == TAG_START) && (State != STATE_ERASED));
    }


    bool IsCleared(void)
    {
        return (State == STATE_RELEASED);
    }


    size_t GetStateSize(void)
    {
        return sizeof(State);
    }

    
    bool IsKey(const char *key)
    {   
        if (!strcmp(key, Header.Key))
        {
            return true;
        }

        return false;
    }

    NVS_Cell *GetNext(void)
    {
        return (NVS_Cell *) &Binary[GetMemoryCellSize() * (Header.BlockCount-1)];
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
        return ((size / GetMemoryCellSize()) + 1);
    }


     // Установка значения

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

#if NVS_CONF_MAXSIZE_OF_CELL_VALUES >=4
    inline void SetValue(uint32_t val)
    {
        Header.Type = TYPE_UINT32;
        Header.Value.UI32 = val;
    }


    inline void SetValue(int32_t val)
    {
        Header.Type = TYPE_INT32;
        Header.Value.I32 = val;
    }



    inline void SetValue(float val)
    {
        Header.Type = TYPE_FLOAT;
        Header.Value.F32 = val;
    }
#endif

#if NVS_CONF_MAXSIZE_OF_CELL_VALUES >=8
    inline void SetValue(uint64_t val)
    {
        Header.Type = TYPE_UINT64;
        Header.Value.UI64 = val;
    }


    inline void SetValue(int64_t val)
    {

        Header.Type = TYPE_INT64;
        Header.Value.I64 = val;
    }


    inline void SetValue(double val)
    {
        Header.Type = TYPE_DOUBLE;
        Header.Value.Double = val;
    }
#endif


    inline void SetValue(char *str)
    {
        Header.Type = TYPE_ARRAY;
        Header.Value.BinarySize = snprintf((char *) Binary, sizeof(Binary), "%s", str) + 1;
        Header.BlockCount += GetBlockCountBySize(Header.Value.BinarySize);
    }


    inline void SetValue(uint8_t *buf, uint16_t len)
    {
        Header.Type = TYPE_ARRAY;
        if (len > sizeof(Binary))
        {
            len = sizeof(Binary);
        }
        Header.Value.BinarySize = len;
        Header.BlockCount += GetBlockCountBySize(Header.Value.BinarySize);
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


    inline uint8_t *GetArray(uint16_t *out_size = nullptr)
    {
        if (out_size)
        {
            *out_size = Header.Value.BinarySize;
        }

    	return Binary;
    }

};



#endif /* __NVS_CELL_HPP__ */
