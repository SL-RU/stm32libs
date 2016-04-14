#ifndef KEYBOARD_H
#define KEYBOARD_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

#include "adc.h"
#include <stdio.h>


#define keyboard_hadc hadc1
#define keyboard_adcDeltaError 150

#define keyboard_on GPIOB, GPIO_PIN_2
	
	
void keyboard_init(void);

void keyboard_update(void);

void keyboard_setPressHandler(void (*KeyPressHandler)(int8_t));
	
void keyboard_handle(int8_t key);

int8_t keyboard_adc_to_lvl(uint16_t adc);

int8_t keyboard_lvl_to_key(int8_t left, int8_t right);

char keyboard_key_to_char(int8_t key);


/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
