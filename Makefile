# MPLAB IDE generated this makefile for use with GNU make.
# Project: rx.mcp
# Date: Mon Jun 16 20:40:02 2014

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

rx.cof : main.o
	$(LD) /p18F45K50 /l"C:\Program Files\Microchip\mplabc18\v3.46\lib" "main.o" /u_CRUNTIME /z__MPLAB_BUILD=1 /o"rx.cof" /M"rx.map" /W

main.o : main.c ../../Program\ Files/Microchip/mplabc18/v3.46/h/delays.h ../../Program\ Files/Microchip/mplabc18/v3.46/h/stdlib.h lcd.h ../../Program\ Files/Microchip/mplabc18/v3.46/h/timers.h std_lib.h master.h sort.h wire_match.h cable_length.h ../../Program\ Files/Microchip/mplabc18/v3.46/h/adc.h ../../Program\ Files/Microchip/mplabc18/v3.46/h/ctmu.h main.c ../../Program\ Files/Microchip/mplabc18/v3.46/h/p18f45k50.h ../../Program\ Files/Microchip/mplabc18/v3.46/h/p18cxxx.h config_45k50.h ../../Program\ Files/Microchip/mplabc18/v3.46/h/pconfig.h
	$(CC) -p=18F45K50 "main.c" -fo="main.o" -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "main.o" "rx.cof" "rx.hex"

