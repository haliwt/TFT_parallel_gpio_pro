#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H
#include "main.h"

void delay_init(uint16_t sysclk);       /* ³õÊ¼»¯ÑÓ³Ùº¯Êý */
void delay_ms(uint16_t nms);            /* ÑÓÊ±nms */
void delay_us(uint32_t nus);            /* ÑÓÊ±nus */




#endif
