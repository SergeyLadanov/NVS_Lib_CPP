#include "TestInfrastructure.hpp"

int main(void)
{
    TEST_SECTION("Модульные тесты NVS_Page");

    NVS_Page page;
    page.Init();

    assert(page.IsReady()      == true);
    assert(page.IsCleared()    == false);
    assert(page.IsMarkAsUsed() == false);

    page.SetNumber(42);
    assert(page.GetNumber() == 42);

    uint32_t hdrSize = page.GetHeaderSize();
    assert(hdrSize == sizeof(NVS_Page::Header_t));
    nvs_printf("  Page header size: {}\r\n", hdrSize);

    // Проверка IsReadyToErase
    assert(page.IsReadyToErase() == false);

    page.SetState(NVS_Page::STATE_RELEASED);
    assert(page.IsReadyToErase() == true);
    assert(page.IsMarkAsUsed()   == true);

    nvs_printf("\r\nAll NVS_Page unit tests passed!\r\n");
    return 0;
}