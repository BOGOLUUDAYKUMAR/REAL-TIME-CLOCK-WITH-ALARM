#include "header.h"

// Array to hold names of days of the week
s8 week[][4] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};
u8 up[24]=
	{
		0x00,0X01,0X03,0X07,0X0e,0X1e,0X1f,0X1f,
		0x00,0X1F,0X1f,0X1f,0X0E,0X0E,0X1f,0X1f,
		0x00,0X10,0X18,0X1C,0X0E,0X0f,0X1f,0X1f
		
	};
u8 down[24]=
	{
		0X1F,0X1E,0X1F,0X0F,0X07,0X03,0X01,0x00,
		0X1F,0X00,0X00,0X1F,0X1F,0X1f,0X1F,0x00,
		0x1f,0x0f,0x1f,0x1e,0x1c,0x18,0x10,0x00
	};	
	
u8 bell[8] = {0x04,0x0E,0x0E,0x0E,0x0E,0x1F,0x1F,0x04};
f32 eAR;
u32 temp;
u8 max_day;
u32 adcDVal;
u8 leap = 0;
u8 alarm_active = 0;
u8 alarm_ring   = 0;
u8 blink_state  = 0;	
s32 curDay, curMonth, curYear;
s32 alarm_hr = 0, alarm_min = 0;
s32 hour = 0, min = 0, sec = 0, date = 0, month = 0, year = 0, day = 0;



void SetTime(void);
void SetDate(void);
void WriteCustomChar(u8 *pattern, u8 startIndex);

/*
***********************************************************************************
*                     RTC_INITIALIZATION                                          *
***********************************************************************************
*/
void RTC_Init(void) 
{
  // Disable and reset the RTC
	CCR = RTC_RESET;
  
  // Set prescaler integer and fractional parts
	PREINT = PREINT_VAL;
	PREFRAC = PREFRAC_VAL;
  
  // Enable the RTC
	CCR = RTC_ENABLE;  
}

/*
***********************************************************************************
*                     RTC_TIME_DISPLY                                             *
***********************************************************************************
*/
void GetRTCTimeInfo(s32 *hour, s32 *minute, s32 *second)
{
	*hour = HOUR;
	*minute = MIN;
	*second = SEC;
}

void DisplayRTCTime(s32 hour, s32 minute, s32 second)
{
		CmdLCD(GOTO_LINE1_POS0);
		CharLCD((hour/10)+48);
		CharLCD((hour%10)+48);
		CharLCD(':');
		CharLCD((minute/10)+0x30);
		CharLCD((minute%10)+0x30);
		CharLCD(':');
		CharLCD((second/10)+'0');
		CharLCD((second%10)+'0');

}
/*
***********************************************************************************
*                     RTC_DATE_DISPLAY                                            *
***********************************************************************************
*/
void GetRTCDateInfo(s32 *date, s32 *month, s32 *year)
{
	*date = DOM;
	*month = MONTH;
	*year = YEAR;
}

void DisplayRTCDate(u32 date, u32 month, u32 year)
{
    CmdLCD(GOTO_LINE2_POS0);
    CharLCD((date / 10) + '0');
    CharLCD((date % 10) + '0');
    CharLCD('/');
    CharLCD((month / 10) + '0');
    CharLCD((month % 10) + '0');
    CharLCD('/');
    CharLCD(((year / 1000) % 10) + '0');
    CharLCD(((year / 100) % 10) + '0');
    CharLCD(((year / 10) % 10) + '0');
    CharLCD((year % 10) + '0');
}

/*
***********************************************************************************
*                     RTC_DAY_DISPLAY                                             *
***********************************************************************************
*/
void GetRTCDay(s32 *dayofweek)
{
	*dayofweek = DOW; 
}

void DisplayRTCDay(u32 dayofweek)
{
	CmdLCD(GOTO_LINE2_POS0+13);
	StrLCD(week[dayofweek]);  
}

