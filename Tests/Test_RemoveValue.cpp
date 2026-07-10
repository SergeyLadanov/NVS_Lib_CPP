#include "TestInfrastructure.hpp"

int main(void)
{
    TEST_SECTION("Тестирование RemoveValue");

    NVS storage = CreateFreshStorage();

    // Записываем значение
    storage.SetValue((NVS_Key_t)"rem_test", (uint16_t)777);

    uint16_t before = 0;
    assert(storage.GetValue((NVS_Key_t)"rem_test", before) == 0);
    assert(before == 777);

    // Удаляем
    storage.RemoveValue((NVS_Key_t)"rem_test");

    // После удаления значение не должно читаться
    uint16_t after = 0;
    assert(storage.GetValue((NVS_Key_t)"rem_test", after) != 0);

    nvs_printf("\r\nAll RemoveValue tests passed!\r\n");
    return 0;
}