unsigned int wire_match_ADC[] = {100, 300, 400, 500};

void copy_eeprom_wire_match_to_buffer(void)
{
unsigned char i;
unsigned char check[2];

	check[0] = EEPROM_getc(0xFC);
	check[1] = EEPROM_getc(0xFD);
	if (check[0] == 0x55 && check[1] == 0xAA)
	{
		for(i = 0; i < elemszam(wire_match_ADC); i++)
		{
			wire_match_ADC[i] = EEPROM_get_int(50+(i*2));
		}
	}
	else
	{
		for(i = 0; i < elemszam(wire_match_ADC); i++)
		{
			EEPROM_put_int(50+(i*2), wire_match_ADC[i]);
		}
		EEPROM_putc(0xFC, 0x55);
		EEPROM_putc(0xFD, 0xAA);
	}

}

void display_result(unsigned char buf[])
{
	Lcd_clearDisplay();
	Lcd_cursorHome();
	Lcd_PutMessage("M: 12345678");
	Lcd_cursorHome();Lcd_newLine();
	Lcd_PutMessage("R:");
	Lcd_PutMessageArray(buf);
}

void check_wire_match(void)
{
	unsigned char rd0_3[] = {0b01010001, 0b01010101, 0b01011001, 0b01011101};
	unsigned char rd4_7[] = {0b10000000, 0b01000000, 0b00100000, 0b00010000};
	unsigned char i, j;
	unsigned int out[] = {0,0,0,0};	// fesz ertekek a kimeneti jel sorrendjeben
	unsigned char in[] = {0,0,0,0};	// a fesz ertekhez tartozo bemenet
	unsigned char w_out[] = {2,6,4,8};
	unsigned char w_in[] = {1,3,5,7};
	unsigned char buf[] = {' ',0,0,0,0,0,0,0,0,0};
	unsigned int value;

	TRISD = 0b00001111;
	// testing
	for (i = 0; i < 4; i++)
	{
		LATD  = rd4_7[i];
		Delay1KTCYx(100);			// @4MHz clock - 1ms
	
		for (j = 0; j < 4; j++)
		{
			ADCON0 = rd0_3[j];
			value = adc_conv();
			if (value > out[i])
			{
				out[i] = value;
				in[i] = w_in[j];
			}
		}
	
		if (out[i] > wire_match_ADC[0] && out[i] < wire_match_ADC[1])
		{
			buf[in[i]] = '1';
			buf[w_out[i]] = '2';
		}
		if (out[i] > wire_match_ADC[1] && out[i] < wire_match_ADC[2])
		{
			buf[in[i]] = '3';
			buf[w_out[i]] = '6';
		}
		if (out[i] > wire_match_ADC[2] && out[i] < wire_match_ADC[3])
		{
			buf[in[i]] = '5';
			buf[w_out[i]] = '4';
		}
		if (out[i] > wire_match_ADC[3])
		{
			buf[in[i]] = '7';
			buf[w_out[i]] = '8';
		}
	}
	TRISD = 0b11111111;
	LATD  = 0b00000000;
	for (i = 1; i < 9; i++)
	{
		if (buf[i] == 0)
			buf[i] = 'X';
	}

	display_result(buf);
}
