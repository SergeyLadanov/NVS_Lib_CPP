#ifndef __NVS_HPP__
#define __NVS_HPP__

#include "NVS_Page.hpp"
#include "NVS_Cell.hpp"
#include "NVS_IFlash.hpp"
#include "NVS_Log.h"
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

    uint32_t CurrentPageUsedBytes = 0;


public:
    NVS(NVS_IFlash &flash_if, FlashDesc_t *flash_desc = nullptr, uint32_t len = 0);

    void Init(FlashDesc_t *flash_desc, uint32_t len);


    template <typename T>
    int8_t SetValue(const char *key, T val)
    {
        NVS_Cell Data;

        Data.Init(key);
        Data.SetValue(val);

        return WriteCell(Data, key);
    }

    template <typename T>
    int8_t GetValue(const char *key, T &out)
    {
        NVS_Cell *Cell = FindCellByKey(key);
        
        int8_t res = -1;

        if (Cell)
        {
        	out = Cell->GetValue<T>();
        	res = 0;
        }

        return res;
    }

    void RemoveValue(const char *key);

    int8_t SetValue(const char *key, uint8_t *buf, uint16_t len);

    uint8_t *GetArray(const char *key, uint16_t *out_size = nullptr);

    int8_t GetArray(const char *key, uint8_t *out_buf, uint16_t *out_size);

    char *GetString(const char *key);

    uint32_t GetAvaliableSpaceInBytes(void);

    uint32_t GetAvaliableSpaceInBlocks(void);

private:

    uint32_t GetPageFreeSpace(void);

    NVS_Cell *FindCellByKey(const char *key);

    uint32_t GetUsedBytes(void);

    uint32_t ScanUsedBytes(void);

    void PagePrepare(uint32_t index, uint32_t number);

    void PageErase(uint32_t index);


    void ReleaseCell(NVS_Cell *cell);

    void CopyItem(NVS_Cell *cell_src, NVS_Cell *cell_dst);

    void ReleaseCurrentPage(void);

    int8_t WriteCell(NVS_Cell &new_cell, const char *key);

    int32_t ScanWriteNumber(void);

    uint32_t GetNewPageIndex(void);

    uint32_t GetCurrentIndex(void);

};

#endif /* __NVS_HPP__ */
