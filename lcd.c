//lcd.c
#include <LPC21xx.h>
#include "types.h"
#include "defines.h"
#include "lcd_defines.h"
#include "lcd.h"
#include "delay.h"

void WriteLCD(u8 byte)
{
	//select write operation
	IOCLR0=1<<LCD_RW;
	//write byte to lcd data pins
	WRITEBYTE(IOPIN0,LCD_DATA,byte);
	//provide high to low pulse
	IOSET0=1<<LCD_EN;
	delayUS(1);
	IOCLR0=1<<LCD_EN;
	delayMS(2);
}

void CmdLCD(u8 cmdByte)
{
	//set rs pin for cmd/inst reg
  IOCLR0=1<<LCD_RS;
  //write cmd byte to cmd reg
  WriteLCD(cmdByte); 	
}

void Init_LCD(void)
{
	//cfg p0.8(d0) to p0.15(d7),
	//p0.16(rs),p0.17(rw),p0.18(en) 
	//as gpio output pins
	IODIR0|=((0xFF<<LCD_DATA)|
	         (1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_EN));
	
	//power on delay
	delayMS(15);
	CmdLCD(0x30);
	delayMS(4);
	delayUS(100);
	CmdLCD(0x30);
	delayUS(100);
	CmdLCD(0x30);
	CmdLCD(MODE_8BIT_2LINE);
	CmdLCD(DSP_ON_CUR_ON);
	CmdLCD(CLEAR_LCD);
	CmdLCD(SHIFT_CUR_RIGHT);
	CmdLCD(0x0C);   // Display ON, Cursor OFF, Blink OFF

}

void CharLCD(u8 asciiVal)
{
	//sel data reg
	IOSET0=1<<LCD_RS;
	//write ascii value via data reg to ddram
	WriteLCD(asciiVal);
}

void StrLCD(s8 *str)
{
	while(*str)
		CharLCD(*str++);
}

void U32LCD(u32 n)
{
	s32 i=0;
	u8 a[10];
	
	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
	  while(n>0)
    {
		  a[i++]=(n%10)+48;
      n/=10;			
    }
    for(--i;i>=0;i--)
      CharLCD(a[i]);		
	}
}

void S32LCD(s32 num)
{
	if(num<0)
	{
		CharLCD('-');
		num=-num;
	}
	U32LCD(num);
}

void F32LCD(f32 fnum,u32 nDP)
{
	u32 num,i;
	if(fnum<0.0)
	{
		CharLCD('-');
		fnum=-fnum;
	}
	num=fnum;
	U32LCD(num);
	CharLCD('.');
	for(i=0;i<nDP;i++)
	{
		fnum=(fnum-num)*10;
		num=fnum;
		CharLCD(num+48);
	}
}

void BuildCGRAM(u8 *ptr, u8 nBytes)
{
    u32 i;

    // 1) Point to CGRAM start: 0x40
    CmdLCD(GOTO_CGRAM_START);

    // 2) Write bytes to CGRAM
    for(i = 0; i < nBytes; i++)
    {
        IOSET0 = 1 << LCD_RS;   // RS = 1 (data)
        WriteLCD(ptr[i]);
    }

    // 3) Return to DDRAM memory
    CmdLCD(0x80);               // Return to first DDRAM address
}


/*
void WriteCustomChar(u8 *pattern, u8 startIndex)
{
    u8 i, j;
    for (i = 0; i < 3; i++) // 3 characters
    {
        CmdLCD(0x40 | ((startIndex + i) << 3)); // Set CGRAM address
        for (j = 0; j < 8; j++)
        {
            CharLCD(pattern[i * 8 + j]);
        }
    }
}
*/

