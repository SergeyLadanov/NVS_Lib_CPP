#include "NVS.hpp"

NVS::NVS(NVS_IFlash &flash_if, FlashDesc_t *flash_desc, uint32_t len)
    : FlashDescriptors(flash_desc), FlashDescriptorsSize(len), FlashInterface(flash_if)
{

}


uint32_t NVS::GetPageFreeSpace(void)
{
    return FlashDescriptors[GetCurrentIndex()].Size - GetUsedBytes();
}


uint32_t NVS::GetUsedBytes(void)
{
    return CurrentPageUsedBytes;
}

uint32_t NVS::ScanUsedBytes(void)
{
    uint32_t res = 0;
    NVS_Page *Page = (NVS_Page *) FlashDescriptors[GetCurrentIndex()].MemPtr;
    NVS_Cell *Cell = (NVS_Cell *) Page->GetData();

    res = Page->GetHeaderSize();

    while (!Cell->IsEmpty())
    {
        res += Cell->GetTotalSize();

        if (res >= FlashDescriptors[GetCurrentIndex()].Size)
        {
            break;
        }

        Cell = Cell->GetNext();
    }
    
    return res;
}



int32_t NVS::ScanWriteNumber(void)
{
    NVS_Page *Page = nullptr;

    int32_t MaxPage = -1;  
    
    for (uint32_t i = 0; i < FlashDescriptorsSize; i++)
    {
        Page = (NVS_Page *) FlashDescriptors[i].MemPtr;

        if (Page->IsReady())
        {
            if ((int32_t) Page->GetNumber() > MaxPage)
            {
                MaxPage = (int32_t) Page->GetNumber();
            }   
        }
    }

    return MaxPage;
}


uint32_t NVS::GetNewPageIndex(void)
{
    return ((GetCurrentIndex() + 1) % FlashDescriptorsSize);
}


uint32_t NVS::GetCurrentIndex(void)
{
    return (WriteNumber % FlashDescriptorsSize);
}


void NVS::PagePrepare(uint32_t index, uint32_t number)
{
    NVS_Page Page;
    Page.Init();
    Page.SetNumber(number);
    FlashInterface.WriteData(FlashDescriptors[index].MemPtr, (uint8_t *) &Page, sizeof(NVS_Page));
}


void NVS::PageErase(uint32_t index)
{
    FlashInterface.PageErase(FlashDescriptors[index].MemPtr, FlashDescriptors[index].Sector, FlashDescriptors[index].Size);
}


void NVS::Init(FlashDesc_t *flash_desc, uint32_t len)
{

    int32_t LastNumTemp = -1;
    
    if (flash_desc)
    {
        FlashDescriptors = flash_desc;
    }

    if (len)
    {
        FlashDescriptorsSize = len;
    }


    LastNumTemp = ScanWriteNumber();
    
    if (LastNumTemp != -1)
    {
        NVS_LOG("NVS storage is not empty!\r\n");
        WriteNumber = LastNumTemp;
        NVS_LOG("Last page index: %d\r\n", GetCurrentIndex());
    }
    else
    {
        NVS_LOG("NVS storage is empty!\r\n");
        NVS_LOG("Initialization of  first storage page...\r\n");
        PageErase(GetCurrentIndex());
        PagePrepare(GetCurrentIndex(), WriteNumber);
    }

    CurrentPageUsedBytes = ScanUsedBytes();

    NVS_LOG("Current settings page: %d\r\n", GetCurrentIndex());
    NVS_LOG("Using bytes: %d\r\n", GetUsedBytes());
}


void NVS::CopyItem(NVS_Cell *cell_src, NVS_Cell *cell_dst)
{
    FlashInterface.WriteData((uint8_t *) cell_dst, (uint8_t *) cell_src, cell_src->GetTotalSize());
}


