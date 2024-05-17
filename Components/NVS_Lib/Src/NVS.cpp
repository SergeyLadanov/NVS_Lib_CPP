#include "NVS.hpp"

NVS::NVS(NVS_IFlash &flash_if, FlashDesc_t *flash_desc, uint32_t len)
    : FlashDescriptors(flash_desc), FlashDescriptorsSize(len), FlashInterface(flash_if)
{

}




int32_t NVS::ScanWriteNumber(void)
{
    NVS_Page *Page = nullptr;

    int32_t MaxPage = -1;  
    
    for (uint32_t i = 0; i < FlashDescriptorsSize; i++)
    {
        Page = (NVS_Page *) FlashDescriptors[i].MemPtr;

        if (Page->IsReady())
        {
            if ((int32_t) Page->GetNumber() > MaxPage)
            {
                MaxPage = (int32_t) Page->GetNumber();
            }   
        }
    }

    return MaxPage;
}


uint32_t NVS::GetCurrentIndex(void)
{
    
    uint32_t current_write = GetWriteIndex();
    //printf("Write number: %d, write index: %d, read index: %d\r\n", WriteNumber, current_write, ((current_write + (FlashDescriptorsSize - 1)) % FlashDescriptorsSize));
    return ((current_write + (FlashDescriptorsSize - 1)) % FlashDescriptorsSize);
}


uint32_t NVS::GetBackupIndex(void)
{
    uint32_t current = GetCurrentIndex();
    return ((current + (FlashDescriptorsSize - 1)) % FlashDescriptorsSize);
}


uint32_t NVS::GetWriteIndex(void)
{
    return (WriteNumber % FlashDescriptorsSize);
}


void NVS::PagePrepare(uint32_t index)
{
    NVS_Page Page;
    Page.Init();
    Page.SetNumber(WriteNumber);
    FlashInterface.PageErase(FlashDescriptors[index].MemPtr, FlashDescriptors[index].Sector, FlashDescriptors[index].Size);
    FlashInterface.WriteData(FlashDescriptors[index].MemPtr, (uint8_t *) &Page, sizeof(NVS_Page));
}


void NVS::Init(FlashDesc_t *flash_desc, uint32_t len)
{

    int32_t LastNumTemp = -1;
    
    if (flash_desc)
    {
        FlashDescriptors = flash_desc;
    }

    if (len)
    {
        FlashDescriptorsSize = len;
    }


    LastNumTemp = ScanWriteNumber();
    
    if (LastNumTemp != -1)
    {
        WriteNumber = LastNumTemp;
    }
    else
    {
            //NVS_BlockWriter<32> block;

            //SaveConfig(block);
    }

}