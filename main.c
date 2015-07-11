/**
 *****************************************************************************
 **
 **  File        : main.c
 **
 **  Abstract    : main function.
 **
 **  Functions   : main
 **
 **  Environment : Atollic TrueSTUDIO(R)
 **
 **  Distribution: The file is distributed �as is,� without any warranty
 **                of any kind.
 **
 **  (c)Copyright Atollic AB.
 **  You may use this file as-is or modify it according to the needs of your
 **  project. Distribution of this file (unmodified or modified) is not
 **  permitted. Atollic AB permit registered Atollic TrueSTUDIO(R) users the
 **  rights to distribute the assembled, compiled & linked contents of this
 **  file as part of an application binary file, provided that it is built
 **  using the Atollic TrueSTUDIO(R) toolchain.
 **
 *****************************************************************************
 */

/* Includes */
#include "stm32f30x.h"
#include "stm32f3_discovery.h"
#include "stm32f3_discovery_l3gd20.h"
#include "stm32f3_discovery_lsm303dlhc.h"
#include "common.h"
#include "main.h"

/* Private typedef */

/* Private define  */
#define DELAY	(1000)

/* Private macro */

/* Private variables */
float a, b, c, d;

/* Private function prototypes */

/* Private functions */

/* Global variables */

/**
 **===========================================================================
 **
 **  Abstract: main program
 **
 **===========================================================================
 */
int main(void)
{
	uint32_t ii;

	/* Example use SysTick timer and read System core clock */
	SysTick_Config(72);  /* 1 us if clock frequency 72 MHz */

	SystemCoreClockUpdate();
	ii = SystemCoreClock;   /* This is a way to read the System core clock */

	/* Initialize LEDs and User Button available on STM32F3-Discovery board */
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);
	STM_EVAL_LEDInit(LED7);
	STM_EVAL_LEDInit(LED8);
	STM_EVAL_LEDInit(LED9);
	STM_EVAL_LEDInit(LED10);

	//STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);

	/* Configure PA0 in interrupt mode */
	EXTI0_Config();

	uint32_t UserButtonPressed;

	while (1)
	{
		/* Waiting User Button is pressed */
		//UserButtonPressed = STM_EVAL_PBGetState(BUTTON_USER);
		//while (UserButtonPressed == 0x00)
		{

			/* Toggle LD3 */
			STM_EVAL_LEDToggle(LED3);
			_delay_ms(DELAY);

			/* Toggle LD5 */
			STM_EVAL_LEDToggle(LED5);
			_delay_ms(DELAY);

			/* Toggle LD7 */
			STM_EVAL_LEDToggle(LED7);
			_delay_ms(DELAY);

			/* Toggle LD9 */
			STM_EVAL_LEDToggle(LED9);
			_delay_ms(DELAY);

			/* Toggle LD10 */
			STM_EVAL_LEDToggle(LED10);
			_delay_ms(DELAY);

			/* Toggle LD8 */
			STM_EVAL_LEDToggle(LED8);
			_delay_ms(DELAY);

			/* Toggle LD6 */
			STM_EVAL_LEDToggle(LED6);
			_delay_ms(DELAY);

			/* Toggle LD4 */
			STM_EVAL_LEDToggle(LED4);
			_delay_ms(DELAY);
		}
	}

	/* Program will never run to this line */
	return 0;
}

/**
  * @brief  Configure PA0 in interrupt mode
  * @param  None
  * @retval None
  */
void EXTI0_Config(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

  /* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure PA0 pin as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Connect EXTI0 Line to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

  /* Configure EXTI0 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void Gyro_Config(void){

  // variables
  L3GD20_InitTypeDef L3GD20_InitStructure;
  L3GD20_FilterConfigTypeDef L3GD20_FilterConfigStructure;

  //set the L3GD20 Init Structure
  L3GD20_InitStructure.Power_Mode = L3GD20_MODE_ACTIVE;




}

void Acc_Config(void){

	//variables
	LSM303DLHCAcc_InitTypeDef LSM303DLHCAcc_InitSturcture;
	LSM303DLHCAcc_FilterConfigTypeDef LSM303DLHCAcc_FilterConfigStructure;

	// setup the Acc configs


}
