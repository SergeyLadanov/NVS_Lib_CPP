#ifndef __NVS_CELL_HPP__
#define __NVS_CELL_HPP__

#include <cstdint>
#include <cstdio>
#include "NVS_IFlash.hpp"




#pragma pack(push, 1)
class NVS_Cell
{
public:
    static constexpr uint32_t TAG_START = 0x35353535;
    static constexpr uint32_t STATE_ERASED = 0xFFFFFFFF;
    static constexpr uint32_t STATE_VALID = 0xAAAAAAAA;
    static constexpr uint32_t STATE_CLEARED = 0x00000000;



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


    struct RecordBlock_t
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

        uint32_t State;


        void Init(NVS_DataType type, const char * key = nullptr)
        {
            SetKey(key);
            StartTag = TAG_START;
            State = STATE_CLEARED;
            Type = (uint8_t) type;
        }

        void SetKey(const char * key)
        {
            if (key)
            {
                snprintf(Key, sizeof(Key), "%s", key);
            }
        }

        RecordBlock_t *GetNext(void)
        {
            return (RecordBlock_t *) (this + BlockCount);
        }

        uint8_t *GetBinaryData(void)
        {
            return (uint8_t *) (this + 1);
        }


        static constexpr size_t GetSize(void)
        {
            return (sizeof(RecordBlock_t));
        }

        static constexpr size_t GetDataSize(void)
        {
            return (GetSize() - GetStateSize());
        }

        static constexpr size_t GetStateSize(void)
        {
            return (sizeof(State));
        }
    };



    struct BinaryChunk_t
    {
        uint8_t Data[sizeof(RecordBlock_t)];

        static constexpr size_t GetSize(void)
        {
            return (sizeof(Data));
        }

        BinaryChunk_t *GetByIndex(uint8_t index)
        {
            return this + index;
        }
    };



    union DataBlock_t
    {
        RecordBlock_t Record;
        BinaryChunk_t Binary;
        
        uint8_t *GetBlobBlock(uint8_t block_num)
        {   
            return Binary.GetByIndex(block_num + 1)->Data;
        }

        DataBlock_t *GetNext(void)
        {
            return (DataBlock_t *) Record.GetNext();
        }


    };



#pragma pack(pop)

    DataBlock_t Body;
    uint8_t  *BinaryBufferPtr = nullptr;

    DataBlock_t *Read(uint8_t *buf)
    {
        return (DataBlock_t *) buf;
    }


};



#endif /* __NVS_CELL_HPP__ */
