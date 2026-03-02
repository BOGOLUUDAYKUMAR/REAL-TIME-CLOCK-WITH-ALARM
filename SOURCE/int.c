#include "header.h"

#define EINT0_VIC_CHN0 14

void SetTime(void);
void SetDate(void);
void SetAlarm(void);
void Init_INT(void)
{
	CfgPortPinFunc(0,1,EINT0_0_1);
	VICIntEnable = 1 << EINT0_VIC_CHN0;
  VICVectCntl0 = (1<<5) | EINT0_VIC_CHN0;   
  VICVectAddr0 = (u32)eint0_isr;
	EXTMODE = 1<<0;
}

void eint0_isr(void) __irq
{	
	u32 key = 0;
	loop1:
    CmdLCD(CLEAR_LCD);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD((s8 *)"/.Time  X.Date");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD((s8 *)"-.Alarm +.Exit");
		
    while(1)
    {
        key = KeyScan();
        if(key == 0) continue;

        switch(key)
        {
            case '/':
                CmdLCD(CLEAR_LCD);
                StrLCD((s8 *)"Setting Time...");
								delayMS(100);
                SetTime();
								goto loop1;

            case 'X':
                CmdLCD(CLEAR_LCD);
                StrLCD((s8 *)"Setting Date...");
								delayMS(100);
                SetDate();
								goto loop1;

            case '-':
                CmdLCD(CLEAR_LCD);
                StrLCD((s8 *)"Setting Alarm...");
								delayMS(100);
                SetAlarm();
								goto loop1;                

            case '+':
                CmdLCD(CLEAR_LCD);
                StrLCD((s8 *)"Exiting Menu...");
                delayS(1);
                goto loop;  // exit loop

            default:
                CmdLCD(CLEAR_LCD);
                CmdLCD(GOTO_LINE1_POS0);
                StrLCD((s8 *)"Invalid Key");
                delayS(1);
								goto loop1;
        }
			}
			loop:
			EXTINT=1<<0;
			VICVectAddr=0;
			CmdLCD(CLEAR_LCD);
				
}
		
	


