//
// Created by zhihao on 2023/11/8.
//

#include <stdio.h>
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "rtc.h"

uint8_t sShowTime[40];

static void RTC_TimeShow(uint8_t *showtime);

void StartRtcTask(void *argument) {
    for (;;) {
        RTC_TimeShow(sShowTime);
        printf("%s",sShowTime);
        vTaskDelay(500);
    }
}

static void RTC_TimeShow(uint8_t *showtime) {
    RTC_DateTypeDef sdatestructureget;
    RTC_TimeTypeDef stimestructureget;

    /* Get the RTC current Time */
    HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
    /* Get the RTC current Date */
    HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
    /* Display time Format : hh:mm:ss */
    sprintf((char *) showtime, "%02d:%02d:%02d", stimestructureget.Hours, stimestructureget.Minutes,
            stimestructureget.Seconds);
}