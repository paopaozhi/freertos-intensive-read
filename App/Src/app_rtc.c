//
// Created by zhihao on 2023/11/8.
//

#include <stdio.h>
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "rtc.h"
#include "../../Drivers/LedSmg/led_smg.h"

uint8_t sShowTime[40];
uint8_t sShowData[40];
// 0: time 1: day
uint8_t isShowMode = 0;

static void RTC_TimeShow(uint8_t *showtime);

static void RTC_DataShow(uint8_t *showdata);

void StartRtcTask(void *argument) {
    for (;;) {
        RTC_TimeShow(sShowTime);
        RTC_DataShow(sShowData);
        HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
        // printf("time: %s\r\n",sShowTime);
        if (!isShowMode) {
            LED_Str((char *) sShowTime);
        } else {
            LED_Str((char *) sShowData);
        }

        vTaskDelay(1000);
    }
}

static RTC_DateTypeDef sdatestructureget;
static RTC_TimeTypeDef stimestructureget;

static void RTC_TimeShow(uint8_t *showtime) {
    /* Get the RTC current Time */
    HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);


    /* Display time Format : hh:mm:ss */
    sprintf((char *) showtime, "%02d-%02d-%02d", stimestructureget.Hours, stimestructureget.Minutes,
            stimestructureget.Seconds);
}

static void RTC_DataShow(uint8_t *showdata) {
    /* Get the RTC current Date */
    HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);

    /* Display time Format : hh:mm:ss */
    sprintf((char *) showdata, "%02d-%02d-%02d", sdatestructureget.Year, sdatestructureget.Month,
            sdatestructureget.Date);
}
