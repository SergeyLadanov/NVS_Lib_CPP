
#include "main.hpp"
#include "NVS.hpp"
#include <cstdio>



uint8_t PageBuffer1[128];
uint8_t PageBuffer2[128];

class SettingsFlash_If : public NVS_IFlash
{
private:
    void PageErase(uint8_t *mem_ptr, uint16_t sector, uint32_t sector_size) override
    {
        memset(mem_ptr, 0xFF, sector_size);
    }


    void WriteData(uint8_t * mem_ptr, uint8_t *data_ptr, uint32_t len) override
    {
        memcpy(mem_ptr, data_ptr, len);
    }
public:
    SettingsFlash_If()
    {
        memset(PageBuffer1, 0xFF, sizeof(PageBuffer1));
        memset(PageBuffer2, 0xFF, sizeof(PageBuffer2));
    }
};


static const NVS::FlashDesc_t FlashDescriptor[] =
{
    {.MemPtr = (uint8_t *) PageBuffer1, .Size = sizeof(PageBuffer1), .Sector = 0},
    {.MemPtr = (uint8_t *) PageBuffer2, .Size = sizeof(PageBuffer2), .Sector = 1},
};


static SettingsFlash_If FlashInterface;


NVS Storage(FlashInterface);

uint32_t Check = 0;
uint32_t Writeble = 0;

// Основная программа
int main(void)
{
    Storage.Init((NVS::FlashDesc_t *) FlashDescriptor, 2);

    Storage.SetValue("test", Writeble++);

    Check = Storage.GetValue<uint32_t>("test");

    Storage.SetValue("test1", Writeble++);

    Check = Storage.GetValue<uint32_t>("test1");

    Storage.SetValue("test", Writeble++);

    Check = Storage.GetValue<uint32_t>("test");

    Storage.SetValue("test1", Writeble++);

    Check = Storage.GetValue<uint32_t>("test1");

    Storage.SetValue("test", Writeble++);

    Check = Storage.GetValue<uint32_t>("test");

    Storage.SetValue("test1", Writeble++);

    Check = Storage.GetValue<uint32_t>("test1");

    Storage.SetValue("test", Writeble++);

    Storage.SetValue("test1", Writeble++);


    Check = Storage.GetValue<uint32_t>("test1");

    Check = Storage.GetValue<uint32_t>("test");

    Storage.Init((NVS::FlashDesc_t *) FlashDescriptor, 2);


    Check = Storage.GetValue<uint32_t>("test1");

    Check = Storage.GetValue<uint32_t>("test");




    

    printf("End");
    return 0;  
}
