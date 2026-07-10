#include "TestInfrastructure.hpp"

int main(void)
{
    TEST_SECTION("Тестирование граничных значений");

    /* -------------------------------------------------- */
    /*  Минимальные / максимальные значения              */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("Экстремальные значения");

    // int8: -128 .. 127
    {
        NVS storage = CreateFreshStorage();
        int8_t v = -128;
        storage.SetValue((NVS_Key_t)"i8_min", v);
        int8_t r = 0;
        storage.GetValue((NVS_Key_t)"i8_min", r);
        assert(r == -128);

        v = 127;
        storage.SetValue((NVS_Key_t)"i8_max", v);
        storage.GetValue((NVS_Key_t)"i8_max", r);
        assert(r == 127);
    }

    // uint8: 0 .. 255
    {
        NVS storage = CreateFreshStorage();
        uint8_t v = 0;
        storage.SetValue((NVS_Key_t)"u8_min", v);
        uint8_t r = 1;
        storage.GetValue((NVS_Key_t)"u8_min", r);
        assert(r == 0);

        v = 255;
        storage.SetValue((NVS_Key_t)"u8_max", v);
        storage.GetValue((NVS_Key_t)"u8_max", r);
        assert(r == 255);
    }

    // int16: -32768 .. 32767
    {
        NVS storage = CreateFreshStorage();
        int16_t v = -32768;
        storage.SetValue((NVS_Key_t)"i16_min", v);
        int16_t r = 0;
        storage.GetValue((NVS_Key_t)"i16_min", r);
        assert(r == -32768);

        v = 32767;
        storage.SetValue((NVS_Key_t)"i16_max", v);
        storage.GetValue((NVS_Key_t)"i16_max", r);
        assert(r == 32767);
    }

    // uint16: 0 .. 65535
    {
        NVS storage = CreateFreshStorage();
        uint16_t v = 0;
        storage.SetValue((NVS_Key_t)"u16_min", v);
        uint16_t r = 1;
        storage.GetValue((NVS_Key_t)"u16_min", r);
        assert(r == 0);

        v = 65535;
        storage.SetValue((NVS_Key_t)"u16_max", v);
        storage.GetValue((NVS_Key_t)"u16_max", r);
        assert(r == 65535);
    }

#if NVS_CONF_MAXSIZE_OF_CELL_VALUES >= 4
    // int32: -2147483648 .. 2147483647
    {
        NVS storage = CreateFreshStorage();
        int32_t v = -2147483647 - 1;
        storage.SetValue((NVS_Key_t)"i32_min", v);
        int32_t r = 0;
        storage.GetValue((NVS_Key_t)"i32_min", r);
        assert(r == (-2147483647 - 1));

        v = 2147483647;
        storage.SetValue((NVS_Key_t)"i32_max", v);
        storage.GetValue((NVS_Key_t)"i32_max", r);
        assert(r == 2147483647);
    }

    // uint32: 0 .. 4294967295
    {
        NVS storage = CreateFreshStorage();
        uint32_t v = 0;
        storage.SetValue((NVS_Key_t)"u32_min", v);
        uint32_t r = 1;
        storage.GetValue((NVS_Key_t)"u32_min", r);
        assert(r == 0);

        v = 4294967295U;
        storage.SetValue((NVS_Key_t)"u32_max", v);
        storage.GetValue((NVS_Key_t)"u32_max", r);
        assert(r == 4294967295U);
    }
#endif

#if NVS_CONF_MAXSIZE_OF_CELL_VALUES >= 8
    // int64: -9223372036854775808 .. 9223372036854775807
    {
        NVS storage = CreateFreshStorage();
        int64_t v = -9223372036854775807LL - 1;
        storage.SetValue((NVS_Key_t)"i64_min", v);
        int64_t r = 0;
        storage.GetValue((NVS_Key_t)"i64_min", r);
        assert(r == (-9223372036854775807LL - 1));

        v = 9223372036854775807LL;
        storage.SetValue((NVS_Key_t)"i64_max", v);
        storage.GetValue((NVS_Key_t)"i64_max", r);
        assert(r == 9223372036854775807LL);
    }

    // uint64: 0 .. 18446744073709551615
    {
        NVS storage = CreateFreshStorage();
        uint64_t v = 0;
        storage.SetValue((NVS_Key_t)"u64_min", v);
        uint64_t r = 1;
        storage.GetValue((NVS_Key_t)"u64_min", r);
        assert(r == 0);

        v = 18446744073709551615ULL;
        storage.SetValue((NVS_Key_t)"u64_max", v);
        storage.GetValue((NVS_Key_t)"u64_max", r);
        assert(r == 18446744073709551615ULL);
    }
#endif

    /* -------------------------------------------------- */
    /*  Пустая строка                                    */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("Пустая строка");
    {
        NVS storage = CreateFreshStorage();
        assert(storage.SetValue((NVS_Key_t)"empty_str", (char *)"") == 0);
        char *r = storage.GetString((NVS_Key_t)"empty_str");
        assert(r != nullptr);
        assert(strlen(r) == 0);
    }

    /* -------------------------------------------------- */
    /*  Массив нулевой длины                             */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("Массив нулевой длины");
    {
        NVS storage = CreateFreshStorage();
        uint8_t dummy = 0;
        assert(storage.SetValue((NVS_Key_t)"empty_arr", &dummy, 0) == 0);
        uint16_t outLen = 0xFF;
        uint8_t *r = storage.GetArray((NVS_Key_t)"empty_arr", &outLen);
        assert(r != nullptr);
        assert(outLen == 0);
    }

    nvs_printf("\r\nAll boundary value tests passed!\r\n");
    return 0;
}