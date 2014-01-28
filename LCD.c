/*
 * LCD.c
 *
 *  Created on: Oct 9, 2013
 *      Author: Ray
 */

#include <msp430.h>
#include "LCD.h"
#include <stdio.h>
#include <string.h>

/*
				LCD	|	F	|	MSP_PIN
			-----------------------------
			PIN_4	|	RS	|	P4.4
			PIN_5	|	RW	|	GND
			PIN_6	|	E	|	P4.5
			PIN_14	|	DB7	|	P4.3
			PIN_13	|	DB6	|	P4.2
			PIN_12	|	DB5	|	P4.1
			PIN_11	|	DB4	|	P4.0



*/

extern char uf_lcd_temp;
extern char uf_lcd_temp2;
extern char uf_lcd_x;
extern int i;

int fputc(int _c, register FILE *_fp)						//function necessary to use printf
{
	lcd_char((unsigned char) _c);
	return((unsigned char)_c);
}

int fputs(const char *_ptr, register FILE *_fp)				//function necessary to use printf
{
  unsigned int i, len;

  len = strlen(_ptr);

  for(i=0 ; i<len; i++)
  {
    lcd_char((unsigned char) _ptr[i]);
  }

  return len;
}

void gen_init()
{
	WDTCTL = WDTPW + WDTHOLD;  				// Stop watchdog timer
	P4DIR = 0xff;							//Using this port to drive the LCD
	P1DIR = 1;								//Debug LED
}

void clk_init()
{
	DCOCTL = CALDCO_16MHZ; 					// using internal 16MHz internal osc
	BCSCTL1 = CALBC1_16MHZ; 				// given that the lcd is slow and the default clk is at 0.5MHz
}

//**********************************************************
void ADC_Init()
{
	ADC10CTL1 |= CONSEQ1 ;						//Continuous sample mode
	ADC10CTL0 |= ADC10SHT_2 + ADC10ON + MSC ;	//Sample and hold time, ADC on, cont. sample
	ADC10AE0 |= 0x01;							//Select Channel A0
	ADC10CTL0 |= ADC10SC + ENC;					//Start conversions
}

//**********************************************************
void lcd_command(char uf_lcd_x){
	 //P4DIR = 0xFF;						//lets try it for example 0x43
	 uf_lcd_temp = uf_lcd_x;				//making a copy of the pass parameter
	 P4OUT = 0x00;							//sending 0 out
	 __delay_cycles(22000);					//delay
	 uf_lcd_x = uf_lcd_x >> 4; 				//shifting the command 4 right to send the MSNibble ||0x43 = 0011_0011 >> 4 = 0000_0011
	 uf_lcd_x = uf_lcd_x & 0x0F; 			//clearing out the 4 MSB							||0x04 & 0x0f = 0x24;
	 uf_lcd_x = uf_lcd_x | 0x20;			//setting high only bits 4&5						||0x24 | 0x20 = 0x24;
	 P4OUT =  uf_lcd_x;						//sending that out									||MCU>>0x24>>LCD
	 __delay_cycles(22000);					//													wait
	 uf_lcd_x = uf_lcd_x & 0x0F;			//													||0x24 & 0x0f = 0x04
	 P4OUT =  uf_lcd_x;						//													||MCU>>0x04>>LCD
	 __delay_cycles(22000);					//													wait
	 P4OUT = 0x00;							//													||MCU>>0>>LCD
	 __delay_cycles(22000);					//													wait
	 uf_lcd_x = uf_lcd_temp;				//													copy the 0x43 back
	 uf_lcd_x = uf_lcd_x & 0x0F;			//													||0x43 & 0x0f = 0x03
	 uf_lcd_x = uf_lcd_x | 0x20;			//													||0x03 | 0x20 = 0x23
	 P4OUT = uf_lcd_x; 						//													||MCU>>0x23>>LCD

	 __delay_cycles(22000);					//													wait
	 uf_lcd_x = uf_lcd_x & 0x0F;			//													||0x23 & 0x0f = 0x03
	 P4OUT = uf_lcd_x;						//													||||MCU>>0x03>>LCD
	 __delay_cycles(22000);					//													wait
}


//**********************************************************
void lcd_init(void){
	 lcd_command(0x33);
	 lcd_command(0x32);
	 lcd_command(0x2C);
	 lcd_command(0x0C);
	 lcd_command(0x01);
}

//**********************************************************
//void lcd_string(char transmit[])
//{
//	for(i = 18; transmit[i] != '\0'; i--)
//    {
//    	lcd_char(transmit[i]);
//    	__delay_cycles(22000);
//    }
//}

//**********************************************************

void lcd_char(char uf_lcd_x){
	 //P4DIR = 0xFF;									//example uf_lcd_x = 0x88
	 uf_lcd_temp = uf_lcd_x;						//
	 P4OUT = 0x10;									//
	 __delay_cycles(22000);
	 uf_lcd_x = uf_lcd_x >> 4;
	 uf_lcd_x = uf_lcd_x & 0x0F;
	 uf_lcd_x = uf_lcd_x | 0x30;
	 P4OUT =  uf_lcd_x;
	 __delay_cycles(22000);
	 uf_lcd_x = uf_lcd_x & 0x1F;
	 P4OUT =  uf_lcd_x;
	 __delay_cycles(22000);
	 P4OUT = 0x10;
	 __delay_cycles(22000);
	 uf_lcd_x = uf_lcd_temp;
	 uf_lcd_x = uf_lcd_x & 0x0F;
	 uf_lcd_x = uf_lcd_x | 0x30;
	 P4OUT = uf_lcd_x;
	 __delay_cycles(22000);
	 uf_lcd_x = uf_lcd_x & 0x1F;
	 P4OUT = uf_lcd_x;
	 __delay_cycles(22000);

}
