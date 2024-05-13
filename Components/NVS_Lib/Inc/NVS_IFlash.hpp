#ifndef __NVS_IFLASH_HPP__
#define __NVS_IFLASH_HPP__

#include <cstdint>

class NVS_IFlash
{
public:
    virtual void PageErase(uint8_t *mem_ptr, uint16_t sector, uint32_t sector_size) = 0;
    virtual void WriteData(uint8_t *mem_ptr, uint8_t *data_ptr, uint32_t len) = 0;
};

#endif /* __NVS_IFLASH_HPP__ */
