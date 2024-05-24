#ifndef __NVS_PAGE_HPP__
#define __NVS_PAGE_HPP__


#include <cstdint>
#include "NVS_Cell.hpp"
#include "NVS_Config.h"

class NVS_Page
{ 
public:
    static constexpr NVS_Tag_t TAG_START = (NVS_Tag_t) 0x35353535;

    static constexpr NVS_State_t STATE_ERASED = (NVS_State_t) 0xFFFFFFFF;
    static constexpr NVS_State_t STATE_VALID = (NVS_State_t) 0xAAAAAAAA;
    static constexpr NVS_State_t STATE_RELEASED = (NVS_State_t) 0x00000000;

#pragma pack(push, 1)
    struct Header_t
    {   
        NVS_Tag_t StartTag;
        uint32_t Number;
        NVS_State_t Reserved;
        NVS_State_t State;
    };

#pragma pack(pop)

    Header_t Header;
    uint8_t Data[];

    void Init(void)
    {
        Header.StartTag = TAG_START;
        Header.State = STATE_VALID;
        Header.Number = 0;
    }

    void SetNumber(uint32_t val)
    {
        Header.Number = val;
    }

    uint32_t GetNumber(void)
    {
        return Header.Number;
    }


    uint32_t GetHeaderSize(void)
    {
        if (IsReady())
        {
            return sizeof(Header);
        }
        
        return 0;
    }


    void SetState(uint32_t val)
    {
        Header.State = val;
    }


    uint32_t GetState(void)
    {
        return Header.State;
    }

    uint8_t *GetData(void)
    {
        return Data;
    }

    bool IsCleared(void)
    {
        if ((Header.State == STATE_ERASED) && (Header.StartTag == STATE_ERASED))
        {
            return true;
        }
        return false;
    }

    bool IsReady(void)
    {
        if ((Header.State == STATE_VALID) && (Header.StartTag == TAG_START))
        {
            return true;
        }

        return false;
    }



    bool IsMarkAsUsed(void)
    {
        if (Header.State == STATE_RELEASED)
        {
            return true;
        }

        return false;
    }


    bool IsReadyToErase(void)
    {
        if ((Header.State == STATE_RELEASED) || (Header.State == STATE_ERASED))
        {
            return true;
        }

        return false;
    }
};


#endif /* __NVS_PAGE_HPP__ */
