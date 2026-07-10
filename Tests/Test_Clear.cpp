#include "TestInfrastructure.hpp"

int main(void)
{
    TEST_SECTION("Тестирование Clear");

    // Используем прямой объект, а не CreateFreshStorage(),
    // чтобы избежать проблем с копированием объекта, содержащего ссылку
    FlashIf.Reset();
    InitFlashDesc();
    NVS storage(FlashIf);
    storage.Init(FlashDesc, 2);

    // Записываем несколько значений
    storage.SetValue((NVS_Key_t)"clr1", (uint16_t)10);
    storage.SetValue((NVS_Key_t)"clr2", (uint16_t)20);
    storage.SetValue((NVS_Key_t)"clr3", (uint16_t)30);

    // Очищаем
    storage.Clear();

    // После очистки значения не должны читаться
    uint16_t val = 0;
    assert(storage.GetValue((NVS_Key_t)"clr1", val) != 0);
    assert(storage.GetValue((NVS_Key_t)"clr2", val) != 0);
    assert(storage.GetValue((NVS_Key_t)"clr3", val) != 0);

    // После очистки можно писать новые значения
    assert(storage.SetValue((NVS_Key_t)"new_clr", (uint16_t)999) == 0);
    uint16_t newVal = 0;
    assert(storage.GetValue((NVS_Key_t)"new_clr", newVal) == 0);
    assert(newVal == 999);

    printf("\r\nAll Clear tests passed!\r\n");
    return 0;
}