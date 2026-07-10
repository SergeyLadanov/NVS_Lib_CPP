#include "TestInfrastructure.hpp"

int main(void)
{
    TEST_SECTION("Тестирование сохранности после Reset (инициализации)");

    FlashIf.Reset();
    InitFlashDesc();
    NVS Storage(FlashIf);
    Storage.Init(FlashDesc, 2);

    // Записываем несколько значений
    Storage.SetValue((NVS_Key_t)"pers_u16", (uint16_t)12345);
    Storage.SetValue((NVS_Key_t)"pers_i8",  (int8_t)-99);
    Storage.SetValue((NVS_Key_t)"pers_str", (char *)"persistent!");

    // Создаём новое хранилище на тех же буферах (имитация перезагрузки)
    NVS Restored(FlashIf);
    Restored.Init(FlashDesc, 2);

    // Проверяем, что значения сохранились
    uint16_t u16val = 0;
    assert(Restored.GetValue((NVS_Key_t)"pers_u16", u16val) == 0);
    assert(u16val == 12345);

    int8_t i8val = 0;
    assert(Restored.GetValue((NVS_Key_t)"pers_i8", i8val) == 0);
    assert(i8val == -99);

    char *strVal = Restored.GetString((NVS_Key_t)"pers_str");
    assert(strVal != nullptr);
    assert(strcmp(strVal, "persistent!") == 0);

    printf("\r\nAll persistence tests passed!\r\n");
    return 0;
}