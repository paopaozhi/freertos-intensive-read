//
// Created by zhihao on 2023/11/7.
//

#include "ledsmg.h"
#include "FreeRTOS.h"
#include "task.h"
#include "../../Drivers/LedSmg/led_smg.h"

void StartLedSmgTask(void *argument) {
    for (;;) {
        LED_Handle();
        vTaskDelay(1);
    }
}