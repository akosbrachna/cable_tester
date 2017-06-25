// Usage

//    unsigned char c;
//    EEPROM_putc(0x12,0x34); //Write 0x34 to EEPROM address 0x12
//    Nop();

//    c = EEPROM_getc(0x12); // Read EEPROM address 0x12 in to variable C

//This function Writes data to given address in internal EEPROM of PIC MCU 

void EEPROM_putc(unsigned char address, unsigned char data)
{
    unsigned char INTCON_SAVE;

    EEADR  = address;
    EEDATA = data;

    EECON1bits.EEPGD= 0; // 0 = Access data EEPROM memory
    EECON1bits.CFGS = 0; // 0 = Access Flash program or DATA EEPROM memory
    EECON1bits.WREN = 1; // enable writes to internal EEPROM

    INTCON_SAVE=INTCON; // Save INTCON register contants
    INTCON=0;             // Disable interrupts, Next two lines SHOULD run without interrupts
    
    EECON2=0x55;        // Required sequence for write to internal EEPROM
    EECON2=0xaa;        // Required sequence for write to internal EEPROM

    EECON1bits.WR=1;    // begin write to internal EEPROM
    INTCON=INTCON_SAVE; //Now we can safely enable interrupts if previously used
    
    Nop();

    while (PIR2bits.EEIF==0)//Wait till write operation complete
    {
        Nop();
    }

    EECON1bits.WREN=0; // Disable writes to EEPROM on write complete (EEIF flag on set PIR2 )
    PIR2bits.EEIF=0; //Clear EEPROM write complete flag. (must be cleared in software. So we do it here)

}

void EEPROM_write(unsigned char address, unsigned char data[])
{
	unsigned char i = 0;
	while(data[i]!=0)
	{
		EEPROM_putc(address+i, data[i]);
		i++;
	}
	EEPROM_putc(address+i, 0);
}

// This function reads data from address given in internal EEPROM of PIC 
unsigned char EEPROM_getc(unsigned char address)
{
    EEADR=address;
    EECON1bits.EEPGD= 0; // 0 = Access data EEPROM memory
    EECON1bits.CFGS = 0; // 0 = Access Flash program or DATA EEPROM memory
    EECON1bits.RD   = 1; // EEPROM Read
    return EEDATA;       // return data
}

void EEPROM_read(unsigned char address, unsigned char *buf, unsigned char length)
{
	unsigned char i;

	for (i = 0; i < length; i++)
	{
		*(buf+i) = EEPROM_getc(address+i);
	}
}

void EEPROM_put_int(unsigned char address, unsigned int data)
{
	unsigned char d;
	d = (unsigned char)data;
	EEPROM_putc(address, d);
	d = (unsigned char)(data>>8);
	EEPROM_putc(address+1, d);
}

unsigned int EEPROM_get_int(unsigned char address)
{
	unsigned int c, d;
	c = EEPROM_getc(address);
	d = EEPROM_getc(address+1);
	c = (d<<8)|c;
	return c;
}

void EEPROM_reset(unsigned char number_of_bytes)
{
	unsigned char i;

	for (i = 0; i < number_of_bytes; i++)
	{
		EEPROM_putc(i, 0);
	}
}
