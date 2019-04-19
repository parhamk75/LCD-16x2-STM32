/* Define to prevent recursive inclusion -------------------------------*/
#ifndef __LM016__
#define __LM016__
/* Includes ------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
typedef enum
{
	_8_BIT = 1,
	_4_BIT = 0
}lcd_dl_t;

typedef struct{
GPIO_TypeDef* data_ports[8];
GPIO_TypeDef* en_port;
GPIO_TypeDef* rs_port;
GPIO_TypeDef* rw_port;
uint16_t data_pins[8];
uint16_t en_pin;
uint16_t rs_pin;
uint16_t rw_pin;	
lcd_dl_t mode;
}lcd_t;


void lcd_init(lcd_t * lcd);
void lcd_putchar(lcd_t * lcd, uint8_t character);
void lcd_set_curser(lcd_t * lcd, uint16_t row, uint16_t col);
void lcd_clear(lcd_t * lcd);
void lcd_puts(lcd_t * lcd, char *str);

void send_cmd(lcd_t * lcd, uint8_t cmd);

#endif

