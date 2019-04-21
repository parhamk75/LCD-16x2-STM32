#include "lm016.h"



void send_cmd(lcd_t * lcd, uint8_t cmd)
{
	HAL_GPIO_WritePin(lcd->rs_port, lcd->rs_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_SET);
	
	if(lcd->mode == _8_BIT)
	{
		for( uint8_t i=0; i < 8; i++)
		{
			HAL_GPIO_WritePin(lcd->data_ports[i], lcd->data_pins[i], (cmd>>i)%2 );
		}
		HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_RESET);
	}
	else if(lcd->mode == _4_BIT)
	{
		for( uint8_t i=4; i < 8; i++)
		{
			HAL_GPIO_WritePin(lcd->data_ports[i], lcd->data_pins[i], (cmd>>i)%2 );
		}
		HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_RESET);
		HAL_Delay(1); //It should be 10ns minimum
		HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_SET);
		
		for( uint8_t i=0; i < 4; i++)
		{
			HAL_GPIO_WritePin(lcd->data_ports[i+4], lcd->data_pins[i+4], (cmd>>i)%2 );
		}
		HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_RESET);
	}	
	HAL_Delay(1); //It should be 10ns minimum
		
}

void lcd_init(lcd_t * lcd)
{
	//TO-DO gather the ports (for loop)
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	GPIO_InitTypeDef GI;
	
	GI.Mode  = GPIO_MODE_OUTPUT_PP;
	GI.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GI.Pull  = GPIO_NOPULL;
	
	for( uint8_t i=0; i<8; i++)
	{
		GI.Pin  = lcd->data_pins[i];
		HAL_GPIO_Init(lcd->data_ports[i], &GI);
	}
	
	GI.Pin  = lcd->rs_pin;
	HAL_GPIO_Init(lcd->rs_port, &GI);
	
	GI.Pin  = lcd->en_pin;
	HAL_GPIO_Init(lcd->en_port, &GI);
	
	HAL_Delay(40);  //Can be omitted if it isn't the first function in your code
	if(lcd->mode == _8_BIT)
	{
		send_cmd(lcd, 0x38);
	}
	else if(lcd->mode == _4_BIT)
	{
		send_cmd(lcd, 0x28);
	}
	
	send_cmd(lcd, 0x01);
	send_cmd(lcd, 0x06);
	send_cmd(lcd, 0x0E);
	
}	
	
void lcd_putchar(lcd_t * lcd, uint8_t character)
{
	HAL_GPIO_WritePin(lcd->rs_port, lcd->rs_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_SET);
	
	if(lcd->mode == _8_BIT)
	{
		for( uint8_t i=0; i < 8; i++)
		{
			HAL_GPIO_WritePin(lcd->data_ports[i], lcd->data_pins[i], (character>>i)%2 );
		}
		HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_RESET);
	}
	else if(lcd->mode == _4_BIT)
	{
		for( uint8_t i=4; i < 8; i++)
		{
			HAL_GPIO_WritePin(lcd->data_ports[i], lcd->data_pins[i], (character>>i)%2 );
		}
		HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_RESET);
		HAL_Delay(1); //It should be 10ns minimum
		HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_SET);
		
		for( uint8_t i=0; i < 4; i++)
		{
			HAL_GPIO_WritePin(lcd->data_ports[i+4], lcd->data_pins[i+4], (character>>i)%2 );
		}
		HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin, GPIO_PIN_RESET);
	}	
	HAL_Delay(1); //It should be 10ns minimum
		
}

void lcd_clear(lcd_t * lcd)
{
	send_cmd(lcd, 0x01);
}

void lcd_puts(lcd_t * lcd, char *str)
{
	uint8_t tmp_cntr = 1;
	while (*str != 0)
	{
		if(tmp_cntr > 16)
		{
			lcd_set_curser(lcd, 2, 1);
			tmp_cntr = 0;
		}
		tmp_cntr++;
		lcd_putchar(lcd, *str);
		str++;
		HAL_Delay(1);		
	}
}

void lcd_set_curser(lcd_t * lcd, uint16_t row, uint16_t col)
{
	uint8_t firstCharAdr[] = { 0x80, 0xC0 };
	send_cmd(lcd, firstCharAdr[row-1] + col - 1);
	HAL_Delay(1);
}