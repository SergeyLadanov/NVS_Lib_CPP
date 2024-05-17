#ifndef __NVS_HPP__
#define __NVS_HPP__

#include "NVS_Page.hpp"
#include "NVS_Cell.hpp"
#include "NVS_IFlash.hpp"
#include <cstdint>

class NVS
{
public:
    struct FlashDesc_t
    {
        uint8_t *MemPtr;
        uint32_t Size;
        uint16_t Sector;
    };
private:
    bool IsInit = false;

    FlashDesc_t *FlashDescriptors = nullptr;

    uint32_t FlashDescriptorsSize = 0;

    uint32_t WriteNumber = 0;

    NVS_IFlash &FlashInterface;
public:
    NVS(NVS_IFlash &flash_if, FlashDesc_t *flash_desc = nullptr, uint32_t len = 0);


    int32_t ScanWriteNumber(void);


    uint32_t GetCurrentIndex(void);


    uint32_t GetBackupIndex(void);


    uint32_t GetWriteIndex(void);


    void Init(FlashDesc_t *flash_desc, uint32_t len);

};

#endif /* __NVS_HPP__ */
