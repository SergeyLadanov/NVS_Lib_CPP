
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

bool Check;

// Основная программа
int main(void)
{
    Storage.Init((NVS::FlashDesc_t *) FlashDescriptor, 2);

    Storage.SetValue("test", false);

    Storage.SetValue("test", false);

    Storage.SetValue("test", false);

    Storage.SetValue("test", false);

    Storage.SetValue("test", false);

    Storage.SetValue("test", false);

    Storage.SetValue("test", false);

    Storage.SetValue("test", false);

    Check = Storage.GetValue<bool>("test");

    printf("End");
    return 0;  
}
