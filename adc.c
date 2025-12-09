#include <LPC21xx.h>
#include "types.h"
#include "pin_connect_block.h"
#include "pin_function_defines.h"
#include "adc_defines.h"
#include "delay.h"

void Init_ADC(void)
{
	//cfg ADCR sfr
	ADCR|=(1<<PDN_BIT)|(CLKDIV<<CLKDIV_BITS);
	//cfg p0.27 pin as Analog Input pin0 (AIN0)
	CfgPortPinFunc(0,28,PIN_FUNC2);
}

void Read_ADC(u32 chNo,u32 *adcDVal,f32 *eAR)
{
	//clear any previous channel selection
	ADCR&=0xFFFFFF00;
	//update req channel select,
	//start analog samping & conversion
	ADCR|=1<<chNo|1<<ADC_START_BIT;
	delayUS(3);
	while(((ADDR>>DONE_BIT)&1)==0);
	ADCR&=~(1<<ADC_START_BIT);
	*adcDVal=(ADDR>>RESULT_BITS)&1023;
	*eAR = (*adcDVal * (3.3/1023));
}
