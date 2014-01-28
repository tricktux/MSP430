#include <msp430.h> 
#include "LCD.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

/*
 * main.c
 */

long int R2const;		//37k == r2 ==378510000 | 66k = 67518000 | 33k = 33759000 | 50k = 52045125

char uf_lcd_temp;
char uf_lcd_temp2;
char uf_lcd_x;
long int results = 0;
int i;
long int R2;


int main(void)
{
	gen_init();
	clk_init();
	__delay_cycles(250000);					//let the new oscilator settle
	lcd_init();
	ADC_Init();
	__delay_cycles(250000);

for(;;)
	{
		for(i = 0; i<16; i++)
		{
			results = results + ADC10MEM;
		}
		results =  results/16;					//Averaging the results

		//results = ADC10MEM;
		//printf("ADC = %ld", results);				//displaying ADC
		//results = ((R2*1023)/results)-R2;			//current pot position 1.5k | nevermind this right now
													//currently trying a physical 180k resistance
													//make everything a long int
		__delay_cycles(16*4*250000);				//waiting for 4 seconds
		lcd_command(0x01);

		if(results <100) {R2const = 54784719; R2 = 50000;}
		if(results >100) {R2const = 37477605; R2 = 37000;}
		if(results >500) {R2const = 31680264; R2 = 30968;}

		results = R2const/results - R2;					//dividing by ADC value

		//printf("R2*1023/ADC = %ld", results);
		//__delay_cycles(16*4*250000);
		//lcd_command(0x01);

		//results = results - 37000;
		//printf("R2 = %ld", results);

		//the equation is ADC = 1023*(Vin-Vr-)/(Vr+-Vr-)
		//R2 = R1/(Vin/Vout-1) | R1 = 37k

		if(results > 1000000 | results < 500) printf("Out of Range");


		else
		{
			printf("R2 = %ld Ohms", results);		//Delta = 5/1023 = 0.0049*ADC10MEM = Vout
		}
		P1OUT ^= 1;
		 __delay_cycles(16*4*250000);
		 lcd_command(0x01);						//clearing up LCD
		 results =0;
	}
}
