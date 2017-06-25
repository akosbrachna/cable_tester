unsigned int ethernet_cable_length[]={500, 1000, 2500, 5000, 7500, 10000};
unsigned int ethernet_cable_ADC[6];
unsigned char calibration_check_switch;
const unsigned char calibration_last_menu = 7;
const unsigned char calibration_reset_menu = 6;
unsigned char calibration_down_up;

void copy_eeprom_to_buffer(void)
{
unsigned char i;
unsigned char check[2];

	check[0] = EEPROM_getc(0xFE);
	check[1] = EEPROM_getc(0xFF);
	if (check[0] == 0x55 && check[1] == 0xAA)
	{
		for(i = 0; i < elemszam(ethernet_cable_ADC); i++)
		{
			ethernet_cable_ADC[i] = EEPROM_get_int(i*2);
		}
	}
	else
	{
		for(i = 0; i < elemszam(ethernet_cable_ADC); i++)
		{
			ethernet_cable_ADC[i] = 0;
			EEPROM_put_int(i*2, 0);
		}
		EEPROM_putc(0xFE, 0x55);
		EEPROM_putc(0xFF, 0xAA);
	}

}

unsigned int adc_to_length(unsigned int data)
{
float factory_length = 12.20476;
float adat;
unsigned char i;
unsigned char min_index = 100;
unsigned int min = 10000;

	for (i = 0; i < elemszam(ethernet_cable_ADC); i++)
	{
		if (ethernet_cable_ADC[i] == 0) continue;
		if (min > abs(data - ethernet_cable_ADC[i]))
		{
			min = abs(data - ethernet_cable_ADC[i]);
			min_index = i;
		}
	}
	if (min_index != 100)
	{
		adat = data*((float)ethernet_cable_length[min_index]/(float)ethernet_cable_ADC[min_index]);
	}
	else{
		adat = data*factory_length;
	}
	return (int)adat;
}

void measure_cable_length(unsigned char channel)
{
	unsigned char i = 0;
	unsigned int data[10];
	unsigned int value;
	unsigned char minmax_index[2];
	unsigned char buf[8];
	unsigned char tris_d;

	tris_d = TRISD;
	TRISD = 0b00000000;

	ADCON0 = 0b01110101;
	CTMUCONHbits.CTMUEN = 1; 		//Enable the CTMU

	Delay10TCYx(50);

	while(i < elemszam(data))
	{	
		CTMUCONLbits.EDG1STAT = 0; 	//Set Edge status bits to zero
		CTMUCONLbits.EDG2STAT = 0;
		CTMUCONHbits.IDISSEN = 1; 	//drain charge on the circuit
		Delay10TCYx(100); 			//wait for 1ms 4Mhz clock
		CTMUCONHbits.IDISSEN = 0; 	//end drain of circuit
	
		CTMUCONHbits.EDGEN = 1;
		LATD = channel;
		
		while(CTMUCONLbits.EDG2STAT == 0);
		data[i] = adc_conv();
		LATD = 0b00000000;
		Delay1KTCYx(50);			//50ms
		i++;
	}
	CTMUCONHbits.CTMUEN = 0; 		//Disable the CTMU
	TRISD = tris_d;
	LATD = 0b00000000;

	sort(data, elemszam(data));
	stat(data, elemszam(data), minmax_index);
	value = average(data, minmax_index);
	value = adc_to_length(value);
	if ((minmax_index[1]-minmax_index[0])<5)
	{
		Lcd_clearDisplay();Lcd_cursorHome();
		Lcd_PutMessage("Erronous");
		//Lcd_cursorHome();Lcd_newLine();
		//Lcd_PutMessage("Please try again");
		return;
	}
	if (float_to_string(value, buf, 'm'))
		Lcd_PutMessageArray(buf);
	else {
		Lcd_clearDisplay();
		Lcd_PutMessage("Error");
		//Lcd_cursorHome();Lcd_newLine();
		//Lcd_PutMessage("Please try again");
	}

	Delay10KTCYx(5);
}

void display_calibration_menu(void)
{
const char *length_values[] = {"5 meters", "10 meters", "25 meters", "50 meters", "75 meters", "100 meters", "reset", "exit"};
	Lcd_clearDisplay();
	Lcd_cursorHome();
	Lcd_PutMessage("Choose length: ");
	Lcd_cursorHome();Lcd_newLine();
	if (ethernet_cable_ADC[calibration_check_switch] && calibration_check_switch < calibration_reset_menu)
	{
		Lcd_PutMessage("* ");
	}
	Lcd_PutMessage(length_values[calibration_check_switch]);
}

void calibration_menu(void)
{
	if (PORTCbits.RC1 == 1)
	{
		calibration_down_up = 0;
		return;
	}
	if (calibration_down_up == 1)
		return;

	calibration_down_up = 1;
	if (calibration_check_switch != calibration_last_menu)
		calibration_check_switch++;
	else calibration_check_switch = 0;

	display_calibration_menu();
}

void make_calibration(void)
{
	unsigned char i = 0;
	unsigned int data[10];
	unsigned int value;
	unsigned char minmax_index[2];
	unsigned char buf[8];

	TRISD  = 0b00000000;
	LATD   = 0b00000000;
	ADCON0 = 0b01110101;

	CTMUCONHbits.CTMUEN = 1; 		//Enable the CTMU

	Delay10TCYx(50);

	while(i < elemszam(data))
	{	
		CTMUCONLbits.EDG1STAT = 0; 	//Set Edge status bits to zero
		CTMUCONLbits.EDG2STAT = 0;
		CTMUCONHbits.IDISSEN = 1; 	//drain charge on the circuit
		Delay10TCYx(100); 			//wait for 1ms 4Mhz clock
		CTMUCONHbits.IDISSEN = 0; 	//end drain of circuit
	
		CTMUCONHbits.EDGEN = 1;
		LATDbits.LATD7 = 1;
		
		while(CTMUCONLbits.EDG2STAT == 0);

		data[i] = adc_conv();
		LATDbits.LATD7 = 0;

		Delay1KTCYx(50);			//50ms
		i++;
	}
	LATD  = 0b00000000;
	CTMUCONHbits.CTMUEN = 0; 		//Disable the CTMU

	sort(data, elemszam(data));
	stat(data, elemszam(data), minmax_index);
	value = average(data, minmax_index);
	EEPROM_put_int(calibration_check_switch*2, value);
	ethernet_cable_ADC[calibration_check_switch] = value;

	Delay10KTCYx(5);
}

void calibrate_length(void)
{
	unsigned int value;
	unsigned char i;

	calibration_check_switch = 0;
	calibration_down_up = 0;

	display_calibration_menu();

	while (PORTCbits.RC0 == 0){}

	while (PORTCbits.RC0 == 1)
	{	
		calibration_menu(); 
	}
	if (calibration_check_switch < calibration_reset_menu) 
	{
		make_calibration();
	}
	if (calibration_check_switch == calibration_reset_menu) 
	{
		for(i = 0; i < elemszam(ethernet_cable_ADC); i++)
		{
			ethernet_cable_ADC[i] = 0;
			EEPROM_put_int(i*2, 0);
		}
	}
	display_main_menu();
}
