//
// Created by zhihao on 2023/11/11.
//

#include <stdio.h>
#include "main.h"
#include "norflash.h"
#include "FreeRTOS.h"
#include "task.h"

static void WriteData(rtcConfigTypedef *rtc);

uint8_t i = 1;
void StartFalshTask(void *argument) {
    Norflash_Init();
    Norflash_Read(&i,0x01,1);
    i ++;
    Norflash_Write(&i,0x01,1);
    printf("i = %d\r\n",i);
    for (;;) {
//        WriteData();
        vTaskDelay(1);
    }
}

void WriteData(rtcConfigTypedef *rtc){
    Norflash_Write(&rtc->data.Year,0x05,1);
    Norflash_Write(&rtc->data.Month,0x06,1);
    Norflash_Write(&rtc->data.Date,0x07,1);
    Norflash_Write(&rtc->data.WeekDay,0x08,1);

    Norflash_Write(&rtc->time.Hours,0x09,1);
    Norflash_Write(&rtc->time.Minutes,0x0A,1);
    Norflash_Write(&rtc->time.Seconds,0x0B,1);
}