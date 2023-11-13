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

static void RTC_InitSetDate(void);
static void RTC_SaveData(rtcConfigTypedef *data);

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
    RTC_InitSetDate();
    for (;;) {
        xQueueReceive(rtcQueueHandle, (void *) &sConfig, portMAX_DELAY);
        RTC_Config(&sConfig->data, &sConfig->time);
    }
}

static RTC_DateTypeDef sdatestructureget;
static RTC_TimeTypeDef stimestructureget;

void RTC_SaveDate(RTC_DateTypeDef *date) {
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, date->Year);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, date->Month);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, date->Date);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, date->WeekDay);
}

void RTC_SaveTime(RTC_TimeTypeDef *time) {
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR5, time->Hours);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR6, time->Minutes);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR7, time->Seconds);
}

static void RTC_TimeShow(uint8_t *showtime) {
    /* Get the RTC current Time */
    HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);

    RTC_SaveTime(&stimestructureget);

    /* Display time Format : hh:mm:ss */
    sprintf((char *) showtime, "%02d-%02d-%02d", stimestructureget.Hours, stimestructureget.Minutes,
            stimestructureget.Seconds);
}

static void RTC_DataShow(uint8_t *showdata) {
    /* Get the RTC current Date */
    HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);

    RTC_SaveDate(&sdatestructureget);

    /* Display time Format : hh:mm:ss */
    sprintf((char *) showdata, "%02d-%02d-%02d", sdatestructureget.Year, sdatestructureget.Month,
            sdatestructureget.Date);
}

static void RTC_Config(RTC_DateTypeDef *date, RTC_TimeTypeDef *time) {
    HAL_RTC_SetTime(&hrtc, time, RTC_FORMAT_BCD);
    HAL_RTC_SetDate(&hrtc, date, RTC_FORMAT_BCD);
}



static void RTC_InitSetDate(void) {
    rtcConfigTypedef *pConfigData;
    pConfigData = (rtcConfigTypedef *) pvPortMalloc(sizeof(rtcConfigTypedef));

    pConfigData->data.Year = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);
    pConfigData->data.Month = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2);
    pConfigData->data.Date = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3);
    pConfigData->data.WeekDay = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR4);

    pConfigData->time.Hours = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR5);
    pConfigData->time.Minutes = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR6);
    pConfigData->time.Seconds = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR7);

//    RTC_Config(&pConfigData->data, &pConfigData->time);
    HAL_RTC_SetTime(&hrtc, &pConfigData->time, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &pConfigData->data, RTC_FORMAT_BIN);

    vPortFree(pConfigData);
}