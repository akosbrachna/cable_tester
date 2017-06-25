
//LCD (PortB)
#define  E LATAbits.LATA0
//#define  R_W LATBbits.LATB6
#define  RS  LATAbits.LATA1
#define LCDdata LATB
#define rw_delay 20
 
void command(unsigned char);
void write(unsigned char);
void Nybble(unsigned char);
void Lcd_init(void);
void Lcd_PutMessage(rom char *);
void Lcd_PutMessageArray(unsigned char[]); 
void Lcd_PutMessageChar(unsigned char);
 
//Write a string to the LCD
void Lcd_PutMessage(rom char *Message){
rom char *Pos = Message;
    while(*Pos!=0)
        write(*Pos++);
}

void Lcd_PutMessageChar(unsigned char Message){
unsigned char *Pos = Message;
    while(*Pos!=0)
        write(*Pos++);
}

void Lcd_PutMessageArray(unsigned char m[]){
unsigned char Pos = 0;
    while(m[Pos]!=0)
	{
        write(m[Pos]);
		Pos++;
	}
}

/**********************************************************/
//4-bit methods for LCD
/**********************************************************/
void command(unsigned char i)
{
    RS =0;
    //R_W =0;          //R/W=LOW : Write
    Nybble(i);    		//Send upper 4 bits
    Nybble(i<<4);       //Send lower 4 bits
    Delay1KTCYx(2);  	//must wait at least 2mS (2*1000*4/1e6 = 8ms used)
}
 
void write(unsigned char i)
{
    RS =1;
    //R_W =0;          //R/W=LOW : Write
    Nybble(i);    		//Send upper 4 bits
    Nybble(i<<4);       //Send lower 4 bits
    Delay1KTCYx(2);  	//must wait 2mS
}
 
/**********************************************************/
void Nybble(unsigned char dat)
{
    dat &= 0xf0;             //clear bottom bits of dat
    LCDdata &= 0x0f;         //clear top bits of port (interested only in DB7-DB4)
    LCDdata |= dat;          //or the two and store at port
    E = 1;
    Delay1TCY();             //enable pulse width >= 300ns (used 4uS)
    E = 0;                   //Clock enable: falling edge
}
 
/**********************************************************/
void Lcd_init(void)
{
    LCDdata=0x00;
    Delay1KTCYx(15);    //Wait >15 msec after power is applied (used 20mS)
    Nybble(0x3);    //command 0x30 = Wake up
    Delay1KTCYx(5);     //must wait 160us, busy flag not available (used 160uS)
    Nybble(0x3);    //command 0x30 = Wake up #2
    Delay1KTCYx(5);     //must wait 160us, busy flag not available (used 160uS)
 
    command(0x20);      //Function set: 4-bit/2-line
	Delay1KTCYx(1);
    command(0x2c);      //Function set: 4-bit/2-line
	Delay1KTCYx(1);
    command(0x10);      //Set cursor
	Delay1KTCYx(1);
    command(0x01);      //Clear Display (Added)
	Delay1KTCYx(1);
    command(0x06);      //Entry Mode set
	Delay1KTCYx(1);
    command(0x0c);
}

void Lcd_clearDisplay(void)
{
	command(0x01);
	Delay1KTCYx(1);
}

void Lcd_cursorHome(void)
{
	command(0x02);
	Delay1KTCYx(1);
}

void Lcd_newLine(void)
{
	command(0xc0);
	Delay1KTCYx(1);
}

/**********************************************************/
//End methods for LCD
/**********************************************************/