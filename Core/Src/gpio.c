/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, TFT_DB4_Pin|TFT_DB5_Pin|TFT_DCX_Pin|VOICE_BROAD_Pin
                          |VOICE_MUTE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, TFT_DB6_Pin|TFT_DB7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, WIFI_EN_Pin|LED_POWER_Pin|PLASMA_CTL_Pin
                          |FAN_CTL_1_Pin|FAN_CTL_2_Pin|TEMP_SENSOR_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOA,LED_KEY_TIM_Pin,GPIO_PIN_RESET);                  

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RELAY_CTL_Pin|TFT_WR_Pin
                          |TFT_CS_Pin|TFT_BACKLIGHT_Pin|TFT_DB0_Pin|TFT_DB1_Pin
                          |TFT_DB2_Pin|TFT_DB3_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOB,LED_WIFI_Pin|LED_PTC_Pin,GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, TFT_RESET_Pin|TFT_RD_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOD,LED_KILL_Pin|LED_RAT_Pin,GPIO_PIN_SET);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                           PCPin */
  GPIO_InitStruct.Pin = TFT_DB4_Pin|TFT_DB5_Pin|TFT_DCX_Pin|VOICE_BROAD_Pin
                          |VOICE_MUTE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PFPin PFPin */
  GPIO_InitStruct.Pin = TFT_DB6_Pin|TFT_DB7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin
                           PAPin PAPin */
  GPIO_InitStruct.Pin = WIFI_EN_Pin|LED_KEY_TIM_Pin|LED_POWER_Pin|PLASMA_CTL_Pin
                          |FAN_CTL_1_Pin|FAN_CTL_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin
                           PBPin PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = RELAY_CTL_Pin|LED_PTC_Pin|TFT_BACKLIGHT_Pin|LED_WIFI_Pin
                          |TFT_DB0_Pin|TFT_DB1_Pin|TFT_DB2_Pin|TFT_DB3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = KEY_POWER_Pin|KEY_MODE_Pin|KEY_ADD_Pin|KEY_DEC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = TEMP_SENSOR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(TEMP_SENSOR_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PDPin PDPin PDPin PDPin */
  GPIO_InitStruct.Pin = LED_KILL_Pin|TFT_RESET_Pin|TFT_RD_Pin|LED_RAT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = TFT_WR_Pin|TFT_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**/
  __HAL_SYSCFG_FASTMODEPLUS_ENABLE(SYSCFG_FASTMODEPLUS_PB6);

  /**/
  __HAL_SYSCFG_FASTMODEPLUS_ENABLE(SYSCFG_FASTMODEPLUS_PB7);

  /**/
  __HAL_SYSCFG_FASTMODEPLUS_ENABLE(SYSCFG_FASTMODEPLUS_PB8);

  /**/
  __HAL_SYSCFG_FASTMODEPLUS_ENABLE(SYSCFG_FASTMODEPLUS_PB9);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
