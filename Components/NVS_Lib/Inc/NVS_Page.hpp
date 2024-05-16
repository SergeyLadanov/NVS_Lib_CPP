#ifndef __NVS_PAGE_HPP__
#define __NVS_PAGE_HPP__


#include <cstdint>
#include "NVS_Cell.hpp"

class NVS_Page
{ 
public:
    static constexpr uint32_t TAG_START = 0x35353535;

    static constexpr uint32_t STATE_ERASED = 0xFFFFFFFF;
    static constexpr uint32_t STATE_VALID = 0xAAAAAAAA;
    static constexpr uint32_t STATE_RELEASED = 0x00000000;

#pragma pack(push, 1)
    struct Header_t
    {   
        uint32_t StartTag;
        uint32_t State;
        uint32_t Number;
        uint32_t Reserved;
    };

#pragma pack(pop)

    Header_t Header;
    uint8_t Data[];

    uint32_t GetNumber(void)
    {
        return Header.Number;
    }

    uint8_t *GetData(void)
    {
        return Data;
    }
};


#endif /* __NVS_PAGE_HPP__ */
