#ifndef __NVS_PAGE_HPP__
#define __NVS_PAGE_HPP__


#include <cstdint>
#include "NVS_Cell.hpp"
//#include "NVS_IFlash.hpp"

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

    NVS_Page(uint8_t *start_ptr = nullptr)
    {
        SetStartAddr(start_ptr);
    }


    void SetStartAddr(uint8_t *start_ptr)
    {
        Header = (Header_t *) start_ptr;
    }
    

};


#endif /* __NVS_PAGE_HPP__ */
