#include "header.h"

// Global variables
//s32 hour, min, sec, date, month, year, day;
extern s32 hour, min, sec, date, month, year, day;
extern s32 alarm_hr, alarm_min;
extern u8 alarm_active,alarm_ring;

u8 asy[8] = 
	{
    0x04, 0x0E, 0x0E, 0x0E, 0x0E, 0x1F, 0x1F, 0x04
	};

// Function prototypes
void SetTime(void);
void SetDate(void);
void SetAlarm(void);

// Main function
int main(void)
{
    RTC_Init();
    Init_LCD();
    Init_ADC();
    Init_INT();
    Init_KPM();

    BuildCGRAM(asy, 8);   // << added

    SetRTCTimeInfo(00, 00, 00);
    SetRTCDateInfo(00, 00, 0000);
    SetRTCDay(6);

    while(1)
    {
        GetRTCTimeInfo(&hour, &min, &sec);
        DisplayRTCTime(hour, min, sec);

        GetRTCDateInfo(&date, &month, &year);
        DisplayRTCDate(date, month, year);

        GetRTCDay(&day);
        DisplayRTCDay(day);
				DispTSymbol();
        DispTemp();

        CheckAndTriggerAlarm();

        // Show static bell when alarm set
        if(alarm_active && !alarm_ring)
        {
            CmdLCD(GOTO_LINE2_POS0 + 11);
            CharLCD(0);
        }
				IODIR0 &= ~(1<<STOP_SW);   // make P0.4 INPUT
				
        // stop switch
        if((IOPIN0 & (1<<STOP_SW)) == 0)
        {
            IOCLR0 = (1<<BUZZER_SW);
            alarm_ring = 0;
            alarm_active = 0;

            CmdLCD(GOTO_LINE2_POS0+11);
            StrLCD(" ");
        }

        delayMS(300);
    }
}
