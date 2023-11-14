//
// Created by zhihao on 2023/11/9.
//

#include "rtc.h"

RTC_HandleTypeDef hrtc;


void MX_RTC_Init(void) {
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef DateToUpdate = {0};

    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = 32767;

    HAL_RTC_Init(&hrtc);
}

void HAL_RTC_MspInit(RTC_HandleTypeDef *rtcHandle) {

    if (rtcHandle->Instance == RTC) {
        /* USER CODE BEGIN RTC_MspInit 0 */

        /* USER CODE END RTC_MspInit 0 */
        HAL_PWR_EnableBkUpAccess();
        /* Enable BKP CLK enable for backup registers */
        __HAL_RCC_BKP_CLK_ENABLE();
        /* RTC clock enable */
        __HAL_RCC_RTC_ENABLE();
        /* USER CODE BEGIN RTC_MspInit 1 */

        /* USER CODE END RTC_MspInit 1 */
    }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *rtcHandle) {
    if (rtcHandle->Instance == RTC) {
        __HAL_RCC_RTC_DISABLE();
    }
}

