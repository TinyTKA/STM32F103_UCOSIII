/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "includes.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//创建开始函数任务
#define START_TASK_PRIO 4	// 开始任务优先级
#define START_TASK_STK_SIZE 128	// 开始任务栈大小
OS_TCB starttaskTCB;
CPU_STK START_TASK_STK[START_TASK_STK_SIZE];
void start_task(void *p_args);

//创建LED任务
#define LED_TASK_PRIO 3
#define LED_TASK_STK_SIZE 128
OS_TCB LED0TCB;
CPU_STK LED0_TASK_STK[LED_TASK_STK_SIZE];
void LED_task(void *p_args);

// 创建PWM1输出任务
#define PWM1_TASK_PRIO 2
#define PWM1_TASK_STK_SIZE 128
OS_TCB PWM1_TCB;
CPU_STK PWM1_TASK_STK[PWM1_TASK_STK_SIZE];
void PWM1_task(void *p_args);
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
/* USER CODE BEGIN PFP */

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
	OS_ERR err;
	CPU_SR_ALLOC();
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
    //CPU_Init();

//    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */
//    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        /* Determine nbr SysTick increments                     */
//    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */

//    Mem_Init();                                                 /* Initialize Memory Management Module                  */

//#if OS_CFG_STAT_TASK_EN > 0u
//    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
//#endif

//    CPU_IntDisMeasMaxCurReset();
	
	OSInit(&err);
	OS_CRITICAL_ENTER();
	OSTaskCreate((OS_TCB   *)&starttaskTCB,
							 (CPU_CHAR *)"start task",
							(OS_TASK_PTR)start_task,
							 (void     *)0,
							 (OS_PRIO   )START_TASK_PRIO,
							 (CPU_STK  *)&START_TASK_STK[0],
							 (CPU_STK_SIZE)START_TASK_STK_SIZE/10,
							 (CPU_STK_SIZE)START_TASK_STK_SIZE,
							 (OS_MSG_QTY  )0,
							 (OS_TICK     )0,
							 (void       *)0,
							(OS_OPT				)OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
							(OS_ERR      *)&err);
	OS_CRITICAL_EXIT();
	OSStart(&err);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//  while (1)
//  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//  }
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

/* USER CODE BEGIN 4 */
void start_task(void *p_args)
{
	OS_ERR err;
	CPU_SR_ALLOC();

	p_args = p_args;
	CPU_Init();

	OS_CRITICAL_ENTER();

	OSTaskCreate((OS_TCB   *)&LED0TCB,
				(CPU_CHAR *)"LED0 task",
				(OS_TASK_PTR)LED_task,
				(void     *)0,
				(OS_PRIO   )LED_TASK_PRIO,
				(CPU_STK  *)&LED0_TASK_STK[0],
				(CPU_STK_SIZE)LED_TASK_STK_SIZE/10,
				(CPU_STK_SIZE)LED_TASK_STK_SIZE,
				(OS_MSG_QTY  )0,
				(OS_TICK     )0,
				(void       *)0,
				(OS_OPT		 )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
				(OS_ERR     *)&err);

OSTaskCreate((OS_TCB   *)&PWM1_TCB,
				(CPU_CHAR *)"PWM1 task",
				(OS_TASK_PTR)PWM1_task,
				(void     *)0,
				(OS_PRIO   )PWM1_TASK_PRIO,
				(CPU_STK  *)&PWM1_TASK_STK[0],
				(CPU_STK_SIZE)PWM1_TASK_STK_SIZE/10,
				(CPU_STK_SIZE)PWM1_TASK_STK_SIZE,
				(OS_MSG_QTY  )0,
				(OS_TICK     )0,
				(void       *)0,
				(OS_OPT		 )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
				(OS_ERR     *)&err);

	OS_TaskSuspend((OS_TCB*)&starttaskTCB,&err);
	OS_CRITICAL_EXIT();
}

void LED_task(void *p_args)
{
	OS_ERR err;
	p_args=p_args;
	while (1)
	{
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_NON_STRICT,&err);
	}
	
}
void PWM1_task(void *p_args)
{
	OS_ERR err;
	p_args=p_args;
	
	while (1)
	{
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_NON_STRICT,&err);
	}
	
}
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

#ifdef  USE_FULL_ASSERT
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
