void int_to_string(int data, unsigned char buf[])
{
int mx = 10000;
unsigned char i = 0;
unsigned char c = 0;

	if (data == 0)
	{
		buf[c] = 48;
		c++;
	}
	else
	{
		while(i<5)
		{
		 	if (data>=mx)
			{
				buf[c] = (data/mx)%10 + 48;
				c++;
			}
			mx = mx/10;
			i++;
		}
	}
	buf[c] = 0;
}

unsigned int adc_conv(void)
{
	unsigned int data = 0;

	ADCON0bits.ADON = 1;

	ADRESH=0;					//clear the ADC result register
	ADRESL=0;					//clear the ADC result register

	ADCON0bits.GO = 1;			//Start A/D Conversion
	while(ADCON0bits.NOT_DONE);	//Loop here

	data = ADRESH;
	data = data<<2;
	data |= ADRESL>>6;

	return data;
}

unsigned char float_to_string(int data, unsigned char buf[], unsigned char unit)
{
unsigned char i = 0;
unsigned char value = 0;

	if(data>0)
	{
		if (data/10000)
		{
			value = (data/10000)%10;
			buf[i] = value + 48;
			i++;
		}
		if (data/1000)
		{
			value = (data/1000)%10;
			buf[i] = value + 48;
			i++;
		}
		if (data/100)
		{
			value = (data/100)%10;
			buf[i] = value + 48;
			i++;
		}
		if (i == 0)
		{ 
			buf[i] = '0';
			i++;
		}
		buf[i] = '.';
		i++;
		if (data/10)
		{
			value = (data/10)%10;
			buf[i] = value + 48;
			i++;
		}
		else
		{ 
			buf[i] = '0';
			i++;
		}
		if (data)
		{
			value = data%10;
			buf[i] = value + 48;
			i++;
		}
		buf[i] = unit;
		i++;
		buf[i] = 0;
		return 1;
	}
	else return 0;
}

void reset_array(unsigned char array[], unsigned char number_of_elements)
{
unsigned char i;

	for (i = 0; i < number_of_elements; i++)
	{
		array[i] = 0;
	}
}

void measure_battery(void)
{
unsigned char buf[8];
unsigned int data;

	TRISAbits.TRISA2 = 1;
	TRISAbits.TRISA3 = 0;
	ADCON0 = 0b00001000; // channel selecteion to RA2 = AN2

	LATAbits.LATA3 = 1;
	Delay10KTCYx(50);

	data = (int)adc_conv()/1.048472;
	//data = adc_conv();

	LATAbits.LATA3 = 0;

	//int_to_string(data, buf);
	float_to_string(data, buf, 'V');
	Lcd_clearDisplay();
	Lcd_cursorHome();
	Lcd_PutMessage("Battery: ");
	Lcd_PutMessageArray(buf);

	if (data < 650) data = 651;
	data = (int)(10000.0*((data-650.0)/(900.0-650.0)));
	if (data > 10000) data = 10000;
	float_to_string(data, buf, '%');
	Lcd_cursorHome();Lcd_newLine();
	Lcd_PutMessage("Left: ");
	Lcd_PutMessageArray(buf);

	Delay10KTCYx(3000);
}
