
#include "main.hpp"
#include "NVS_Cell.hpp"
#include <cstdio>

uint8_t Buffer[1024];
NVS_Cell *Test = (NVS_Cell *) Buffer;

NVS_Cell *Next;

// Основная программа
int main(void)
{
    Test->Init(NVS_Cell::TYPE_INT16, "test");
    Next = Test->GetNext();
    // Test->Record.Init(NVS_Cell::TYPE_INT16);
    // uint8_t *Bin = Test->GetBlobBlock(0);
    // printf("Test");
    return 0;  
}
