//
// Created by paopa on 2023/5/24.
//

#include <string.h>
#include "led_smg.h"

uint8_t smg_num[18] = {0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe,
                       0xf6};

uint8_t smg_buf[8] = {0};

void delay_us(uint32_t us) {
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--);
}

//数码管驱动 IO 初始化
void LED_SMG_Init(void) {
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE(); //开启 GPIOB 时钟
    __HAL_RCC_GPIOC_CLK_ENABLE(); //开启 GPIOC 时钟
    __HAL_RCC_AFIO_CLK_ENABLE(); //开启 AFIO 时钟
    __HAL_AFIO_REMAP_SWJ_NOJTAG(); //关闭 jtag，使能 SWD，可以用 SWD 模式调
//74HC595 DS-PB3 LCLK-PB4 SCK-PB5
    GPIO_Initure.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;//PB3~5
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP; //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH; //高速
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);
//74HC138 A0-PC10 A1-PC11 A2-PC12
    GPIO_Initure.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;//PC10~12
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
}

//74HC138 驱动
//数码管位选
//num:要显示的数码管编号 0-7(共 8 个数码管)
void LED_Wei(uint8_t num) {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, (GPIO_PinState) (num & 0x01));
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, (GPIO_PinState) ((num & 0x02) >> 1));
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, (GPIO_PinState) ((num & 0x04) >> 2));
}

//74HC595 驱动
//数码管显示
//duan:显示的段码
//wei:要显示的数码管编号 0-7(共 8 个数码管)
void LED_Write_Data(uint8_t duan, uint8_t wei) {
    uint8_t i;
    for (i = 0; i < 8; i++)//先送段
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (GPIO_PinState) ((duan >> i) & 0x01));
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
        delay_us(5);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
    }
    LED_Wei(wei);//后选中位
}

//74HC595 驱动
//数码管刷新显示
void LED_Refresh(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
    delay_us(5);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
}

void LED_Char(uint8_t num, uint8_t t_char) {
    uint32_t i = 0;
    i = t_char - 48;
    smg_buf[num] = smg_num[i];
}

void LED_Str(char *s) {
    if (strlen(s) > 8) {
        return;
    } else {
        for (int i = 0; i < strlen(s); i++) {
            LED_Char(i, s[i]);
        }
    }
}

static uint8_t count = 0;

void LED_Handle(void) {
    LED_Write_Data(smg_buf[count], count);

    LED_Refresh();

    delay_us(500);
    LED_Write_Data(0x00, count);
    LED_Refresh();

    count++;
    if (count == 8) {
        count = 0;
    }
}
