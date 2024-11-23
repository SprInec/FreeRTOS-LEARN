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
#define QUEUE_LEN 4
#define QUEUE_SIZE 4
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static TaskHandle_t APPTaskCreate_Handle = NULL;
static TaskHandle_t LowPriority_Taks_Handle = NULL;
static TaskHandle_t MidPriority_Task_Handle = NULL;
static TaskHandle_t HighPriority_Task_Handle = NULL;

SemaphoreHandle_t MuxSem_Handle = NULL;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void AppTaskCreate(void); // 用于创建任务

static void LowPriority_Taks(void *pvParameters); 
static void MidPriority_Task(void *pvParameters);
static void HighPriority_Task(void *pvParameters);
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */
    BaseType_t xRetrn = pdPASS;
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
    MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */
    BSP_LED_Init();
    BSP_KEY_Init();

    printf("- programe start!\n");

    xRetrn = xTaskCreate((TaskFunction_t)AppTaskCreate,
                         "AppTaskCreate",
                         512,
                         NULL,
                         1,
                         &APPTaskCreate_Handle);
    if (pdPASS == xRetrn)
        vTaskStartScheduler();
    else {
        printf("ERROR: app create task create failed!\n");
        return -1;
   }
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
    // 定义一个创建信息返回值, 默认为 pdPASS
    BaseType_t xReturn = pdPASS;

    taskENTER_CRITICAL(); // 进入临界区

    /* 创建互斥量 */
    MuxSem_Handle = xSemaphoreCreateMutex();
    if (NULL != MuxSem_Handle)
        printf("MuxSem_Handle 互斥量创建成功 !\n");

    // 给出互斥量
    xReturn = xSemaphoreGive(MuxSem_Handle);

    xReturn = xTaskCreate((TaskFunction_t)LowPriority_Taks,
                          "LowPriority Task",
                          512,
                          NULL,
                          3,
                          &LowPriority_Taks_Handle);
    if (pdPASS == xReturn)
        printf("Create Lowrienty Task 成功!\n");

    xReturn = xTaskCreate((TaskFunction_t)MidPriority_Task,
                          "MidPriority Task",
                          512,
                          NULL,
                          4,
                          &MidPriority_Task_Handle);

    if (pdPASS == xReturn)
        printf("Create Midpriority Task 成功!\n");

    xReturn = xTaskCreate((TaskFunction_t)HighPriority_Task,
                          "HighPriority Task",
                          512,
                          NULL,
                          5,
                          &HighPriority_Task_Handle);
    
    if (pdPASS == xReturn)
        printf("Create Highpriority Task successfully!\n\n");

    vTaskDelete(NULL);

    taskEXIT_CRITICAL();
}

static void LowPriority_Taks(void *pvParameters)
{
    static uint32_t i;
    BaseType_t xReturn = pdPASS;
    while(1)
    {
        printf("LowPriority Task GET the MuxSem...\n");
        xReturn = xSemaphoreTake(MuxSem_Handle,
                                 portMAX_DELAY);
        if (xReturn == pdTRUE)
            printf("LowPriority Task GOT the MuxSem Successfully!\n");

        // 模拟低优先级任务占用信号量
        for (i = 0; i < 2000000; i++)
        {
            taskYIELD();
        }

        // 释放互斥量
        printf("LowPriority Task release the MuxSem...\n");
        xReturn = xSemaphoreGive(MuxSem_Handle);

        __BSP_LED1_Ficker(100);
        vTaskDelay(300);
    }
}

static void MidPriority_Task(void *pvParameters)
{
    while (1)
    {
        printf("MidPriority Task running...\n");
        vTaskDelay(300);
    }
}

static void HighPriority_Task(void *pvParameters)
{
    BaseType_t xReturn = pdTRUE;
    vTaskDelay(1000);
    while(1)
    {
        printf("HighPriority Task GET the MuxSem...\n");
        xReturn = xSemaphoreTake(MuxSem_Handle,
                                 portMAX_DELAY);
        if (xReturn == pdTRUE)
            printf("HighPriority Task GOT the MuxSem Successfully!\n");

        // 释放互斥量
        printf("HighPriority Task release the MuxSem...\n");
        xReturn = xSemaphoreGive(MuxSem_Handle);

        __BSP_LED2_Ficker(100);
        vTaskDelay(300);
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