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
TaskHandle_t falshTaskHandle;
TaskHandle_t usartTaskHandle;

void StartDefaultTask(void *argument);

extern void StartLedTask(void *argument);

extern void StartKeyTask(void *argument);

extern void StartLedSmgTask(void *argument);

extern void StartRtcTask(void *argument);

extern void StartFalshTask(void *argument);

extern void StartUsartTask(void *argument);

void init(void){

}

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
extern "C" {
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

void MX_FREERTOS_Init(void) {
    LED_SMG_Init();
    xTaskCreate(
            StartDefaultTask,
            "defaultTask",
            128,
            nullptr,
            31,
            &defaultTaskHandle
    );

    vTaskStartScheduler();
}
};
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
            nullptr,
            1,
            &ledTaskHandle
    );

    xTaskCreate(
            StartKeyTask,
            "key",
            128,
            NULL,
            10,
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
            200,
            NULL,
            4,
            &rtcTaskHandle
    );

//    xTaskCreate(
//            StartFalshTask,
//            "falsh",
//            256,
//            NULL,
//            1,
//            &falshTaskHandle
//    );

    xTaskCreate(
            StartUsartTask,
            "usart",
            512,
            NULL,
            8,
            &usartTaskHandle
    );

    vTaskDelete(defaultTaskHandle);
    taskEXIT_CRITICAL();
}



