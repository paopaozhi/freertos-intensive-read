//
// Created by zhihao on 2023/11/11.
//

#include <stdio.h>
#include "main.h"
#include "norflash.h"
#include "FreeRTOS.h"
#include "task.h"

uint8_t i = 1;
void StartFalshTask(void *argument) {
    Norflash_Init();
    Norflash_Read(&i,0x01,1);
    i ++;
    Norflash_Write(&i,0x01,1);
    printf("i = %d\r\n",i);
    for (;;) {
        vTaskDelay(1);
    }
}