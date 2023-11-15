//
// Created by zhihao on 2023/11/8.
//

#include <stdio.h>
#include <string.h>
#include "main.h"
#include "rtc.h"
#include "../../Drivers/LedSmg/led_smg.h"
#include "cJSON.h"

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
    rtcQueueHandle = xQueueCreate(5, sizeof(uint8_t *));
    if (rtcQueueHandle == NULL) {
        Error_Handler();
    }

    xTaskCreate(
            StartRtcConfigTask,
            "rtcConfig",
            128,
            NULL,
            5,
            &rtcConfigHandle
    );
    if(rtcConfigHandle == NULL){
        Error_Handler();
    }
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

uint8_t *sConfig;
RTC_DateTypeDef test_configDate;

static void unpack(char *str) {
    cJSON *json_Handle;

    cJSON *cjson_date = NULL;

    cJSON *cjson_Year = NULL;
    cJSON *cjson_Month = NULL;
    cJSON *cjson_Date = NULL;
    cJSON *cjson_WeekDay = NULL;

    json_Handle = cJSON_Parse(str);

    cjson_date = cJSON_GetObjectItem(json_Handle, "date");

    cjson_Year = cJSON_GetObjectItem(cjson_date, "year");
    cjson_Month = cJSON_GetObjectItem(cjson_date, "Month");
    cjson_Date = cJSON_GetObjectItem(cjson_date, "Date");
    cjson_WeekDay = cJSON_GetObjectItem(cjson_date, "WeekDay");

    test_configDate.Year = cjson_Year->valueint;
    test_configDate.Month = cjson_Month->valueint;
    test_configDate.Date = cjson_Date->valueint;
    test_configDate.WeekDay = cjson_WeekDay->valueint;

    cJSON_Delete(json_Handle);
}

static void StartRtcConfigTask(void *argument) {
    for (;;) {
        xQueueReceive(rtcQueueHandle, &sConfig, portMAX_DELAY);

        unpack((char *) sConfig);
        RTC_Config(&test_configDate, NULL);

        vPortFree(sConfig);
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
    HAL_RTC_SetTime(&hrtc, time, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, date, RTC_FORMAT_BIN);
}
