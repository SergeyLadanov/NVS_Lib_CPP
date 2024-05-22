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

    uint32_t CurrentPageUsedBytes = 0;


public:
    NVS(NVS_IFlash &flash_if, FlashDesc_t *flash_desc = nullptr, uint32_t len = 0);

    int32_t ScanWriteNumber(void);


    uint32_t GetNewPageIndex(void);


    uint32_t GetCurrentIndex(void);


    void Init(FlashDesc_t *flash_desc, uint32_t len);


    void ReleaseCell(NVS_Cell *cell)
    {
        uint32_t NewState = NVS_Cell::STATE_RELEASED;
        FlashInterface.WriteData((uint8_t *) &cell->State, (uint8_t *) &NewState, sizeof(uint32_t));
    }


    template <typename T>
    void SetValue(const char *key, T val)
    {
        NVS_Page *Page = (NVS_Page *) FlashDescriptors[GetCurrentIndex()].MemPtr;
        NVS_Cell *Cell = (NVS_Cell *) Page->GetData();
        NVS_Cell Data;

        while (!Cell->IsEmpty())
        {
            if (Cell->IsKey(key))
            {
                ReleaseCell(Cell);
            }
            Cell = Cell->GetNext();
        }

        Data.Init(key);
        Data.SetValue(val);
        FlashInterface.WriteData((uint8_t *) Cell, (uint8_t *) &Data, Data.GetTotalSize());
        Data.State = Data.STATE_VALID;
        FlashInterface.WriteData((uint8_t *) &Cell->State, (uint8_t *) &Data.State, sizeof(Data.State));
    }


    template <typename T>
    T GetValue(const char *key)
    {
        NVS_Page *Page = (NVS_Page *) FlashDescriptors[GetCurrentIndex()].MemPtr;
        NVS_Cell *Cell = (NVS_Cell *) Page->GetData();


        while (!Cell->IsEmpty())
        {
            if (Cell->IsKey(key))
            {
                if (Cell->State == NVS_Cell::STATE_VALID)
                {
                    break;
                }
            }
            Cell = Cell->GetNext();
        }
        
    
        return Cell->GetValue<T>();
    }

private:

    uint32_t GetPageFreeSpace(void);

    uint32_t GetUsedBytes(void);

    uint32_t ScanUsedBytes(void);

    void PagePrepare(uint32_t index);

};

#endif /* __NVS_HPP__ */
