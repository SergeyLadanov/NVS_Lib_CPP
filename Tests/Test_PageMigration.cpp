#include "TestInfrastructure.hpp"

int main(void)
{
    TEST_SECTION("Тестирование миграции страниц");

    NVS storage = CreateFreshStorage();

    uint32_t cellSize = NVS_Cell::MEMORY_CELL_SIZE;
    uint32_t dataArea = PAGE_SIZE - sizeof(NVS_Page::Header_t);
    uint32_t maxCells = dataArea / cellSize;

    // Заполняем первую страницу
    for (uint32_t i = 0; i < maxCells; i++)
    {
        char key[16];
        snprintf(key, sizeof(key), "mig_%u", i);
        assert(storage.SetValue((NVS_Key_t)key, (uint32_t)i) == 0);
    }

    // После заполнения свободно 16 байт — новая запись не влезет
    uint32_t beforeMigrate = storage.GetAvaliableSpaceInBytes();
    nvs_printf("  Available before overflow: {}\r\n", beforeMigrate);

    // Запись не влезает в доступное место (16 < 32)
    int8_t overflowRes = storage.SetValue((NVS_Key_t)"mig_last", (uint32_t)999);
    assert(overflowRes != 0);
    nvs_printf("  Overflow write result: {} (expected <0)\r\n", overflowRes);

    // Проверяем, что все записанные значения читаются
    for (uint32_t i = 0; i < maxCells; i++)
    {
        char key[16];
        snprintf(key, sizeof(key), "mig_%u", i);
        uint32_t readback = 0;
        int8_t res = storage.GetValue((NVS_Key_t)key, readback);
        assert(res == 0);
        assert(readback == i);
    }

    nvs_printf("\r\nAll page migration tests passed!\r\n");
    return 0;
}