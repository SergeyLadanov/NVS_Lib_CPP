#ifndef TEST_INFRASTRUCTURE_HPP
#define TEST_INFRASTRUCTURE_HPP

#include "NVS.hpp"
#include "NVS_Cell.hpp"
#include "NVS_Page.hpp"
#include "NVS_Config.h"
#include "NVS_Format.h"
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cassert>

/*
 * ============================================================
 *  МОК-РЕАЛИЗАЦИЯ ФЛЕШ-ИНТЕРФЕЙСА (RAM-буферы)
 * ============================================================
 */

static constexpr uint32_t PAGE_SIZE = 256;
static uint8_t PageBufA[PAGE_SIZE];
static uint8_t PageBufB[PAGE_SIZE];

class MockFlash : public NVS_IFlash
{
private:
    void PageErase(uint8_t *mem_ptr, uint16_t sector, uint32_t sector_size) override
    {
        (void)sector;
        memset(mem_ptr, 0xFF, sector_size);
    }

    void WriteData(uint8_t *mem_ptr, uint8_t *data_ptr, uint32_t len) override
    {
        memcpy(mem_ptr, data_ptr, len);
    }

public:
    MockFlash()
    {
        memset(PageBufA, 0xFF, sizeof(PageBufA));
        memset(PageBufB, 0xFF, sizeof(PageBufB));
    }

    void Reset(void)
    {
        memset(PageBufA, 0xFF, sizeof(PageBufA));
        memset(PageBufB, 0xFF, sizeof(PageBufB));
    }
};

static MockFlash FlashIf;

static NVS::FlashDesc_t FlashDesc[2];

static void InitFlashDesc(void)
{
    FlashDesc[0].MemPtr = PageBufA;
    FlashDesc[0].Size   = PAGE_SIZE;
    FlashDesc[0].Sector = 0;

    FlashDesc[1].MemPtr = PageBufB;
    FlashDesc[1].Size   = PAGE_SIZE;
    FlashDesc[1].Sector = 1;
}

static NVS CreateFreshStorage(void)
{
    FlashIf.Reset();
    InitFlashDesc();
    NVS storage(FlashIf);
    storage.Init(FlashDesc, 2);
    return storage;
}

/*
 * ============================================================
 *  ВСПОМОГАТЕЛЬНЫЕ КЛЮЧИ
 * ============================================================
 */

#if NVS_CONF_USE_STRING_KEY != 0
    static NVS_Key_t KeyI8     = "t_i8";
    static NVS_Key_t KeyI16    = "t_i16";
    static NVS_Key_t KeyI32    = "t_i32";
    static NVS_Key_t KeyI64    = "t_i64";
    static NVS_Key_t KeyUI8    = "t_ui8";
    static NVS_Key_t KeyUI16   = "t_ui16";
    static NVS_Key_t KeyUI32   = "t_ui32";
    static NVS_Key_t KeyUI64   = "t_ui64";
    static NVS_Key_t KeyFloat  = "t_flt";
    static NVS_Key_t KeyDouble = "t_dbl";
    static NVS_Key_t KeyBool   = "t_bool";
    static NVS_Key_t KeyString = "t_str";
    static NVS_Key_t KeyArray  = "t_arr";
    static NVS_Key_t KeyNone   = "t_none";
#else
    enum {
        KEY_I8 = 1, KEY_I16, KEY_I32, KEY_I64,
        KEY_UI8, KEY_UI16, KEY_UI32, KEY_UI64,
        KEY_FLOAT, KEY_DOUBLE, KEY_BOOL,
        KEY_STRING, KEY_ARRAY, KEY_NONE
    };
    static NVS_Key_t KeyI8     = KEY_I8;
    static NVS_Key_t KeyI16    = KEY_I16;
    static NVS_Key_t KeyI32    = KEY_I32;
    static NVS_Key_t KeyI64    = KEY_I64;
    static NVS_Key_t KeyUI8    = KEY_UI8;
    static NVS_Key_t KeyUI16   = KEY_UI16;
    static NVS_Key_t KeyUI32   = KEY_UI32;
    static NVS_Key_t KeyUI64   = KEY_UI64;
    static NVS_Key_t KeyFloat  = KEY_FLOAT;
    static NVS_Key_t KeyDouble = KEY_DOUBLE;
    static NVS_Key_t KeyBool   = KEY_BOOL;
    static NVS_Key_t KeyString = KEY_STRING;
    static NVS_Key_t KeyArray  = KEY_ARRAY;
    static NVS_Key_t KeyNone   = KEY_NONE;
#endif

/*
 * ============================================================
 *  ВСПОМОГАТЕЛЬНЫЕ МАКРОСЫ
 * ============================================================
 */

#define TEST_SECTION(title)             nvs_printf("\r\n========== {} ==========\r\n", title)
#define TEST_SUBSECTION(title)          nvs_printf("\r\n--- {} ---\r\n", title)

#endif /* TEST_INFRASTRUCTURE_HPP */