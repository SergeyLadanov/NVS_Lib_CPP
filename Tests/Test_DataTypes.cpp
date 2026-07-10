#include "TestInfrastructure.hpp"

int main(void)
{
    TEST_SECTION("Тестирование всех типов данных");

    /* -------------------------------------------------- */
    /*  int8_t                                           */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("int8_t");
    {
        NVS storage = CreateFreshStorage();
        int8_t  written = -42;
        int8_t  readback = 0;
        assert(storage.SetValue(KeyI8, written) == 0);
        assert(storage.GetValue(KeyI8, readback) == 0);
        assert(readback == written);
    }

    /* -------------------------------------------------- */
    /*  int16_t                                          */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("int16_t");
    {
        NVS storage = CreateFreshStorage();
        int16_t written = -12345;
        int16_t readback = 0;
        assert(storage.SetValue(KeyI16, written) == 0);
        assert(storage.GetValue(KeyI16, readback) == 0);
        assert(readback == written);
    }

    /* -------------------------------------------------- */
    /*  int32_t  (if supported)                          */
    /* -------------------------------------------------- */
#if NVS_CONF_MAXSIZE_OF_CELL_VALUES >= 4
    TEST_SUBSECTION("int32_t");
    {
        NVS storage = CreateFreshStorage();
        int32_t written = -2000000000;
        int32_t readback = 0;
        assert(storage.SetValue(KeyI32, written) == 0);
        assert(storage.GetValue(KeyI32, readback) == 0);
        assert(readback == written);
    }
#endif

    /* -------------------------------------------------- */
    /*  int64_t  (if supported)                          */
    /* -------------------------------------------------- */
#if NVS_CONF_MAXSIZE_OF_CELL_VALUES >= 8
    TEST_SUBSECTION("int64_t");
    {
        NVS storage = CreateFreshStorage();
        int64_t written = -9000000000000000000LL;
        int64_t readback = 0;
        assert(storage.SetValue(KeyI64, written) == 0);
        assert(storage.GetValue(KeyI64, readback) == 0);
        assert(readback == written);
    }
#endif

    /* -------------------------------------------------- */
    /*  uint8_t                                          */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("uint8_t");
    {
        NVS storage = CreateFreshStorage();
        uint8_t written = 200;
        uint8_t readback = 0;
        assert(storage.SetValue(KeyUI8, written) == 0);
        assert(storage.GetValue(KeyUI8, readback) == 0);
        assert(readback == written);
    }

    /* -------------------------------------------------- */
    /*  uint16_t                                         */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("uint16_t");
    {
        NVS storage = CreateFreshStorage();
        uint16_t written = 60000;
        uint16_t readback = 0;
        assert(storage.SetValue(KeyUI16, written) == 0);
        assert(storage.GetValue(KeyUI16, readback) == 0);
        assert(readback == written);
    }

    /* -------------------------------------------------- */
    /*  uint32_t  (if supported)                         */
    /* -------------------------------------------------- */
#if NVS_CONF_MAXSIZE_OF_CELL_VALUES >= 4
    TEST_SUBSECTION("uint32_t");
    {
        NVS storage = CreateFreshStorage();
        uint32_t written = 4000000000U;
        uint32_t readback = 0;
        assert(storage.SetValue(KeyUI32, written) == 0);
        assert(storage.GetValue(KeyUI32, readback) == 0);
        assert(readback == written);
    }
#endif

    /* -------------------------------------------------- */
    /*  uint64_t  (if supported)                         */
    /* -------------------------------------------------- */
#if NVS_CONF_MAXSIZE_OF_CELL_VALUES >= 8
    TEST_SUBSECTION("uint64_t");
    {
        NVS storage = CreateFreshStorage();
        uint64_t written = 18000000000000000000ULL;
        uint64_t readback = 0;
        assert(storage.SetValue(KeyUI64, written) == 0);
        assert(storage.GetValue(KeyUI64, readback) == 0);
        assert(readback == written);
    }
#endif

    /* -------------------------------------------------- */
    /*  float  (if supported)                            */
    /* -------------------------------------------------- */
#if NVS_CONF_MAXSIZE_OF_CELL_VALUES >= 4
    TEST_SUBSECTION("float");
    {
        NVS storage = CreateFreshStorage();
        float written = 3.14159265f;
        float readback = 0.0f;
        assert(storage.SetValue(KeyFloat, written) == 0);
        assert(storage.GetValue(KeyFloat, readback) == 0);
        assert(readback == written);
    }
#endif

    /* -------------------------------------------------- */
    /*  double  (if supported)                           */
    /* -------------------------------------------------- */
#if NVS_CONF_MAXSIZE_OF_CELL_VALUES >= 8
    TEST_SUBSECTION("double");
    {
        NVS storage = CreateFreshStorage();
        double written = 3.141592653589793;
        double readback = 0.0;
        assert(storage.SetValue(KeyDouble, written) == 0);
        assert(storage.GetValue(KeyDouble, readback) == 0);
        assert(readback == written);
    }
#endif

    /* -------------------------------------------------- */
    /*  bool                                             */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("bool");
    {
        NVS storage = CreateFreshStorage();
        bool written = true;
        bool readback = false;
        assert(storage.SetValue(KeyBool, written) == 0);
        assert(storage.GetValue(KeyBool, readback) == 0);
        assert(readback == true);

        written = false;
        readback = true;
        assert(storage.SetValue(KeyBool, written) == 0);
        assert(storage.GetValue(KeyBool, readback) == 0);
        assert(readback == false);
    }

    /* -------------------------------------------------- */
    /*  строка (char*)                                   */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("string (char*)");
    {
        NVS storage = CreateFreshStorage();
        const char *written = "Hello, NVS World!";
        assert(storage.SetValue(KeyString, (char *)written) == 0);

        char *readback = storage.GetString(KeyString);
        assert(readback != nullptr);
        assert(strcmp(readback, written) == 0);
    }

    /* -------------------------------------------------- */
    /*  массив (uint8_t*)                                */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("array (uint8_t*)");
    {
        NVS storage = CreateFreshStorage();
        uint8_t written[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                             0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
        uint16_t writtenLen = sizeof(written);

        assert(storage.SetValue(KeyArray, written, writtenLen) == 0);

        // Чтение с копированием в буфер
        uint8_t readbuf[32];
        uint16_t readLen = 0;
        assert(storage.GetArray(KeyArray, readbuf, &readLen) == 0);
        assert(readLen == writtenLen);
        assert(memcmp(readbuf, written, readLen) == 0);

        // Чтение с получением указателя
        uint16_t ptrLen = 0;
        uint8_t *ptr = storage.GetArray(KeyArray, &ptrLen);
        assert(ptr != nullptr);
        assert(ptrLen == writtenLen);
        assert(memcmp(ptr, written, ptrLen) == 0);
    }

    /* -------------------------------------------------- */
    /*  несуществующий ключ                              */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("несуществующий ключ");
    {
        NVS storage = CreateFreshStorage();
        uint16_t dummy = 0;
        assert(storage.GetValue(KeyNone, dummy) != 0);
        assert(storage.GetString(KeyNone) == nullptr);
        assert(storage.GetArray(KeyNone) == nullptr);
    }

    printf("\r\nAll data type tests passed!\r\n");
    return 0;
}