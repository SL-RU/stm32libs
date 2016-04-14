#include "spi.h"
#include "gpio.h"
#include <stdio.h>
#include "ff.h"
#include "keyboard.h"
#include "gui.h"

#define VS1053_SPI hspi2
#define VS1053_xCS GPIOC, GPIO_PIN_7
#define VS1053_xDCS GPIOC, GPIO_PIN_8
#define VS1053_xRST GPIOB, GPIO_PIN_12
#define VS1053_DREQ GPIOC, GPIO_PIN_6

void VS1053_Init(void);
void VS1053_play(FIL* file, char* name);

void VS1053_draw(void);
uint8_t VS1053_input(uint8_t key);