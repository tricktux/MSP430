/*
 * LCD_H.h
 *
 *  Created on: Oct 9, 2013
 *      Author: Ray
 */

#include <stdio.h>
#include <string.h>

#ifndef LCD_H
#define LCD_H

void gen_init();
void ADC_Init();
int fputc(int _c, register FILE *_fp);
int fputs(const char *_ptr, register FILE *_fp);
void clk_init();

void lcd_command(char uf_lcd_x);
void lcd_init(void);
void lcd_char(char uf_lcd_x);

#endif /* LCD_H */
