#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "time.h"
#include "queue.h"
#include <stdio.h>

typedef enum {
    IDLE_STATE = 0,
    PRESS_DETECTED_STATE,
    RELEASE_DETECTED_STATE
} keyStateTypedef;

typedef enum {
    keyName0 = 0,
    keyName1,
    keyName2
} keyNameTypedef;

typedef struct {
    uint8_t name;
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
    keyStateTypedef state;
} keyTypedef;

extern uint8_t isShowMode;

TaskHandle_t keyCallHandle;

xQueueHandle keyQueueHandle;

keyTypedef key0 = {
        .name = keyName0,
        .GPIOx = KEY0_GPIO_Port,
        .GPIO_Pin = KEY0_Pin,
        .state = IDLE_STATE
};
keyTypedef key1 = {
        .name = keyName1,
        .GPIOx = KEY1_GPIO_Port,
        .GPIO_Pin = KEY1_Pin,
        .state = IDLE_STATE
};
keyTypedef key2 = {
        .name = keyName2,
        .GPIOx = KEY2_GPIO_Port,
        .GPIO_Pin = KEY2_Pin,
        .state = IDLE_STATE
};

static void key_readState(keyTypedef *key, QueueHandle_t handle);

void StartKeyCallTask(void *argument);

void StartKeyTask(void *argument) {
    keyQueueHandle = xQueueCreate(5, sizeof(uint32_t));
    if (keyQueueHandle == NULL) {
        Error_Handler();
    }

    xTaskCreate(
            StartKeyCallTask,
            "keyCall",
            256, // printf:164 32
            NULL,
            11,
            &keyCallHandle
    );

    for (;;) {
        key_readState(&key0, keyQueueHandle);
        key_readState(&key1, keyQueueHandle);
        key_readState(&key2, keyQueueHandle);
        vTaskDelay(50);
    }
}

void StartKeyCallTask(void *argument) {
    int key;
    uint32_t free;
    for (;;) {
        xQueueReceive(keyQueueHandle, &key, portMAX_DELAY);
        if (key == keyName0) {
            // TODO: 数码管Mode切换
            isShowMode = !isShowMode;
        } else if (key == keyName1) {
            // TODO: 测试demo 可替换为其他逻辑
            // xQueueSend(rtcQueueHandle, &pconfigRtc, portMAX_DELAY);
            HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
        } else if (key == keyName2) {
            // TODO: 测试demo 可替换为其他逻辑
            free = xPortGetFreeHeapSize();
            printf("free:%lu\r\n",free);
            HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
        } else {}
    }
}

static void key_readState(keyTypedef *key, QueueHandle_t handle) {
    if (HAL_GPIO_ReadPin(key->GPIOx, key->GPIO_Pin) == 0) {
        if (key->state != PRESS_DETECTED_STATE) {
            key->state = PRESS_DETECTED_STATE;
            xQueueSend(handle, &key->name, portMAX_DELAY);
        }
    } else {
        key->state = RELEASE_DETECTED_STATE;
    }
}
