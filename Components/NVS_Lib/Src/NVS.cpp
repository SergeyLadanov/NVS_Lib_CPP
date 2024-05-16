#include "NVS.hpp"

NVS::NVS(NVS_IFlash &flash_if, FlashDesc_t *flash_desc, uint32_t len)
    : FlashDescriptors(flash_desc), FlashDescriptorsSize(len), FlashInterface(flash_if)
{

}