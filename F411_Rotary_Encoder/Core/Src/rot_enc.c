/***************************************************
* Module name: ENC
*
* Copyright 2016 Company as an unpublished work.
* All Rights Reserved.
*
* The information contained herein is confidential
* property of Company. The user, copying, transfer or
* disclosure of such information is prohibited except
* by express written agreement with Company.
*
* First written on 01.03.2016 by BogTog.
*
* Module Description:
* Driver for using a quadrature signal rotary encoder
* with STM32 uController
*
**************************************************/
/* Include section
* Add all #includes here
**
**************************************************/
#include "rot_enc.h"

/* Defines section
* Add all #defines here
**
**************************************************/
static GPIO_InitTypeDef  GPIO_InitStruct;

/* Function Prototype Section
* Add prototypes for all functions called by this
* module, with the exception of runtime routines.
**
**************************************************/
extern void ENC_vRotEnc_Handler(ENC_tenDir ENC__enDir);
/**************************************************
* Function name : void ENC_vInit()
* returns : void
* Created by : BogTog
* Date created : 01.03.2016
* Description : Init function for executing the
*	GPIO init for interrupt drivern encoder driver
* Notes : N/A
**************************************************/
void ENC_vInit()
{
	/* Enable encoder GPIO CLK */
	ENC_GPIO_CLK_ENABLE();
	
	/*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = ENC_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ENC_B_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = ENC_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ENC_A_GPIO_Port, &GPIO_InitStruct);
	
	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**************************************************
* Function name : void ENC_vINT_Handler()
* returns : void
* Created by : BogTog
* Date created : 01.03.2016
* Description : Function for handling the interrupt request
*	generated by encoder rotation (no soft debounce. Implemented
* in HW using 100nF capacitors)
* Notes : N/A
**************************************************/
void ENC__vINT_Handler()
{
	ENC_tenDir ENC__enDir;
	if (HAL_GPIO_ReadPin(ENC_B_GPIO_Port, ENC_B_Pin) == GPIO_PIN_SET)
	{
		ENC__enDir = right;
		ENC_vRotEnc_Handler(ENC__enDir);
	}
	
	if (HAL_GPIO_ReadPin(ENC_B_GPIO_Port, ENC_B_Pin) == GPIO_PIN_RESET)
	{
		ENC__enDir = left;
		ENC_vRotEnc_Handler(ENC__enDir);
	}
}