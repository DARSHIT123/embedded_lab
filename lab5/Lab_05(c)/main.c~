
//------------------------------------------
// TivaWare Header Files
//------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "driverlib/uart.h"
#include "driverlib/uart.c"
#include <time.h>
#include "inc/hw_pwm.h"
#include "inc/hw_adc.h"
#include "utils/uartstdio.h"

#include "driverlib/adc.c"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/udma.h"
#include "driverlib/pwm.h"
#include "driverlib/ssi.h"
#include "driverlib/systick.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.c"

#include "one.h"
#include "two.h"
#include "three.h"
#include "four.h"
#include "five.h"
#include "six.h"
#include "seven.h"
#include "eight.h"

#define LOCK_F (*((volatile unsigned long *)0x40025520))
#define CR_F (*((volatile unsigned long *)0x40025524))

//----------------------------------------
// Prototypes
//----------------------------------------
void hardware_init(void);
void delay(void);
void inituart (void);

void hardware_init(void);
void delay(void);
void inituart (void);
void glcd_setcolumn(unsigned char column);
void glcd_setpage(unsigned char page);
void glcd_data(unsigned char data);
void glcd_cmd (unsigned char cmd);
void glcd_init (void);
void glcd_cleardisplay(void);

uint32_t ui32Period;

volatile uint32_t X,Y;

//---------------------------------------------------------------------------
// main()
//---------------------------------------------------------------------------
void main(void)
{
	//char num [32] = {0};
	uint32_t ADC_Raw_X[4],ADC_Raw_Y[4];

	hardware_init();	// init hardware via Xware
	//inituart();
	delay();
	delay();

		glcd_init();
		SysCtlDelay(134000);
		glcd_cleardisplay();
		delay();
		unsigned int i;
		unsigned int j = 0;
		unsigned char p = 0;
		unsigned del=1;
	while(1)									// forever loop
	{
			while(p<8)
			{
				glcd_setpage(p);

				for(i=0; i<128; i++)
				{
					glcd_setcolumn(i);
					glcd_data(one[j]);
					j++;
				}

				p++;
			}

			SysCtlDelay(del);
//			glcd_cleardisplay();
			p=0;
			j=0;

			while(p<8)
			{
				glcd_setpage(p);

				for(i=0; i<128; i++)
				{
					glcd_setcolumn(i);
					glcd_data(two[j]);
					j++;
				}

				p++;
			}

			SysCtlDelay(del);
	//		glcd_cleardisplay();
			p=0;
			j=0;

			while(p<8)
			{
				glcd_setpage(p);

				for(i=0; i<128; i++)
				{
					glcd_setcolumn(i);
					glcd_data(three[j]);
					j++;
				}

				p++;
			}

			SysCtlDelay(del);
		//	glcd_cleardisplay();
			p=0;
			j=0;

			while(p<8)
			{
				glcd_setpage(p);

				for(i=0; i<128; i++)
				{
					glcd_setcolumn(i);
					glcd_data(four[j]);
					j++;
				}

				p++;
			}

			SysCtlDelay(del);
//			glcd_cleardisplay();
			p=0;
			j=0;

			while(p<8)
			{
				glcd_setpage(p);

				for(i=0; i<128; i++)
				{
					glcd_setcolumn(i);
					glcd_data(five[j]);
					j++;
				}

				p++;
			}

			SysCtlDelay(del);
	//		glcd_cleardisplay();
			p=0;
			j=0;

			while(p<8)
			{
				glcd_setpage(p);

				for(i=0; i<128; i++)
				{
					glcd_setcolumn(i);
					glcd_data(six[j]);
					j++;
				}

				p++;
			}

			SysCtlDelay(del);
		//	glcd_cleardisplay();
			p=0;
			j=0;

			while(p<8)
			{
				glcd_setpage(p);

				for(i=0; i<128; i++)
				{
					glcd_setcolumn(i);
					glcd_data(seven[j]);
					j++;
				}

				p++;
			}

			SysCtlDelay(del);
//			glcd_cleardisplay();
			p=0;
			j=0;

			while(p<8)
			{
				glcd_setpage(p);

				for(i=0; i<128; i++)
				{
					glcd_setcolumn(i);
					glcd_data(eight[j]);
					j++;
				}

				p++;
			}

			SysCtlDelay(del);
//			glcd_cleardisplay();
			p=0;
			j=0;
		ADCProcessorTrigger(ADC0_BASE, 1);
		ADCProcessorTrigger(ADC1_BASE, 1);

//		while(!ADCIntStatus(ADC0_BASE, 1, false) && !ADCIntStatus(ADC1_BASE, 1, false));
//		{
//		}
//		ADCIntClear(ADC0_BASE, 1);
//		ADCIntClear(ADC1_BASE, 1);

		ADCSequenceDataGet(ADC0_BASE, 1, ADC_Raw_Y);
		ADCSequenceDataGet(ADC1_BASE, 1, ADC_Raw_X);

		X = ((ADC_Raw_X[0] + ADC_Raw_X[1] + ADC_Raw_X[2] + ADC_Raw_X[3])/4);
		Y = ((ADC_Raw_Y[0] + ADC_Raw_Y[1] + ADC_Raw_Y[2] + ADC_Raw_Y[3])/4);

		if(X > 3900)
		{
			del = 1340000;
		}
		else if(X < 200)
		{
			del = 1;
		}
		else
		{
			del = 670000;
		}
	}

}


