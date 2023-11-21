# 数码管实时时钟显示

| RTC |
|-----|

## 添加RTC支持

## 初始化RTC外设

创建`rtc.c` `rtc.h` 文件

### API函数

RTC时钟初始化:
`HAL_StatusTypeDef HAL_RTC_Init(RTC_HandleTypeDef *hrtc)`

RTC时钟时间设置:
`HAL_StatusTypeDef HAL_RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format)`

RTC日期设置:
`HAL_StatusTypeDef HAL_RTC_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format)`

```c
// RTC初始化
void MX_RTC_Init(void) {
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef DateToUpdate = {0};

    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = 32767;

    HAL_RTC_Init(&hrtc);

    sTime.Hours = 0x0;
    sTime.Minutes = 0x0;
    sTime.Seconds = 0x0;

    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);

    // 2023 11 10 五
    DateToUpdate.Year = 0x23;
    DateToUpdate.Month = RTC_MONTH_NOVEMBER;
    DateToUpdate.Date = 0x10;
    DateToUpdate.WeekDay = RTC_WEEKDAY_FRIDAY;

    HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD);
}

// RTC msp init
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
```
## RTC demo

`rtc.c`
```c
void MX_RTC_Init(void) {
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef DateToUpdate = {0};

    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = 32767;

    HAL_RTC_Init(&hrtc);
    
    // 00:00:00
    sTime.Hours = 0x0;
    sTime.Minutes = 0x0;
    sTime.Seconds = 0x0;

    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);

    // 2023 11 10 五
    DateToUpdate.Year = 0x23;
    DateToUpdate.Month = RTC_MONTH_NOVEMBER;
    DateToUpdate.Date = 0x10;
    DateToUpdate.WeekDay = RTC_WEEKDAY_FRIDAY;

    HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD);
}

// RTC msp init
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
```

`main.c`

```c
#include <stdio.h>
#include "stm32f1xx_hal.h"
#include "rtc.h"

uint8_t sShowTime[40];
uint8_t sShowData[40];

UART_HandleTypeDef huart1;

void MX_USART1_UART_Init(void) {
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle) {

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
  }
}



void main {
    MX_USART1_UART_Init();
    MX_RTC_Init();
    
    while(1){
        RTC_TimeShow(sShowTime);
        RTC_DataShow(sShowData);
        printf("data:%s time: %s\r\n",sShowData,sShowTime);
        HAL_Delay(1000);
    }
};

int _write(int fd, char *ptr, int len) {
    HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, 0xFFFF);
    return len;
}
```

实验现象：

[//]: # (TODO:RTC串口输出结果)

