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

TaskHandle_t defaultTaskHandle;
TaskHandle_t ledTaskHandle;

void StartDefaultTask(void *argument);

extern void StartLedTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
    xTaskCreate(
            StartDefaultTask,
            "defaultTask",
            128,
            NULL,
            1,
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
            64,
            NULL,
            2,
            &ledTaskHandle
    );

    vTaskDelete(defaultTaskHandle);
    taskEXIT_CRITICAL();

}



