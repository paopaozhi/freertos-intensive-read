/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "../../Drivers/LedSmg/led_smg.h"

TaskHandle_t defaultTaskHandle;
TaskHandle_t ledTaskHandle;
TaskHandle_t keyTaskHandle;
TaskHandle_t ledSmgTaskHandle;
TaskHandle_t rtcTaskHandle;

void StartDefaultTask(void *argument);

extern void StartLedTask(void *argument);
extern void StartKeyTask(void *argument);

void StartLedSmgTask(void *argument);

void StartRtcTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
    LED_SMG_Init();
    LED_Str("123");
    xTaskCreate(
            StartDefaultTask,
            "defaultTask",
            128,
            NULL,
            31,
            &defaultTaskHandle
    );

    vTaskStartScheduler();
}

/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
void StartDefaultTask(void *argument) {
    taskENTER_CRITICAL();

    xTaskCreate(
            StartLedTask,
            "led",
            32,
            NULL,
            1,
            &ledTaskHandle
    );

    xTaskCreate(
            StartKeyTask,
            "key",
            128,
            NULL,
            2,
            &keyTaskHandle
    );


    xTaskCreate(
            StartLedSmgTask,
            "ledSmg",
            64,
            NULL,
            3,
            &ledSmgTaskHandle
    );

    // 512byte
    xTaskCreate(
            StartRtcTask,
            "rtc",
            512,
            NULL,
            3,
            &rtcTaskHandle
    );

    vTaskDelete(defaultTaskHandle);
    taskEXIT_CRITICAL();
}



