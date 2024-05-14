#ifndef __NVS_PAGE_HPP__
#define __NVS_PAGE_HPP__


#include <cstdint>
#include "NVS_Cell.hpp"

class NVS_Page
{
    
public:
#pragma pack(push, 1)
    struct Header_t
    {   
        uint32_t State;
        uint32_t Number;
        uint8_t Data[];

        uint8_t *GetData(void)
        {
            return Data;
        }
    };

#pragma pack(pop)

    Header_t *Header = nullptr;
    

};


#endif /* __NVS_PAGE_HPP__ */
