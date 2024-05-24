
#include "main.hpp"
#include "NVS.hpp"
#include <cstdio>



static uint8_t PageBuffer1[256];
static uint8_t PageBuffer2[256];

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


static NVS Storage(FlashInterface);
static NVS CheckStorage(FlashInterface);

static uint16_t ProbeU16 = 0x35;
static uint16_t CheckProbeU16 = 0;

char *StrProbe = (char *) "hello, world";
char *CheckStrProbe = nullptr;


struct ProbeStruct_t
{
    uint32_t a1;
    uint32_t a2;
};


static ProbeStruct_t ProbeStruct = {35, 35};

static ProbeStruct_t CheckProbeStruct;

static ProbeStruct_t *CheckProbeStructPtr = nullptr;

static uint16_t outsize = 0;


static void Write(void)
{
    if (!Storage.SetValue("test_string", StrProbe))
    {
        printf("Write string success!\r\n");
    }
    else
    {
        printf("Write string failed!\r\n");
    }


    if (!Storage.SetValue("test_struct", (uint8_t *) &ProbeStruct, sizeof(ProbeStruct)))
    {
        printf("Write struct success!\r\n");
    }
    else
    {
        printf("Write struct failed!\r\n");
    }


    if (!Storage.SetValue("test_u16", ProbeU16))
    {
        printf("Write u16 success!\r\n");
    }
    else
    {
        printf("Write u16 failed!\r\n");
    }
}



static void Check(NVS &storage_for_check)
{
    CheckStrProbe = storage_for_check.GetString("test_string");

    if ((CheckStrProbe) && (!strcmp(CheckStrProbe, StrProbe)))
    {
        printf("String validation success!\r\n");
    }
    else
    {
        printf("String validation failed!\r\n");
    }


    CheckProbeU16 = storage_for_check.GetValue<typeof(CheckProbeU16)>("test_u16");

    if (ProbeU16 == CheckProbeU16)
    {
        printf("U16 validation success!\r\n");
    }
    else
    {
        printf("U16 validation failed!\r\n");
    }


    CheckProbeStructPtr = (ProbeStruct_t *)storage_for_check.GetArray("test_struct", &outsize);

    if ((CheckProbeStructPtr) && (CheckProbeStructPtr->a1 == ProbeStruct.a1) && (CheckProbeStructPtr->a2 == ProbeStruct.a2) && (outsize == sizeof(ProbeStruct_t)))
    {
        printf("Struct validation 1 success!\r\n");
    }
    else
    {
        printf("Struct validation 1 failed!\r\n");
    }

    if ((!storage_for_check.GetArray("test_struct", (uint8_t *) &CheckProbeStruct, &outsize)) && (CheckProbeStruct.a1 == ProbeStruct.a1) && (CheckProbeStruct.a2 == ProbeStruct.a2) && (outsize == sizeof(ProbeStruct_t)))
    {
        printf("Struct validation 2 success!\r\n");
    }
    else
    {
        printf("Struct validation 2 failed!\r\n");
    }
}



// Основная программа
int main(void)
{
    Storage.Init((NVS::FlashDesc_t *) FlashDescriptor, 2);

    for (uint8_t tries = 0; tries < 35; tries++)
    {
        Write();
    }

    printf("\r\n\r\nTest immediatly...\r\n\r\n");
    Check(Storage);

    printf("\r\n\r\n");

    printf("Initialization new storage in defined pages...\r\n");

    CheckStorage.Init((NVS::FlashDesc_t *) FlashDescriptor, 2);

    printf("\r\n\r\nTest after reset...\r\n\r\n");
    Check(CheckStorage);
    printf("\r\n\r\n");

    printf("End");
    return 0;  
}
