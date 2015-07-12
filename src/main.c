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
#include "common.h"
#include "main.h"

/* Private typedef */

/* Private define  */
#define DELAY	(1000)

extern void initialise_monitor_handles(void);


/* Private macro */
#define PI (float) 3.14159265f
#define LSM_Acc_Sensitivity_2g (float) 1.0f /*!< accelerometer sensitivity with 2 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_4g (float) 0.5f /*!< accelerometer sensitivity with 4 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_8g (float) 0.25f /*!< accelerometer sensitivity with 8 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_16g (float) 0.0834f /*!< accelerometer sensitivity with 12 g full scale [LSB/mg] */


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

	//configure the acc on the board
	Acc_Config();

	uint8_t pBuffer[6];
	float pnData[3];

	//setbuf(stdout, NULL);

	//initialise_monitor_handles();
	printf("Program started \n");

	while (1)
	{
		/* Waiting User Button is pressed */
		//UserButtonPressed = STM_EVAL_PBGetState(BUTTON_USER);
		//while (UserButtonPressed == 0x00)
		{

			// send a read
			//LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_L_A,pBuffer, 6);
			Acc_ReadData(pnData);

			int i = 0;


			for(i=0; i<3; i++)
			{
				trace_printf("%f ", pnData[i]);
			}

			trace_printf(" \n");
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

	// setup the ACC configs
	LSM303DLHCAcc_InitSturcture.Power_Mode = LSM303DLHC_NORMAL_MODE;
	LSM303DLHCAcc_InitSturcture.Axes_Enable = LSM303DLHC_AXES_ENABLE;
	LSM303DLHCAcc_InitSturcture.AccOutput_DataRate = LSM303DLHC_ODR_50_HZ;
	LSM303DLHCAcc_InitSturcture.AccFull_Scale = LSM303DLHC_FULLSCALE_2G;
	LSM303DLHCAcc_InitSturcture.BlockData_Update = LSM303DLHC_BlockUpdate_Continous;
	LSM303DLHCAcc_InitSturcture.Endianness = LSM303DLHC_BLE_LSB;
	LSM303DLHCAcc_InitSturcture.High_Resolution = LSM303DLHC_HR_ENABLE;

	// set the filter configs
	LSM303DLHCAcc_FilterConfigStructure.HighPassFilter_AOI1 = LSM303DLHC_HPF_AOI1_DISABLE;
	LSM303DLHCAcc_FilterConfigStructure.HighPassFilter_AOI2 = LSM303DLHC_HPF_AOI2_DISABLE;
	LSM303DLHCAcc_FilterConfigStructure.HighPassFilter_Mode_Selection = LSM303DLHC_HPM_NORMAL_MODE;
	LSM303DLHCAcc_FilterConfigStructure.HighPassFilter_CutOff_Frequency = LSM303DLHC_HPFCF_16;

	LSM303DLHC_AccInit(&LSM303DLHCAcc_InitSturcture);
	LSM303DLHC_AccFilterConfig(&LSM303DLHCAcc_FilterConfigStructure);

}

/**
* @brief Read LSM303DLHC output register, and calculate the acceleration ACC=(1/SENSITIVITY)* (out_h*256+out_l)/16 (12 bit rappresentation)
* @param pnData: pointer to float buffer where to store data
* @retval None
*/
void Acc_ReadData(float* pfData)
{
int16_t pnRawData[3];
uint8_t ctrlx[2];
float LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_2g;
uint8_t buffer[6], cDivider;
uint8_t i = 0;
/* Read the register content */
LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG4_A, ctrlx,2);
LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_L_A, buffer, 6);
if(ctrlx[1]&0x40)
cDivider=64;
else
cDivider=16;
/* check in the control register4 the data alignment*/
if(!(ctrlx[0] & 0x40) || (ctrlx[1] & 0x40)) /* Little Endian Mode or FIFO mode */
{
for(i=0; i<3; i++)
{
pnRawData[i]=((int16_t)((uint16_t)buffer[2*i+1] << 8) + buffer[2*i])/cDivider;
}
}
else /* Big Endian Mode */
{
for(i=0; i<3; i++)
pnRawData[i]=((int16_t)((uint16_t)buffer[2*i] << 8) + buffer[2*i+1])/cDivider;
}
/* Read the register content */
LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG4_A, ctrlx,2);
if(ctrlx[1]&0x40)
{
/* FIFO mode */
LSM_Acc_Sensitivity = 0.25;
}
else
{
/* normal mode */
/* switch the sensitivity value set in the CRTL4*/
switch(ctrlx[0] & 0x30)
{
case LSM303DLHC_FULLSCALE_2G:
LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_2g;
break;
case LSM303DLHC_FULLSCALE_4G:
LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_4g;
break;
case LSM303DLHC_FULLSCALE_8G:
LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_8g;
break;
case LSM303DLHC_FULLSCALE_16G:
LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_16g;
break;
}
}
/* Obtain the mg value for the three axis */
for(i=0; i<3; i++)
{
pfData[i]=(float)pnRawData[i]/LSM_Acc_Sensitivity;
}
}

/**
* @}
*/
/**
* @}
*/

