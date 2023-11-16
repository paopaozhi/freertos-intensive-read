//
// Created by zhihao on 2023/11/13.
//
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"


xTimerHandle usartTimerHandle;
//xQueueHandle usart1QueueHandle;

uint8_t USART1_Buf[128];
uint32_t USART1_BufLen = 0;
uint8_t USART1_State = 0;

void Usart1_Recycle(void);


void ledCall(TimerHandle_t xTimer) {
    Usart1_Recycle();
}

void StartUsartTask(void *argument) {
//    usart1QueueHandle = xQueueCreate(5, sizeof(uint8_t *));

    HAL_UART_Receive_IT(&huart1, USART1_Buf, 1);
    usartTimerHandle = xTimerCreate("myTimer", 5, pdFALSE, (void *) 0, ledCall);
    if (usartTimerHandle == NULL) {
        Error_Handler();
    }
    xTimerStop(usartTimerHandle, portMAX_DELAY);
    for (;;) {
        vTaskDelay(1);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xTimerStartFromISR(usartTimerHandle, &xHigherPriorityTaskWoken);
        USART1_State = 1;
        USART1_BufLen++;
        HAL_UART_Receive_IT(huart, (USART1_Buf + USART1_BufLen), 1);
    }
}

/**
 * @brief 串口不定长接受完成回调函数
 *
 * @param buf
 * @param len
 */
uint8_t *usartCallBuf;
__weak void App_Usart2Callback(uint8_t *buf, uint32_t len) {
    usartCallBuf =  (uint8_t *)pvPortMalloc(sizeof(USART1_Buf));

    memcpy(usartCallBuf,buf,len);

    xQueueSend(rtcQueueHandle,&usartCallBuf,portMAX_DELAY);
    printf("%s\r\n", buf);
//    return;
}

/**
 * @brief 串口回调应用函数
 *
 */
void Usart1_Recycle(void) {
    if (USART1_State == 1) {
        HAL_UART_Abort_IT(&huart1);
        App_Usart2Callback(USART1_Buf, USART1_BufLen);
        USART1_State = 0;
        USART1_BufLen = 0;
        memset(USART1_Buf, 0, 128);
        HAL_UART_Receive_IT(&huart1, USART1_Buf, 1);
    }
}