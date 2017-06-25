#pragma config PLLSEL   = PLL3X     // 3X PLL multiplier selected
#pragma config CFGPLLEN = OFF       // PLL turned on during execution
#pragma config CPUDIV   = NOCLKDIV  // 1:1 mode (for 48MHz CPU)
#pragma config LS48MHZ  = SYS48X8   // Clock div / 8 in Low Speed USB mode
#pragma config FOSC     = INTOSCIO  // INTOSCCLKO HFINTOSC selected at powerup, no clock out
#pragma config PCLKEN   = OFF       // Primary oscillator driver
#pragma config FCMEN    = OFF       // Fail safe clock monitor
#pragma config IESO     = OFF       // Internal/external switchover (two speed startup)
#pragma config nPWRTEN  = OFF       // Power up timer
#pragma config BOREN    = SBORDIS   // BOR enabled
#pragma config nLPBOR   = ON        // Low Power BOR
#pragma config WDTEN    = SWON      // Watchdog Timer controlled by SWDTEN
#pragma config WDTPS    = 32768     // WDT postscalar
#pragma config PBADEN   = OFF       // Port B Digital/Analog Powerup Behavior
#pragma config SDOMX    = RC7       // SDO function location
#pragma config LVP      = OFF       // Low voltage programming
#pragma config MCLRE    = ON        // MCLR function enabled (RE3 disabled)
#pragma config STVREN   = ON        // Stack overflow reset
//#pragma config ICPRT  = OFF       // Dedicated ICPORT program/debug pins enable
#pragma config XINST    = OFF       // Extended instruction set