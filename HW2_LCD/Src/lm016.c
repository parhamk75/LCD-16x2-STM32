#include "lm016.h"



void send_cmd(lcd_t * lcd, uint8_t cmd)
{
	HAL_GPIO_WritePin(lcd->rs_port, lcd->rs_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_SET);
	
	if(lcd->mode == _8_BIT)
	{
	}
	else if(lcd->mode == _4_BIT)
	{
	}
}

	
	