void NVS::ReleaseCurrentPage(void)
{
    uint32_t NewWriteNumber = WriteNumber + 1;

    PageErase(GetNewPageIndex());

    NVS_Page *SrcPage = (NVS_Page *) FlashDescriptors[GetCurrentIndex()].MemPtr;
    NVS_Page *DstPage = (NVS_Page *) FlashDescriptors[GetNewPageIndex()].MemPtr;


    NVS_Cell *SrcCell = (NVS_Cell *) SrcPage->GetData();
    NVS_Cell *DstCell = (NVS_Cell *) DstPage->GetData();

    uint32_t CurrentPageNewState = NVS_Page::STATE_RELEASED;

    

    uint32_t Bytes = SrcPage->GetHeaderSize();


    while ((!SrcCell->IsEmpty()) && (Bytes < CurrentPageUsedBytes))
    {

        if (SrcCell->State == NVS_Cell::STATE_VALID)
        {
            CopyItem(SrcCell, DstCell);
            ReleaseCell(SrcCell);
            DstCell = DstCell->GetNext();
        }

        Bytes += SrcCell->GetTotalSize();

        SrcCell = SrcCell->GetNext();
    }

    PagePrepare(GetNewPageIndex(), NewWriteNumber);

    FlashInterface.WriteData((uint8_t *) &SrcPage->Header.State, (uint8_t *) &CurrentPageNewState, sizeof(CurrentPageNewState));

    WriteNumber = NewWriteNumber;

    CurrentPageUsedBytes = ScanUsedBytes();

    NVS_LOG("Used bytes: %d\r\n", CurrentPageUsedBytes);
}



void NVS::WriteCell(NVS_Cell &new_cell, const char *key)
{

    if ((FlashDescriptors[GetCurrentIndex()].Size - CurrentPageUsedBytes) < new_cell.GetTotalSize())
    {
        NVS_LOG("Page is full\r\n");
        NVS_LOG("Migrate to new page...\r\n");

        ReleaseCurrentPage();
    }

    NVS_Page *Page = (NVS_Page *) FlashDescriptors[GetCurrentIndex()].MemPtr;
    NVS_Cell *Cell = (NVS_Cell *) Page->GetData();


    while (!Cell->IsEmpty())
    {
        if (Cell->IsKey(key))
        {
            ReleaseCell(Cell);
        }
        Cell = Cell->GetNext();
    }


    FlashInterface.WriteData((uint8_t *) Cell, (uint8_t *) &new_cell, new_cell.GetTotalSize());
    new_cell.State = new_cell.STATE_VALID;
    FlashInterface.WriteData((uint8_t *) &Cell->State, (uint8_t *) &new_cell.State, sizeof(new_cell.State));
    CurrentPageUsedBytes += new_cell.GetTotalSize();

    NVS_LOG("Write success!\r\n");
    NVS_LOG("Used bytes: %d\r\n", GetUsedBytes());
    NVS_LOG("Available bytes: %d\r\n", GetPageFreeSpace());
}




void NVS::ReleaseCell(NVS_Cell *cell)
{
    uint32_t NewState = NVS_Cell::STATE_RELEASED;
    FlashInterface.WriteData((uint8_t *) &cell->State, (uint8_t *) &NewState, sizeof(uint32_t));
}



NVS_Cell *NVS::FindCellByKey(const char *key)
{
    NVS_Page *Page = (NVS_Page *) FlashDescriptors[GetCurrentIndex()].MemPtr;
    NVS_Cell *Cell = (NVS_Cell *) Page->GetData();


    uint32_t Bytes = Page->GetHeaderSize();


    while ((!Cell->IsEmpty()) && (Bytes < CurrentPageUsedBytes))
    {
        if (Cell->IsKey(key))
        {
            if (Cell->State == NVS_Cell::STATE_VALID)
            {
                break;
            }
        }

        Bytes += Cell->GetTotalSize();

        Cell = Cell->GetNext();
    }

    return Cell;
}



char *NVS::GetString(const char *key)
{
    NVS_Cell *Cell = FindCellByKey(key);
    
    return Cell->GetString();
}



uint32_t NVS::GetAvaliableSpaceInBytes(void)
{
    uint32_t used = 0;
    NVS_Page *Page = (NVS_Page *) FlashDescriptors[GetCurrentIndex()].MemPtr;
    NVS_Cell *Cell = (NVS_Cell *) Page->GetData();

    used = Page->GetHeaderSize();

    while (!Cell->IsEmpty())
    {
        if (Cell->IsValid())
        {
            used += Cell->GetTotalSize();
        }
        

        if (used >= FlashDescriptors[GetCurrentIndex()].Size)
        {
            break;
        }

        Cell = Cell->GetNext();
    }
    
    return FlashDescriptors[GetCurrentIndex()].Size - used;
}


uint32_t NVS::GetAvaliableSpaceInBlocks(void)
{
    return GetAvaliableSpaceInBytes() / NVS_Cell::MEMORY_CELL_SIZE;
}