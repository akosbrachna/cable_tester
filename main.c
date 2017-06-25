#define elemszam(x) (sizeof(x)/sizeof(x[0]))
#define abs(x) ((x) > 0 ? (x) : -(x))
#include <p18f45k50.h>
#include <delays.h>
#include <stdlib.h>
#include "eeprom.h"
#include "config_45k50.h"
#include "lcd.h"
#include "menu.h"
#include <timers.h>
#include "std_lib.h"
#include "sort.h"
#include "wire_match.h"
#include "wire_match_calibration.h"
#include "cable_length.h"
#include <adc.h>
#include <ctmu.h>

void init(void);

unsigned char buf[8];

void main(void)
{
	init();

	while(1)
	{
		Delay10KTCYx(50);

		while (PORTCbits.RC0 == 1)
		{	
			menu(); 
		}

		Lcd_clearDisplay();
		Lcd_cursorHome();

		switch(check_switch)
		{
			case 0:
			{
				Lcd_PutMessage("Testing");
				check_wire_match();
				break;
			}
			case 1:
			{
				//Lcd_PutMessage("Orange: ");
				measure_cable_length(0b10000000);
				//Lcd_cursorHome();Lcd_newLine();
				//Lcd_PutMessage("Green: ");
				//measure_cable_length(0b10110000);
				break;
			}
			case 2:
			{
				calibrate_length();
				break;
			}
			case 3:
			{
				calibrate_wire_match();
				break;
			}
		}
	}
}

void init(void)
{
	OSCCON = 0b01010110; //4MHz orajel T = 250ns, 1 utasitas = 1us	
	TRISA  = 0b00000100;
	TRISB  = 0b00001100;
	TRISC  = 0b00000011;
	TRISD  = 0b00001111;
	LATA = 0;
	LATD = 0;
	LATD = 0b00000000;
	ANSELA = 0;
	ANSELB = 0;
	ANSELC = 0;
	ANSELD = 0;

	CM1CON0 = 0;
	CM2CON0 = 0;

	ADCON1 = 0;				//VSS,VDD ref.
	//ADCON2 = 0b00010001;;	//ADCON2 setup: Left justified, Tacq=4Tad, Tad=Fosc/8)
	ADCON2 = 0b00101111;	//ADCON2 setup: Left justified, Tacq=12Tad, Tad=FRC)

	CTMUCONH = 0x00; //make sure CTMU is disabled
	CTMUCONL = 0b11011100;
	CTMUICON = 0b01111111; //55uA, Nominal - Max positive Adjustment

	Lcd_init();

	measure_battery();
	//EEPROM_reset(50);
	copy_eeprom_to_buffer(); //copy calibrated length data to buffer
	copy_eeprom_wire_match_to_buffer();
	display_main_menu();
}
