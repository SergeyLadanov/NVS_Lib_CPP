# NVS_Lib_CPP
Шаблон проекта C/C++ для Cmake
## Формат данных ячейки
```
start_4, key_14, size_1, type_1, data_8, state_4
```

## Пример для STM32F303VCT6

```cpp
class SettingsFlash_If : public NVS_IFlash
{
private:
    void PageErase(uint8_t *mem_ptr, uint16_t sector, uint32_t sector_size) override
    {
    	FLASH_EraseInitTypeDef EraseDescr =
    	{
    		.TypeErase = FLASH_TYPEERASE_PAGES,
			.PageAddress = (uint32_t ) mem_ptr,
			.NbPages = 1
    	};

    	uint32_t Status = 0;
        //memset(mem_ptr, 0xFF, sector_size);
    	HAL_FLASH_Unlock();

	    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
	    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_WRPERR);
	    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGERR);

    	HAL_FLASHEx_Erase(&EraseDescr, &Status);
    	HAL_FLASH_Lock();
    }


    void WriteData(uint8_t * mem_ptr, uint8_t *data_ptr, uint32_t len) override
    {
    	HAL_FLASH_Unlock();
    	len = len/4;
    	uint32_t *U32_data = (uint32_t *) data_ptr;
    	uint32_t *U32_Mem = (uint32_t *) mem_ptr;
    	for (uint32_t i = 0; i < len; i++)
    	{
    		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t) &U32_Mem[i], (uint64_t) U32_data[i]);
    	}

    	HAL_FLASH_Lock();
    }
public:
    SettingsFlash_If()
    {
//        memset(PageBuffer1, 0xFF, sizeof(PageBuffer1));
//        memset(PageBuffer2, 0xFF, sizeof(PageBuffer2));
    }
};


static const NVS::FlashDesc_t FlashDescriptor[] =
{
    {.MemPtr = (uint8_t *) 0x0803F000, .Size = 2048, .Sector = 126},
    {.MemPtr = (uint8_t *) 0x0803F800, .Size = 2048, .Sector = 127},
};

static SettingsFlash_If FlashInterface;
static NVS Storage(FlashInterface);


static uint16_t ProbeU16 = 0x35;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  Storage.Init((NVS::FlashDesc_t *) FlashDescriptor, 2);




  ProbeU16 = Storage.GetValue<typeof(ProbeU16)>("test_u16");

  ProbeU16++;

  if (!Storage.SetValue("test_u16", (uint16_t) (ProbeU16)))
  {
	printf("Write u16 success!\r\n");
  }
  else
  {
	printf("Write u16 failed!\r\n");
  }


  HAL_Delay(2000);

  NVIC_SystemReset();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
```

