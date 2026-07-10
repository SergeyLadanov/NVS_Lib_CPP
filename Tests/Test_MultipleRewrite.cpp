#include "TestInfrastructure.hpp"

int main(void)
{
    TEST_SECTION("Тестирование многократной перезаписи");

    NVS storage = CreateFreshStorage();

    // Многократно перезаписываем один и тот же ключ
    constexpr uint32_t NUM_WRITES = 50;

    for (uint32_t i = 0; i < NUM_WRITES; i++)
    {
        int8_t res = storage.SetValue((NVS_Key_t)"multi", (uint32_t)i);
        assert(res == 0);
    }

    // Проверяем финальное значение
    uint32_t finalVal = 0;
    assert(storage.GetValue((NVS_Key_t)"multi", finalVal) == 0);
    assert(finalVal == NUM_WRITES - 1);

    printf("  Final value after %u rewrites: %lu\r\n", NUM_WRITES, (unsigned long)finalVal);

    printf("\r\nAll multiple rewrite tests passed!\r\n");
    return 0;
}