//adc_defines.h

//clock defines
/*#define FOSC 12000000 //Frequency of Oscillator
#define CCLK (FOSC*5) //CPU Clock
#define PCLK (CCLK/4) //Peripheral Clock*/
#include "rtc_defines.h"
#define ADDCLK 3000000 //ADC Clock
#define CLKDIV ((PCLK/ADDCLK)-1)
//ADCR sfr defines
#define CLKDIV_BITS 8
#define ADC_START_BIT 24
#define PDN_BIT 21
//ADDR sfr defines
#define RESULT_BITS 6
#define DONE_BIT 31
