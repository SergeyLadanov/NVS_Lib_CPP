
#include "main.hpp"
#include "NVS_Cell.hpp"
#include <cstdio>

uint8_t Buffer[1024];
NVS_Cell::DataBlock_t *Test = (NVS_Cell::DataBlock_t *) Buffer;



// Основная программа
int main(void)
{
    Test->Record.Init(NVS_Cell::TYPE_INT16);
    uint8_t *Bin = Test->GetBlobBlock(0);
    printf("Test");
    return 0;  
}
