# NVS_Lib_CPP
Библиотека для хранения настроек, используя встроенную FLASH память микроконтроллера

Исходные файлы библиотеки находятся в Components/NVS_Lib


Пример эмуляции в ОЗУ находится в Core/Src/main.cpp

## Пример для STM32F303VCT6

```cpp
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "NVS.hpp"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
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




  if (!Storage.GetValue("test_u16", ProbeU16))
  {
	  printf("Value was found!\r\n");
  }

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

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
```

