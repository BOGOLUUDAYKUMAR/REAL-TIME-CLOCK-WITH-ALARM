Project Overview:
This project implements a Real-Time Clock (RTC) system using the LPC21xx ARM7 microcontroller. 
The system displays real-time Time and Date on a 16x2 LCD and includes an Alarm feature that activates a buzzer or LED when the current time matches a preset alarm time. 
The internal RTC peripheral of the microcontroller is used for accurate and continuous timekeeping.

Features:
Real-Time Clock display (HH:MM:SS)
Date display (DD/MM/YYYY)
Alarm functionality
Buzzer/LED activation on alarm match
Continuous RTC register monitoring
Hardware and Software Used

Hardware:
LPC21xx (ARM7) Microcontroller
16x2 LCD Display
Buzzer / LED
Crystal Oscillator (if external used)
Regulated Power Supply

Software:
Embedded C
Keil uVision
Flash Magic
Proteus (optional for simulation)

Working Principle:
The internal RTC registers (SEC, MIN, HOUR, DOM, MONTH, YEAR) are initialized and configured. 
The LCD continuously displays the current time and date. 
An alarm time is defined in the program, and when the RTC time matches the alarm time, the buzzer or LED is activated to indicate the alarm event.

Project Structure:
main.c – Main program control
rtc.c – RTC initialization and configuration
lcd.c – LCD interfacing functions
kpm.c - For keyboard input
int.c - Interupt raising
Header files – Register definitions and macros

How to Run:
Open the project in Keil uVision.
Build the project and generate the HEX file.
Flash the HEX file into LPC21xx using Flash Magic.
Power the hardware and observe time, date, and alarm operation on the LCD.

Learning Outcomes:
Understanding ARM7 architecture
Working with RTC peripheral registers
LCD interfacing in Embedded C
Real-time embedded system design
