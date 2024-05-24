
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
NVS CheckStorage(FlashInterface);

uint16_t Check = 0;
uint16_t Writeble = 0;

char *buf = "hello, world";
char *check_buf = nullptr;


// Основная программа
int main(void)
{
    Storage.Init((NVS::FlashDesc_t *) FlashDescriptor, 2);

    Storage.SetValue("test", buf);

    printf("Avaliavble in bytes: %d\r\n", Storage.GetAvaliableSpaceInBytes());

    printf("Avaliavble in blocks: %d\r\n", Storage.GetAvaliableSpaceInBlocks());

    check_buf = Storage.GetString("test");

    Check = Storage.GetValue<typeof(Check)>("test");

    Storage.SetValue("test", Writeble++);

    Check = Storage.GetValue<typeof(Check)>("test");

    Storage.SetValue("test1", Writeble++);

    Check = Storage.GetValue<typeof(Check)>("test1");

    Storage.SetValue("test", Writeble++);

    Check = Storage.GetValue<typeof(Check)>("test");

    Storage.SetValue("test1", Writeble++);

    Check = Storage.GetValue<typeof(Check)>("test1");

    Storage.SetValue("test", Writeble++);

    Check = Storage.GetValue<typeof(Check)>("test");

    Storage.SetValue("test1", Writeble++);

    Check = Storage.GetValue<typeof(Check)>("test1");

    Storage.SetValue("test", Writeble++);

    Storage.SetValue("test1", Writeble++);


    Check = Storage.GetValue<typeof(Check)>("test1");

    Check = Storage.GetValue<typeof(Check)>("test");







    CheckStorage.Init((NVS::FlashDesc_t *) FlashDescriptor, 2);


    Check = Storage.GetValue<typeof(Check)>("test1");

    Check = Storage.GetValue<typeof(Check)>("test");




    

    printf("End");
    return 0;  
}