//---------------------------------------------------------------------------
// hardware_init()
//
// inits GPIO pins for toggling the LED
//---------------------------------------------------------------------------
void hardware_init(void)
{

	//Set CPU Clock to 40MHz. 400MHz PLL/2 = 200 DIV 5 = 40MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

		//Removing Locks From Switch
					LOCK_F=0x4C4F434B;
					CR_F=GPIO_PIN_0|GPIO_PIN_4;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0);            // Configuring PD0 as ADC input (channel 1)     CH7 ADC0
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_1);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);

	ADCSequenceConfigure(ADC0_BASE,	1, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceConfigure(ADC1_BASE,	1, ADC_TRIGGER_PROCESSOR, 0);

	ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH7);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH7);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH7);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_CH7 | ADC_CTL_IE | ADC_CTL_END);

	ADCSequenceStepConfigure(ADC1_BASE, 1, 0, ADC_CTL_CH6);
	ADCSequenceStepConfigure(ADC1_BASE, 1, 1, ADC_CTL_CH6);
	ADCSequenceStepConfigure(ADC1_BASE, 1, 2, ADC_CTL_CH6);
	ADCSequenceStepConfigure(ADC1_BASE, 1, 3, ADC_CTL_CH6 | ADC_CTL_IE | ADC_CTL_END);

	ADCSequenceEnable(ADC0_BASE, 1);
	ADCSequenceEnable(ADC1_BASE, 1);
	ADCIntClear(ADC0_BASE, 1);
	ADCIntClear(ADC1_BASE, 1);

	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
		GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_6);
		GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3);
		GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5);
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
		//GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4,0x10);

}



//---------------------------------------------------------------------------
// delay()
//
// Creates a 500ms delay via TivaWare fxn
//---------------------------------------------------------------------------
void delay(void)
{
	SysCtlDelay(67000);		// creates ~50ms delay - TivaWare fxn

}

//-----------------------
// 3) inituart
//-----------------------

void inituart (void)
{
// Enable GPIO port A which is used for UART0 pins.

// change this to whichever GPIO port you are using.

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

// Enable UART0 so that we can configure the clock.

	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

// Configure the pin muxing for UART0 functions on port A0 and A1.

// This step is not necessary if your part does not support pin muxing.

// change this to select the port/pin you are using.

	GPIOPinConfigure(GPIO_PA0_U0RX);

	GPIOPinConfigure(GPIO_PA1_U0TX);

	// change this to select the port/pin you are using.

	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);



// Use the internal 16MHz oscillator as the UART clock source.

UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

// Select the alternate (UART) function for these pins.

// Initialize the UART for console I/O.

//UARTStdioConfig(0, 9600, 16000000);
//UARTStdioConfig(1, 9600, 16000000);

//UARTStdioConfig(0, 9600, 16000000);

}

//-----------------------
// initGLCD
//-----------------------

void glcd_init (void)
{
	SysCtlDelay(134000);
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x00);
	SysCtlDelay(134000);
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x20);
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0x00);
	glcd_cmd(0xC0);
	glcd_cmd(0xB8);
	glcd_cmd(0x40);
	glcd_cmd(0x3F);
	SysCtlDelay(134000);
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0x08);
	glcd_cmd(0xC0);
	glcd_cmd(0xB8);
	glcd_cmd(0x40);
	glcd_cmd(0x3F);
	SysCtlDelay(134000);
}

//---------------------------
// glcd_cmd
//--------------------------

void glcd_cmd (unsigned char cmd)
{
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 , 0x00);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 , 0x00);
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6 , 0x00);			//RS should be zero.
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 , cmd);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 , cmd);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 , 0x01);
	SysCtlDelay(1340);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 , 0x00);
}

//----------------------------
// glcd_data
//--------------------------

void glcd_data(unsigned char data)
{
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 , 0x00);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 , 0x00);
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6 , 0x40);
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 , data);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 , data);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 , 0x01);
	SysCtlDelay(100);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 , 0x00);
}

//----------------------------
// glcd_setpage
//--------------------------

void glcd_setpage(unsigned char page)
{
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3 , 0x00);
	glcd_cmd(0xB8 | page);
//	SysCtlDelay(100);
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3 , 0x08);
	glcd_cmd(0xB8 | page);
//	SysCtlDelay(100);
}

//----------------------------
// glcd_setcolumn
//--------------------------

void glcd_setcolumn(unsigned char column)
{
	if (column < 64)
	{
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3 , 0x00);
		glcd_cmd(0x40 | column);
//		SysCtlDelay(6700);
	}
	else
	{
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3 , 0x08);
		glcd_cmd(0x40 | (column - 64));
//		SysCtlDelay(6700);
	}
}


//----------------------------
// glcd_cleardisplay
//--------------------------

void glcd_cleardisplay(void)
{
	unsigned char i ;
	unsigned int j;
	for(i=0;i<8;i++)
	{
		glcd_setpage(i);
		for(j=0;j<128;j++)
		{
			glcd_setcolumn(j);
			glcd_data(0x00);
		}
	}
}
