#include "TestInfrastructure.hpp"

int main(void)
{
    TEST_SECTION("Тестирование вместимости / пространства");

    uint32_t pageHeaderSize = sizeof(NVS_Page::Header_t);   // 16 bytes
    uint32_t cellSize       = NVS_Cell::MEMORY_CELL_SIZE;    // 32 bytes
    uint32_t dataArea       = PAGE_SIZE - pageHeaderSize;    // 240 bytes
    uint32_t maxCells       = dataArea / cellSize;           // 7 cells

    nvs_printf("  Page header size : {}\r\n", pageHeaderSize);
    nvs_printf("  Cell size        : {}\r\n", cellSize);
    nvs_printf("  Data area        : {}\r\n", dataArea);
    nvs_printf("  Max cells/page   : {}\r\n", maxCells);

    /* -------------------------------------------------- */
    /*  Начальное свободное место                        */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("Начальное свободное место");
    {
        NVS storage = CreateFreshStorage();
        uint32_t availBytes  = storage.GetAvaliableSpaceInBytes();
        uint32_t availBlocks = storage.GetAvaliableSpaceInBlocks();
        nvs_printf("  Available bytes  : {}\r\n", availBytes);
        nvs_printf("  Available blocks : {}\r\n", availBlocks);
        assert(availBytes  == dataArea);
        assert(availBlocks == maxCells);
    }

    /* -------------------------------------------------- */
    /*  Заполнение страницы до предела и миграция        */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("Заполнение страницы до предела + миграция");
    {
        NVS storage = CreateFreshStorage();

        // Записываем maxCells uint16_t значений (каждое = 1 cell)
        for (uint32_t i = 0; i < maxCells; i++)
        {
            char key[16];
            snprintf(key, sizeof(key), "fill_%u", i);
            int8_t res = storage.SetValue((NVS_Key_t)key, (uint16_t)i);
            assert(res == 0);
        }

        uint32_t usedBytes  = PAGE_SIZE - storage.GetAvaliableSpaceInBytes();
        uint32_t expected   = pageHeaderSize + maxCells * cellSize;
        nvs_printf("  Used bytes after fill : {} (expected ~{})\r\n", usedBytes, expected);
        assert(usedBytes >= expected - cellSize);

        // После заполнения страницы свободно 16 байт, а новая ячейка требует 32.
        int8_t overflowRes = storage.SetValue((NVS_Key_t)"overflow", (uint16_t)0xFFFF);
        assert(overflowRes != 0);
        nvs_printf("  Overflow write result: {} (expected <0)\r\n", overflowRes);
    }

    /* -------------------------------------------------- */
    /*  Запись значения, не влезающего в свободное место */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("Запись значения, превышающего доступное место");
    {
        NVS storage = CreateFreshStorage();

        // Сначала заполняем страницу
        for (uint32_t i = 0; i < maxCells; i++)
        {
            char key[16];
            snprintf(key, sizeof(key), "pre_%u", i);
            storage.SetValue((NVS_Key_t)key, (uint8_t)i);
        }

        // Теперь пытаемся записать большой массив — не хватит места
        uint8_t bigBuf[100];
        memset(bigBuf, 0xAA, sizeof(bigBuf));
        int8_t res = storage.SetValue((NVS_Key_t)"big_arr", bigBuf, sizeof(bigBuf));
        assert(res != 0);
        nvs_printf("  Oversized write result: {} (expected <0)\r\n", res);
    }

    /* -------------------------------------------------- */
    /*  GetAvaliableSpaceInBlocks                         */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("GetAvaliableSpaceInBlocks");
    {
        NVS storage = CreateFreshStorage();

        uint32_t blocks = storage.GetAvaliableSpaceInBlocks();
        assert(blocks == maxCells);

        // Записываем одно значение — блоков должно стать на 1 меньше
        storage.SetValue((NVS_Key_t)"block_test", (uint8_t)42);
        uint32_t blocksAfter = storage.GetAvaliableSpaceInBlocks();
        assert(blocksAfter == maxCells - 1);
    }

    /* -------------------------------------------------- */
    /*  Clear и проверка пространства                    */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("Clear и восстановление пространства");
    {
        NVS storage = CreateFreshStorage();

        // Записываем несколько значений
        storage.SetValue((NVS_Key_t)"clr_a", (uint32_t)100);
        storage.SetValue((NVS_Key_t)"clr_b", (uint32_t)200);
        storage.SetValue((NVS_Key_t)"clr_c", (uint32_t)300);

        uint32_t beforeClear = storage.GetAvaliableSpaceInBytes();
        nvs_printf("  Available before clear: {}\r\n", beforeClear);
        assert(beforeClear < dataArea);

        // Очищаем
        storage.Clear();

        uint32_t afterClear = storage.GetAvaliableSpaceInBytes();
        nvs_printf("  Available after clear : {}\r\n", afterClear);
        assert(afterClear == dataArea);
    }

    /* -------------------------------------------------- */
    /*  Перезапись существующего ключа                   */
    /* -------------------------------------------------- */
    TEST_SUBSECTION("Перезапись существующего ключа");
    {
        NVS storage = CreateFreshStorage();

        // Записываем значение
        storage.SetValue((NVS_Key_t)"rewrite", (uint16_t)100);
        uint32_t afterFirst = storage.GetAvaliableSpaceInBytes();

        // Перезаписываем тот же ключ
        storage.SetValue((NVS_Key_t)"rewrite", (uint16_t)200);
        uint32_t afterSecond = storage.GetAvaliableSpaceInBytes();

        nvs_printf("  Available after 1st write: {}\r\n", afterFirst);
        nvs_printf("  Available after 2nd write: {}\r\n", afterSecond);

        // После перезаписи freed-пространство должно учитываться
        assert(afterSecond >= afterFirst);

        // Проверяем, что читается последнее значение
        uint16_t readback = 0;
        storage.GetValue((NVS_Key_t)"rewrite", readback);
        assert(readback == 200);
    }

    nvs_printf("\r\nAll space/capacity tests passed!\r\n");
    return 0;
}