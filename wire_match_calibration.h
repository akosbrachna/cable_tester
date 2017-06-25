void calibrate_wire_match(void)
{
	unsigned char rd0_3[] = {0b01010001, 0b01010101, 0b01011001, 0b01011101};
	unsigned char rd4_7[] = {0b10000000, 0b01000000, 0b00100000, 0b00010000};
	unsigned char i;
	unsigned int out[] = {0,0,0,0};	// fesz ertekek a kimeneti jel sorrendjeben
	unsigned int value;
	unsigned char buf[] = {0,0,0,0,0,0,0,0};

	TRISD = 0b00001111;
	// testing
	Lcd_clearDisplay();
	Lcd_cursorHome();
	for (i = 0; i < 4; i++)
	{
		LATD  = rd4_7[i];
		Delay1KTCYx(100);			// @4MHz clock - 1ms
	
		ADCON0 = rd0_3[i];
		out[i] = adc_conv();
	}
	TRISD = 0b11111111;
	LATD  = 0b00000000;

	wire_match_ADC[0] = out[0] - (int)((out[1] - out[0]) / 2);
	wire_match_ADC[1] = out[0] + (int)((out[1] - out[0]) / 2);
	wire_match_ADC[2] = out[1] + (int)((out[2] - out[1]) / 2);
	wire_match_ADC[3] = out[2] + (int)((out[3] - out[2]) / 2);

	EEPROM_put_int(50, wire_match_ADC[0]);
	EEPROM_put_int(52, wire_match_ADC[1]);
	EEPROM_put_int(54, wire_match_ADC[2]);
	EEPROM_put_int(56, wire_match_ADC[3]);
	EEPROM_putc(0xFC, 0x55);
	EEPROM_putc(0xFD, 0xAA);

	Lcd_clearDisplay();
	Lcd_cursorHome();
	Lcd_PutMessage("done");
}