/*
***********************************************************************************
*                     SET_TIME_DATE_DAY                                           *
***********************************************************************************
*/
void SetRTCTimeInfo(u32 hour, u32 minute, u32 second)
{
	HOUR = hour;
	MIN = minute;
	SEC = second;

}

void SetRTCDateInfo(u32 date, u32 month, u32 year)
{
	DOM = date;
	MONTH = month;
	YEAR = year;

}

void SetRTCDay(u32 dayofweek)
{
		DOW = dayofweek;
}
/*
***********************************************************************************
*                     DISPLAY_TEMPERATURE                                         *
***********************************************************************************
*/
void DispTSymbol(void)
{
    CmdLCD(GOTO_LINE1_POS0 + 10);// Move to line 1, column 11 (GOTO_LINE1_POS0 + 10)
    CharLCD('T');
    CharLCD(':');
		//CharLCD((u8)(0x1F514));
		CmdLCD(GOTO_LINE1_POS0 + 14);
		CharLCD(0xdf);//ASCII 223
		CharLCD('C');
}

void DispTemp(void)
{
        Read_ADC(1,&adcDVal,&eAR);
        temp = eAR*100;
        CmdLCD(GOTO_LINE1_POS0+12);
        U32LCD(temp);
}
void DispASymbol(u8 *a, u32 b)
{
   if(alarm_active)
   {
			u8 a[8]={0x04,0x0e,0x0e,0x0e,0x0e,0x1f,0x1f,0x04};
			BuildCGRAM(a, 8);            
			CmdLCD(GOTO_LINE2_POS0 + 11); 
      CharLCD(0);
   }                        
}
/*
***********************************************************************************
*                          EDIT_TIME                                              *
***********************************************************************************
*/
void SetTime(void)
{
    u8 hr = 0, min = 0, sec = 0;
    char k, choice;
    int key = 0;        
    u8 dig = 0;         

		TimeMenu:
    CmdLCD(CLEAR_LCD);
    StrLCD((s8 *)"1.Hour 2.Minutes");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD((s8 *)"3.Sec  4.Exit");

    /* get menu choice */
    do { choice = KeyScan(); } while (choice == 0);
    while (KeyScan() != 0);

    switch (choice)
    {
        case '1': CmdLCD(CLEAR_LCD); StrLCD((s8 *)"Edit Hour:"); break;
        case '2': CmdLCD(CLEAR_LCD); StrLCD((s8 *)"Edit Minute:"); break;
        case '3': CmdLCD(CLEAR_LCD); StrLCD((s8 *)"Edit Second:"); break;
        case '4': CmdLCD(CLEAR_LCD); StrLCD((s8 *)"Exit Time Menu"); delayMS(500); return;
        default : goto TimeMenu;
    }

    /* show input field */
    CmdLCD(GOTO_LINE1_POS0+12);
    StrLCD((s8 *)"__");
    CmdLCD(GOTO_LINE1_POS0+12);
    /* reset builder */
    key = 0;
    dig = 0;

    while (1)
    {
        k = KeyScan();
        if (k == 0) continue;
        /* ----- Digit entry ----- */
        if (k >= '0' && k <= '9')
				{
					if (dig < 2)
					{
						key = key * 10 + (k - '0');
						CharLCD(k);
						dig++;

						// ------- After 2 digits entered -------
						if (dig == 2)
						{
							/* Show confirmation message */
							CmdLCD(GOTO_LINE1_POS0);
							switch (choice)
							{
                case '1': StrLCD((s8 *)"Edit Hour:   "); break;
                case '2': StrLCD((s8 *)"Edit Minute: "); break;
                case '3': StrLCD((s8 *)"Edit Second: "); break;
							}
							// 	Print the value on Line-1
							CmdLCD(GOTO_LINE1_POS0+12);
							CharLCD((key / 10) + '0');
							CharLCD((key % 10) + '0');

							// Line-2 instruction
							CmdLCD(GOTO_LINE2_POS0);
							StrLCD((s8 *)"Press'='to Save");
						}
					}
					else
					{
						CmdLCD(CLEAR_LCD);
						StrLCD((s8 *)"Too many digits!");
						delayMS(500);
						goto TimeMenu;
					}

					while (KeyScan() != 0);
					continue;
				}

				 /* --- Backspace --- */
        else if (k == 'C')
        {
            if (dig > 0)
            {
                dig--;
                key = key / 10;  
                CmdLCD(GOTO_LINE2_POS0 + dig);
                CharLCD('_');
                CmdLCD(GOTO_LINE2_POS0 + dig);
            }
            while (KeyScan() != 0);
            continue;
        }
        else if (k == '=')
        {
            /* ensure user entered at least one digit */
            if (dig == 0)
            {
                CmdLCD(CLEAR_LCD);StrLCD((s8 *)"No Input!");delayMS(500);goto TimeMenu;
            }
						/*Test Casess*/
            switch (choice)
            {
                case '1': /* hour */
                    if (key > 23)
                    {
                        CmdLCD(CLEAR_LCD);StrLCD((s8 *)"Invalid Hour!");delayMS(500);goto TimeMenu;
                    }
                    hr = (u8)key;
                    break;
                case '2': /* minute */
                    if (key > 59)
                    {
                        CmdLCD(CLEAR_LCD);StrLCD((s8 *)"Invalid Minute!");delayMS(500);goto TimeMenu;
                    }
                    min = (u8)key;
                    break;
                case '3': /* second */
                    if (key > 59)
                    {
                        CmdLCD(CLEAR_LCD);StrLCD((s8 *)"Invalid Second!");delayMS(500);goto TimeMenu;
                    }
                    sec = (u8)key;
                    break;
            }
            /* write to RTC */
            SetRTCTimeInfo(hr, min, sec);
						
            /* confirmation */
            CmdLCD(CLEAR_LCD);
            switch (choice)
            {
                case '1': StrLCD((s8 *)"Hour Set OK"); break;
                case '2': StrLCD((s8 *)"Minute Set OK"); break;
                case '3': StrLCD((s8 *)"Second Set OK"); break;
            }
            delayMS(500);
						CmdLCD(CLEAR_LCD);
            StrLCD((s8*)"Updating...");
            delayMS(700);

            key = 0;
            dig = 0;
            goto TimeMenu;
        }
        /* ----- any other key is invalid ----- */
        else
        {
            CmdLCD(CLEAR_LCD);
            StrLCD((s8 *)"Invalid Key!");
            delayMS(500);
            goto TimeMenu;
        }
    }
}
/*
***********************************************************************************
*                          EDIT_DATE                                              *
***********************************************************************************
*/
void SetDate(void)
{
    u8 day = 0, month = 0, dow = 0, dig = 0;
    u16 year = 0;
    char k, choice;
    int key = 0;
    s32 curDay, curMonth, curYear;
    const s8 *dayStr[7] = { "SUN","MON","TUE","WED","THU","FRI","SAT" };

DateMenu:
    CmdLCD(CLEAR_LCD);
    StrLCD((s8*)"1.DD 2.MM 3.YYYY");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD((s8*)"4.DAY 5.EXIT");

    do { choice = KeyScan(); } while(choice==0);
    while(KeyScan()!=0);

    switch(choice)
    {
        case '1': CmdLCD(CLEAR_LCD); StrLCD((s8*)"Enter Date");  break;
        case '2': CmdLCD(CLEAR_LCD); StrLCD((s8*)"Enter Month"); break;
        case '3': CmdLCD(CLEAR_LCD); StrLCD((s8*)"Enter Year");  break;
        case '4': CmdLCD(CLEAR_LCD); StrLCD((s8*)"Enter Day(0-6):"); break;
        case '5': CmdLCD(CLEAR_LCD); StrLCD((s8*)"Exit..."); delayMS(500); return;
        default: goto DateMenu;
    }

    CmdLCD(GOTO_LINE1_POS0+12);
    if(choice=='3')      {StrLCD((s8*)"____");CmdLCD(GOTO_LINE1_POS0+12);}
    else if(choice=='4') {CmdLCD(GOTO_LINE1_POS0+15); StrLCD((s8*)"_");
													CmdLCD(GOTO_LINE2_POS0+0);StrLCD((s8*)"0S1M2T3W4T5F6S");
													CmdLCD(GOTO_LINE1_POS0+15);}
    else                 {StrLCD((s8*)"__");CmdLCD(GOTO_LINE1_POS0+12);}

  //  CmdLCD(GOTO_LINE1_POS0+12);
    key = 0;
    dig = 0;

    while(1)
    {
        k = KeyScan();
        if(k==0) continue;

        //---------------- NUMERIC -----------------
        if(k>='0' && k<='9')
        {
            if((choice=='3' && dig<4) || (choice!='3' && dig<2))
            {
                key = key*10 + (k-'0');
                CharLCD(k);
                dig++;

                if((choice=='1' || choice=='2') && (dig==2))
                {
                    CmdLCD(GOTO_LINE2_POS0);StrLCD((s8*)"Press '=' to Save");
                }
                else if(choice=='3' && dig==4)
                {
                    CmdLCD(GOTO_LINE2_POS0);StrLCD((s8*)"Press '=' to Save");
                }
                else if(choice=='4' && dig==1)
                {
										//CmdLCD(CLEAR_LCD);
                    CmdLCD(GOTO_LINE2_POS0);StrLCD((s8*)"Press '=' to Save");
                }
            }
            else
            {
                CmdLCD(CLEAR_LCD);
                StrLCD((s8*)"Too Many Inputs!");
                delayMS(700);
                goto DateMenu;
            }

            while(KeyScan()!=0);
            continue;
        }

        //---------------- CLEAR DIGIT -------------
        if(k=='C')
        {
            if(dig>0)
            {
                dig--;
                key = key/10;
                CmdLCD(GOTO_LINE1_POS0+12+dig);
                CharLCD('_');
                CmdLCD(GOTO_LINE1_POS0+12+dig);
            }
            while(KeyScan()!=0);
            continue;
        }

        //---------------- CONFIRM '=' -------------
        if(k=='=')
        {   
            // NO INPUT CHECK
            if(choice=='1' || choice=='2' || choice=='3')
            {
                if(dig==0 || key==0)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD((s8*)"No Input!");
                    delayMS(500);
                    goto DateMenu;
                }
            }
            else if(choice=='4')
            {
                if(dig==0)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD((s8*)"No Input!");
                    delayMS(500);
                    goto DateMenu;
                }
            }

            // GET CURRENT DATE
            GetRTCDateInfo(&curDay,&curMonth,&curYear);
            day   = (u8)curDay;
            month = (u8)curMonth;
            year  = (u16)curYear;

            // VALIDATION
            if(choice=='1')
            {
                if(dig != 2 || key < 1 || key > 31)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD((s8*)"Invalid Date!");
                    delayMS(500);
                    goto DateMenu;
                }
                day = key;
            }
            else if(choice=='2')
            {
                if(dig != 2 || key < 1 || key > 12)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD((s8*)"Invalid Month!");
                    delayMS(500);
                    goto DateMenu;
                }
                month = key;
            }
            else if(choice=='3')
            {
                if(dig != 4 || key < 1900 || key > 2099)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD((s8*)"Invalid Year!");
                    delayMS(500);
                    goto DateMenu;
                }
                year = key;
            }
            else if(choice=='4')
            {
                if(key<0 || key>6)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD((s8*)"Invalid Day!");
                    delayMS(500);
                    goto DateMenu;
                }
                dow = key;
                SetRTCDay(dow);
                CmdLCD(CLEAR_LCD);
                StrLCD((s8*)"Day=");
                StrLCD((s8*)dayStr[dow]);
                delayMS(700);
                goto DateMenu;
            }

            SetRTCDateInfo(day,month,year);
            CmdLCD(CLEAR_LCD);
            StrLCD((s8*)"Updating...");
            delayMS(700);
						
            goto DateMenu;
        }

        //---------------- INVALID KEY -------------
        if(k!='C' && k!='=' && (k<'0' || k>'9'))
        {
            CmdLCD(CLEAR_LCD);
            StrLCD((s8*)"Invalid Key!");
            delayMS(700);
            goto DateMenu;
        }
    }
}

