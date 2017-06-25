#define last_menu 3
unsigned char check_switch = 0;
unsigned char down_up = 0;

void display_main_menu(void)
{
    rom char *menu_options[] = {"Wire Match", "Cable Length", "Calibration ", "WM Calibration "};

	Lcd_clearDisplay();
	Lcd_cursorHome();
	Lcd_PutMessage(menu_options[check_switch]);
	Lcd_cursorHome();Lcd_newLine();
	Lcd_PutMessage("Press Start!");
}

void menu(void)
{
	if (PORTCbits.RC1 == 1)
	{
		down_up = 0;
		return;
	}
	if (down_up == 1)
		return;

	down_up = 1;

	if (check_switch == last_menu)
		check_switch = 0;
	else check_switch++;

	display_main_menu();
}
