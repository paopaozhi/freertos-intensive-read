//
// Created by zhihao on 2023/11/8.
//

#include <stdio.h>
#include "main.h"
#include "rtc.h"
#include "../../Drivers/LedSmg/led_smg.h"

TaskHandle_t rtcConfigHandle;
QueueHandle_t rtcQueueHandle;

uint8_t sShowTime[15];
uint8_t sShowData[15];
// 0: time 1: day
uint8_t isShowMode = 0;

static void RTC_TimeShow(uint8_t *showtime);

static void RTC_DataShow(uint8_t *showdata);

static void StartRtcConfigTask(void *argument);

static void RTC_Config(RTC_DateTypeDef *date, RTC_TimeTypeDef *time);

void StartRtcTask(void *argument) {
    rtcQueueHandle = xQueueCreate(5, sizeof(rtcConfigTypedef *));
    if (rtcQueueHandle == NULL) {
        Error_Handler();
    }

    xTaskCreate(
            StartRtcConfigTask,
            "rtcConfig",
            64,
            NULL,
            5,
            &rtcConfigHandle
    );
    for (;;) {
        RTC_TimeShow(sShowTime);
        RTC_DataShow(sShowData);

        if (!isShowMode) {
            LED_Str((char *) sShowTime);
        } else {
            LED_Str((char *) sShowData);
        }

        vTaskDelay(1000);
    }
}

rtcConfigTypedef *sConfig;

static void StartRtcConfigTask(void *argument) {
    for (;;) {
        xQueueReceive(rtcQueueHandle, (void *) &sConfig, portMAX_DELAY);
        RTC_Config(&sConfig->data, &sConfig->time);
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

static void RTC_Config(RTC_DateTypeDef *date, RTC_TimeTypeDef *time) {
    HAL_RTC_SetTime(&hrtc, time, RTC_FORMAT_BCD);
    HAL_RTC_SetDate(&hrtc, date, RTC_FORMAT_BCD);
}
