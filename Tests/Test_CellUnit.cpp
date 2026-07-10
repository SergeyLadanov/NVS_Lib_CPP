#include "TestInfrastructure.hpp"

int main(void)
{
    TEST_SECTION("Модульные тесты NVS_Cell");

    /* -------------------------------------------------- */
    /*  Размеры структур                                 */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("Размеры структур");
    {
        nvs_printf("  sizeof(NVS_Cell::Header_t) = {}\r\n", sizeof(NVS_Cell::Header_t));
        nvs_printf("  sizeof(NVS_Cell::State)    = {}\r\n", sizeof(NVS_State_t));
        nvs_printf("  NVS_Cell::MEMORY_CELL_SIZE = {}\r\n", NVS_Cell::MEMORY_CELL_SIZE);
        nvs_printf("  NVS_Cell::MAX_BINARY_BYTES = {}\r\n", NVS_Cell::MAX_BINARY_BYTES);
        nvs_printf("  sizeof(NVS_Page::Header_t) = {}\r\n", sizeof(NVS_Page::Header_t));

        // Проверка выравнивания: Header_t должен быть кратен NVS_State_t
        assert((sizeof(NVS_Cell::Header_t) % sizeof(NVS_State_t)) == 0);
    }

    /* -------------------------------------------------- */
    /*  Init / IsEmpty / IsValid / IsCleared             */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("Init / IsEmpty / IsValid / IsCleared");
    {
        NVS_Cell cell;

        cell.Init("unit_key");
        assert(cell.IsEmpty()   == false);
        assert(cell.IsValid()   == false);
        assert(cell.IsCleared() == false);

        // После установки значения и состояния STATE_VALID
        cell.SetValue((uint16_t)1234);
        cell.State = NVS_Cell::STATE_VALID;

        assert(cell.IsEmpty()   == false);
        assert(cell.IsValid()   == true);
        assert(cell.IsCleared() == false);

        // После освобождения
        cell.State = NVS_Cell::STATE_RELEASED;
        assert(cell.IsCleared() == true);
    }

    /* -------------------------------------------------- */
    /*  SetValue / GetValue для разных типов             */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("SetValue / GetValue (cell-level)");
    {
        NVS_Cell cell;
        cell.Init("val_key");

        // int8
        cell.SetValue((int8_t)-12);
        assert(cell.GetValue<int8_t>()  == -12);

        // uint16
        cell.SetValue((uint16_t)0xAABB);
        assert(cell.GetValue<uint16_t>() == 0xAABB);

#if NVS_CONF_MAXSIZE_OF_CELL_VALUES >= 4
        // float
        cell.SetValue(2.71828f);
        float f = cell.GetValue<float>();
        assert((f > 2.718f) && (f < 2.719f));
#endif

#if NVS_CONF_MAXSIZE_OF_CELL_VALUES >= 8
        // double
        cell.SetValue(1.618033988749894);
        double d = cell.GetValue<double>();
        assert((d > 1.618) && (d < 1.619));
#endif

        // bool
        cell.SetValue(true);
        assert(cell.GetValue<bool>() == true);
        cell.SetValue(false);
        assert(cell.GetValue<bool>() == false);
    }

    /* -------------------------------------------------- */
    /*  SetValue для строки                              */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("SetValue string (cell-level)");
    {
        NVS_Cell cell;
        cell.Init("str_key");

        const char *testStr = "NVS_Test";
        cell.SetValue((char *)testStr);

        char *stored = cell.GetString();
        assert(stored != nullptr);
        assert(strcmp(stored, testStr) == 0);
    }

    /* -------------------------------------------------- */
    /*  SetValue для массива                             */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("SetValue array (cell-level)");
    {
        NVS_Cell cell;
        cell.Init("arr_key");

        uint8_t data[] = {10, 20, 30, 40, 50};
        cell.SetValue(data, sizeof(data));

        uint16_t outSize = 0;
        uint8_t *stored = cell.GetArray(&outSize);
        assert(stored != nullptr);
        assert(outSize == sizeof(data));
        assert(memcmp(stored, data, outSize) == 0);
    }

    /* -------------------------------------------------- */
    /*  GetTotalSize / GetBlockCount                     */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("GetTotalSize / GetBlockCount");
    {
        NVS_Cell cell;
        cell.Init("size_key");

        assert(cell.GetTotalSize() == NVS_Cell::MEMORY_CELL_SIZE);

        cell.SetValue((uint8_t)42);
        cell.State = NVS_Cell::STATE_VALID;
        assert(cell.GetTotalSize() == NVS_Cell::MEMORY_CELL_SIZE);
        assert(cell.GetBinaryBlockCount() == 0);
    }

    /* -------------------------------------------------- */
    /*  IsKey                                            */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("IsKey");
    {
        NVS_Cell cell;
        cell.Init("my_key");

        assert(cell.IsKey("my_key")   == true);
        assert(cell.IsKey("other")    == false);
    }

    nvs_printf("\r\nAll NVS_Cell unit tests passed!\r\n");
    return 0;
}