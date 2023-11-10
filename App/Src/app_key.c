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
    keyName1
} keyNameTypedef;

typedef struct {
    uint8_t name;
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
    keyStateTypedef state;
} keyTypedef;

TaskHandle_t keyCallHandle;

QueueHandle_t keyQueueHandle;

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

static void key_readState(keyTypedef *key, QueueHandle_t handle);

void StartKeyCallTask(void *argument);

void StartKeyTask(void *argument) {
    keyQueueHandle = xQueueCreate(10, sizeof(uint32_t));

    xTaskCreate(
            StartKeyCallTask,
            "keyCall",
            128,
            NULL,
            3,
            &keyCallHandle
    );

    for (;;) {
        key_readState(&key0, keyQueueHandle);
        key_readState(&key1, keyQueueHandle);
        vTaskDelay(50);
    }
}

void StartKeyCallTask(void *argument) {
    int key;
    for (;;) {
        xQueueReceive(keyQueueHandle, &key, portMAX_DELAY);
        if (key == keyName0) {
            // TODO: 数码管Mode切换
        } else if (key == keyName1) {
            // TODO: 测试demo 可替换为其他逻辑
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
