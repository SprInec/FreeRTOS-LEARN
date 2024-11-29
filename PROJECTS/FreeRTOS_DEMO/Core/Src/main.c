/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_config.h"
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
static TaskHandle_t AppTaskCreate_Handle = NULL; /* 创建任务句柄 */
static TaskHandle_t LED1_Task_Handle = NULL;      /* LED1_Task 任务句柄 */
static TaskHandle_t LED2_Task_Handle = NULL;      /* LED2_Task 任务句柄 */
static TaskHandle_t CPU_Task_Handle = NULL;        /* CPU_Task 任务句柄 */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void AppTaskCreate(void);              /* 用于创建任务 */
static void LED1_Task(void *pvParameters);     /* LED1_Task 任务实现 */
static void LED2_Task(void *pvParameters);     /* LED2_Task 任务实现 */
static void CPU_Task(void *pvParameters);     /* CPU_Task 任务实现 */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */
    BaseType_t xReturn = pdPASS;
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
    MX_DMA_Init();
    MX_USART1_UART_Init();
    MX_TIM7_Init();
    /* USER CODE BEGIN 2 */
    BSP_LED_Init();
    BSP_KEY_Init();
    BSP_FREERTOS_TIMBASE_Init();
    HAL_TIM_Base_Start_IT(&htim7);

    printf("\n- Program Start -\n");

    xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,
                          "AppTaskCreate",
                          512,
                          NULL,
                          1,
                          &AppTaskCreate_Handle);
    if (pdPASS == xReturn)
    {
        printf("AppTaskCreate task create success!\n");
        vTaskStartScheduler();
    }
    else
        printf("AppTaskCreate task create failed!\n");
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

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 72;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
static void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS;

    taskENTER_CRITICAL();

    xReturn = xTaskCreate((TaskFunction_t)LED1_Task,
                          "LED1_Task",
                          512,
                          NULL,
                          2,
                          &LED1_Task_Handle);
    if (pdPASS == xReturn)
        printf("LED1_Task task create success!\n");

    xReturn = xTaskCreate((TaskFunction_t)LED2_Task,
                          "LED2_Task",
                          512,
                          NULL,
                          3,
                          &LED2_Task_Handle);
    if (pdPASS == xReturn)
        printf("LED2_Task task create success!\n");

    xReturn = xTaskCreate((TaskFunction_t)CPU_Task,
                          "CPU_Task",
                          512,
                          NULL,
                          4,
                          &CPU_Task_Handle);
    if (pdPASS == xReturn)
        printf("CPU_Task task create success!\n");

    vTaskDelete(NULL);

    taskEXIT_CRITICAL();
}

static void LED1_Task(void *pvParameters)
{
    while (1)
    {
        __BSP_LED1_FICKER(100);
        printf("LED1_Task is running!\r\n");
    }
}

static void LED2_Task(void *pvParameters)
{
    while (1)
    {
        __BSP_LED2_FICKER(100);
        printf("LED2_Task is running!\r\n");
    }
}

static void CPU_Task(void *pvParameters)
{
    uint8_t CPU_RunInfo[400];
    while (1)
    {
        memset(CPU_RunInfo, 0, 400);

        // 获取任务运行时间信息
        vTaskList((char *)&CPU_RunInfo);

        printf("-----------------------------------------\r\n");
        printf("任务名  任务状态  优先级  剩余栈  任务序号\r\n");
        printf("%s", CPU_RunInfo);
        printf("-----------------------------------------\r\n");

        memset(CPU_RunInfo, 0, 400);

        vTaskGetRunTimeStats((char *)&CPU_RunInfo);

        printf("任务名  运行计数  使用率\r\n");
        printf("%s", CPU_RunInfo);
        printf("-----------------------------------------\r\n");

        vTaskDelay(1000);
    }
}
/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM2 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM2)
    {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

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

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
