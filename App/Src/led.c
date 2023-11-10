//
// Created by zhihao on 2023/11/4.
//
#include "led.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

void StartLedTask(void *argument) {
    for (;;) {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        vTaskDelay(500);
    }
}

