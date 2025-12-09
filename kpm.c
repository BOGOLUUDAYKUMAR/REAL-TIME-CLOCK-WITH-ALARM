//kpm.c
#include "types.h"
#include <LPC21xx.h>
#include "defines.h"
#include "kpm_defines.h"
#include "lcd_defines.h"
#include "lcd.h"

u8 kpmLUT[4][4]=
{
	{'7','8','9','/'},
	{'4','5','6','X'},
	{'1','2','3','-'},
	{'C','0','=','+'}
};

void Init_KPM(void)
{
	//cfg row lines as gpio output lines &
	//ground all rows
	WRITENIBBLE(IODIR1,ROWs_4,15);
	//cfg col lines as gpio input(default anyway);
}

u32  ColScan(void)
{
	return (READNIBBLE(IOPIN1,COLs_4)<15)? 0 : 1; 
}

u32  RowCheck(void)
{
  u32 r;
  for(r=0;r<=3;r++)
  {
		//ground one row at a time,starting from 0th row
		WRITENIBBLE(IOPIN1,ROWs_4,~(1<<r));
		//check if key was pressed in that row
		if(ColScan()==0)
		{
			break;
		}			
  }		
	//re-initiaze all rows to ground
	WRITENIBBLE(IOPIN1,ROWs_4,0);
	return r;
}

u32  ColCheck(void)
{
	u32 c;
	for(c=0;c<=3;c++)
	{
		if(READBIT(IOPIN1,COL0+c)==0)
		{
			break;
		}
	}
	return c;
}

u32  KeyScan(void)
{
	u32 r,c,keyV;
	
	//wait to detect any key press
	while(ColScan());
	//if key press detected
	//identify row in which key was pressed
	r=RowCheck();
	//identify col in which key was pressed
	c=ColCheck();
	//map key value to pressed key
	keyV=kpmLUT[r][c];
	
	return keyV;
}

void ReadNum(u32 *num,u8 *key)
{
	*num=0;
	while(1)
	{
		*key=KeyScan();
		if(*key>='0' && *key<='9')
		{
			*num=(*num*10)+(*key-48);
			CmdLCD(GOTO_LINE2_POS0);
			U32LCD(*num);
			while(ColScan()==0);
    }
    else
    {
			CharLCD(' ');
			CharLCD(*key);
			while(ColScan()==0);
			break;		
    }    		
	}
}