/*
***********************************************************************************
*                     CHECK_ALARM_ON                                              *
***********************************************************************************
*/
//#define BUZZER_SW 21  // P0.21

void CheckAndTriggerAlarm(void)
{
    IO0DIR |= (1 << BUZZER_SW);

    // TRIGGER ALARM once when time matches
    if(alarm_active && hour == alarm_hr && min == alarm_min && !alarm_ring)
    {
        alarm_ring = 1;
        IOSET0 = (1 << BUZZER_SW);   // buzzer ON
    }

    // Blinking bell when ringing
    if(alarm_ring)
    {
        if(blink_state)
        {
            CmdLCD(GOTO_LINE2_POS0+11);
            StrLCD(" ");
            blink_state = 0;
        }
        else
        {
            CmdLCD(GOTO_LINE2_POS0+11);
            CharLCD(0);
            blink_state = 1;
        }
    }
}
		
/*
***********************************************************************************
*                         EDIT_ALARM                                              *
***********************************************************************************
*/
void SetAlarm(void)
{
    char k;
    u8 dig = 0;
    u16 key = 0;
    u8 hr = 0, mn = 0;

    CmdLCD(CLEAR_LCD);
    StrLCD((s8 *)"Set Alarm");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD((s8 *)"__:__");
    CmdLCD(GOTO_LINE2_POS0);

    while (1)
    {
        k = KeyScan();
        if (k == 0) continue;

        if (k >= '0' && k <= '9')
        {
            if (dig < 4)
            {
                if (dig < 2)
                    CmdLCD(GOTO_LINE2_POS0 + dig);
                else
                    CmdLCD(GOTO_LINE2_POS0 + dig + 1);

                CharLCD(k);
                key = key * 10 + (k - '0');
                dig++;
            }
            while (KeyScan() != 0);
            continue;
        }
        else if (k == 'C')
        {
            if (dig > 0)
            {
                dig--;
                key = key / 10;
                if (dig < 2)
                    CmdLCD(GOTO_LINE2_POS0 + dig);
                else
                    CmdLCD(GOTO_LINE2_POS0 + dig + 1);
                CharLCD('_');
            }
            while (KeyScan() != 0);
            continue;
        }
        else if (k == '=')
        {
            if (dig < 3)
            {
                CmdLCD(CLEAR_LCD);
                StrLCD((s8 *)"Incomplete!");
                delayMS(500);
                return;
            }

            hr = key / 100;
            mn = key % 100;

            if (hr > 23 || mn > 59)
            {
                CmdLCD(CLEAR_LCD);
                StrLCD((s8 *)"Invalid Time!");
                delayMS(500);
                return;
            }

            alarm_hr = hr;
            alarm_min = mn;
            alarm_active = 1;
            alarm_ring = 0;

            CmdLCD(GOTO_LINE2_POS0+11);
            CharLCD(0);

            CmdLCD(CLEAR_LCD);
            StrLCD((s8 *)"Alarm Set!");
            delayMS(500);
            return;
        }
        else if (k == '+')
        {
            CmdLCD(CLEAR_LCD);
            StrLCD((s8 *)"Cancelled");
            delayMS(500);
            return;
        }
        else
        {
            CmdLCD(CLEAR_LCD);
            StrLCD((s8 *)"Invalid!");
            delayMS(500);
            return;
        }
    }
}
