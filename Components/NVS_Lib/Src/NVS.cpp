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

    WriteNumber = 0;
    
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


void NVS::RemoveValue(NVS_Key_t key)
{
    NVS_Page *Page = (NVS_Page *) FlashDescriptors[GetCurrentIndex()].MemPtr;
    NVS_Cell *Cell = (NVS_Cell *) Page->GetData();
    uint32_t Bytes = Page->GetHeaderSize();


    while ((!Cell->IsEmpty()) && (Bytes < CurrentPageUsedBytes))
    {

        if (Cell->State == NVS_Cell::STATE_VALID)
        {
            ReleaseCell(Cell);
        }

        Bytes += Cell->GetTotalSize();

        Cell = Cell->GetNext();
    }
    
}

void NVS::ReleaseCurrentPage(void)
{
    uint32_t NewWriteNumber = WriteNumber + 1;

    PageErase(GetNewPageIndex());

    NVS_Page *SrcPage = (NVS_Page *) FlashDescriptors[GetCurrentIndex()].MemPtr;
    NVS_Page *DstPage = (NVS_Page *) FlashDescriptors[GetNewPageIndex()].MemPtr;


    NVS_Cell *SrcCell = (NVS_Cell *) SrcPage->GetData();
    NVS_Cell *DstCell = (NVS_Cell *) DstPage->GetData();

    uint32_t Bytes = SrcPage->GetHeaderSize();


    while ((!SrcCell->IsEmpty()) && (Bytes < CurrentPageUsedBytes))
    {

        if (SrcCell->State == NVS_Cell::STATE_VALID)
        {
            CopyItem(SrcCell, DstCell);
            //ReleaseCell(SrcCell);
            DstCell = DstCell->GetNext();
        }

        Bytes += SrcCell->GetTotalSize();

        SrcCell = SrcCell->GetNext();
    }

    PagePrepare(GetNewPageIndex(), NewWriteNumber);

    ReleasePage(SrcPage);

    WriteNumber = NewWriteNumber;

    CurrentPageUsedBytes = ScanUsedBytes();

    NVS_LOG("Used bytes: %d\r\n", CurrentPageUsedBytes);
}



int8_t NVS::WriteCell(NVS_Cell &new_cell, NVS_Key_t key)
{
    
    NVS_LOG("Required size: %d\r\n", new_cell.GetTotalSize());
    NVS_LOG("Avalible size: %d\r\n", GetAvaliableSpaceInBytes());

    if (new_cell.GetTotalSize() > GetAvaliableSpaceInBytes())
    {
        return -1;
    }

    if ((FlashDescriptors[GetCurrentIndex()].Size - CurrentPageUsedBytes) < new_cell.GetTotalSize())
    {
        NVS_LOG("Page is full\r\n");
        NVS_LOG("Migrate to new page...\r\n");

        ReleaseCurrentPage();
    }

    NVS_Page *Page = (NVS_Page *) FlashDescriptors[GetCurrentIndex()].MemPtr;
    NVS_Cell *Cell = (NVS_Cell *) Page->GetData();
    NVS_Cell *PreviousCell = nullptr;


    while (!Cell->IsEmpty())
    {
        if (Cell->IsKey(key))
        {
            PreviousCell = Cell;
        }
        Cell = Cell->GetNext();
    }


    FlashInterface.WriteData((uint8_t *) Cell, (uint8_t *) &new_cell, new_cell.GetTotalSize());
    new_cell.State = new_cell.STATE_VALID;
    FlashInterface.WriteData((uint8_t *) &Cell->State, (uint8_t *) &new_cell.State, sizeof(new_cell.State));
    CurrentPageUsedBytes += new_cell.GetTotalSize();

    if (PreviousCell)
    {
        ReleaseCell(PreviousCell);
    }

    NVS_LOG("Write success!\r\n");
    NVS_LOG("Used bytes: %d\r\n", GetUsedBytes());
    NVS_LOG("Available bytes: %d\r\n", GetPageFreeSpace());

    return 0;
}




void NVS::ReleaseCell(NVS_Cell *cell)
{
    NVS_State_t NewState = NVS_Cell::STATE_RELEASED;
    FlashInterface.WriteData((uint8_t *) &cell->State, (uint8_t *) &NewState, sizeof(NewState));
}



void NVS::ReleasePage(NVS_Page *page)
{
    NVS_State_t NewState = NVS_Cell::STATE_RELEASED;
    FlashInterface.WriteData((uint8_t *) &page->Header.State, (uint8_t *) &NewState, sizeof(NewState));
}


NVS_Cell *NVS::FindCellByKey(NVS_Key_t key)
{
    NVS_Page *Page = (NVS_Page *) FlashDescriptors[GetCurrentIndex()].MemPtr;
    NVS_Cell *Cell = (NVS_Cell *) Page->GetData();


    uint32_t Bytes = Page->GetHeaderSize();

    bool status = false;


    while ((!Cell->IsEmpty()) && (Bytes < CurrentPageUsedBytes))
    {
        if (Cell->IsKey(key))
        {
            if (Cell->State == NVS_Cell::STATE_VALID)
            {
                status = true;
                break;
            }
        }

        Bytes += Cell->GetTotalSize();

        Cell = Cell->GetNext();
    }

    if (!status)
    {
        Cell = nullptr;
    }

    return Cell;
}



char *NVS::GetString(NVS_Key_t key)
{
    NVS_Cell *Cell = FindCellByKey(key);

    if ((!Cell) || (Cell->Header.Type != NVS_Cell::TYPE_ARRAY))
    {
        return nullptr;
    }
    
    return Cell->GetString();
}



int8_t NVS::SetValue(NVS_Key_t key, uint8_t *buf, uint16_t len)
{
    NVS_Cell Data;

    Data.Init(key);
    Data.SetValue(buf, len);

    return WriteCell(Data, key);
}


uint8_t *NVS::GetArray(NVS_Key_t key, uint16_t *out_size)
{
    NVS_Cell *Cell = FindCellByKey(key);


    if ((!Cell) || (Cell->Header.Type != NVS_Cell::TYPE_ARRAY))
    {
        return nullptr;
    }

    return Cell->GetArray(out_size);
}



int8_t NVS::GetArray(NVS_Key_t key, uint8_t *out_buf, uint16_t *out_size)
{
    NVS_Cell *Cell = FindCellByKey(key);
    uint16_t BlobSize = 0;

    if ((!Cell) || (Cell->Header.Type != NVS_Cell::TYPE_ARRAY))
    {
        return -1;
    }

    uint8_t *blob = Cell->GetArray(&BlobSize);

    if (out_size)
    {
        *out_size = BlobSize;
    }

    memcpy(out_buf, blob, BlobSize);


    return 0;
}


uint32_t NVS::GetAvaliableSpaceInBytes(void)
{
    uint32_t used = 0;
    NVS_Page *Page = (NVS_Page *) FlashDescriptors[GetCurrentIndex()].MemPtr;
    NVS_Cell *Cell = (NVS_Cell *) Page->GetData();

    used = Page->GetHeaderSize();

    while (!Cell->IsEmpty())
    {
        if (!Cell->IsCleared())
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



void NVS::Clear(void)
{
    for (uint32_t i = 0; i < FlashDescriptorsSize; i++)
    {
        FlashInterface.PageErase(FlashDescriptors[i].MemPtr, FlashDescriptors[i].Sector, FlashDescriptors[i].Size);
    }
    
    Init(FlashDescriptors, FlashDescriptorsSize);
}