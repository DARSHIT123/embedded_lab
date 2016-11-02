#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
//------------------------------------------
// TivaWare Header Files
//------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_timer.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_uart.h"
#include "inc/hw_gpio.h"
#include "inc/hw_pwm.h"
#include "inc/hw_adc.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/uart.c"
#include "utils/uartstdio.h"
#include "driverlib/adc.h"
#include "driverlib/adc.c"
#include "driverlib/fpu.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/udma.h"
#include "driverlib/pwm.h"
#include "driverlib/ssi.h"
#include "driverlib/systick.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.c"

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Hello World (hello)</h1>
//!
//! A very simple ``hello world'' example.  It simply displays ``Hello World!''
//! on the UART and is a starting point for more complicated applications.
//!
//! UART0, connected to the Virtual Serial Port and running at
//! 115,200, 8-N-1, is used to display messages from this application.
//
//*****************************************************************************

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif



#ifdef DEBUG
void__error__(char *pcFilename, unsigned long ulLine){
}
#endif

//----------------------
// Variable declaration
//----------------------

float temp ;

uint32_t count=110;

uint32_t ulADC0Value[3];

//----------------------
// Function declaration
//----------------------

void impact(int temp);

void inituart (void);

void adc_init(void);

void hardware_init(void);

//--------After LCD------------

void delay(void);
void LCDInit(void);
void LCD_ClearScreen(void);
void LCD_Go2Line2(void);
void LCD_Go2Line1(void);
void LCD_WriteCommand(unsigned char cmd);
void LCD_WriteChar(unsigned char data);
void enable(void);
void LCD_WriteString(char * string);

uint32_t ui32Period;

//---------------------------------------------------
// Function Definitions
//---------------------------------------------------

//-----------------------
// 1) hardware_init
//-----------------------

void hardware_init(void)

{

SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

//GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);

}


//-----------------------
// 2) adc_init
//-----------------------

void adc_init(void)

{

SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

ADCHardwareOversampleConfigure(ADC0_BASE, 8);// Hardware averaging. ( 2, 4, 8 , 16, 32, 64 )

//64 Samples are averaged here i.e, each sample will be a result of 64 averaged samples. Therefore,every result is a result of 64 x 4 = 256 samples.

ADCSequenceDisable(ADC0_BASE, 0); //Before Configuring ADC Sequencer 1, it should be OFF

ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);

//ADC Configured so that Processor Triggers the sequence and we want to use highest priority. ADC Sequencer 0 is Used.

ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0); //Sequencer Step 0: Samples Channel PE3

ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH1); //Sequencer Step 1: Samples Channel PE2

ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH2 | ADC_CTL_IE | ADC_CTL_END);

//Final Sequencer Step also Samples and enables Interrupt and we are telling the sequencer that this is the last step

//Configuring all eight steps in the ADC Sequence

//Sequencer Step 2: Samples Channel PE1

ADCSequenceEnable(ADC0_BASE, 0); //Enable ADC Sequence

ADCIntClear(ADC0_BASE, 0);

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

UARTStdioConfig(0, 57600, 16000000);

}

//-----------------------
// 4) main
//-----------------------

void main ()

{
	char num [16] = {0},i=0;
	uint32_t ulADC0Value[3];

	float res;

	int p,q,r;
	char s [16] = {0};

	//char gsm;

	inituart();

	hardware_init();

	adc_init();


	delay();
	delay();
	LCDInit();

	p=0;q=0;r=0;

	//LCD_WriteString("Ch01 :");
	LCD_Go2Line2();
	LCD_WriteString("res :");

	//UARTCharPutNonBlocking(UART0_BASE, 'A');
	//UARTCharPutNonBlocking(UART0_BASE, '"');
	UARTprintf("ATD 9769380305;\n");
	//UARTprintf("ATD+CMGF=1\n");
	delay();
	delay();
		delay();
		delay();
			delay();
	//UARTprintf("ATD+CMGS=");
	delay();
		delay();
	//	delay();
	//		delay();
	//UARTCharPutNonBlocking(UART0_BASE, '"');
	//delay();
	//	delay();delay();
	//	delay();
	//UARTprintf("8297109010");
	//delay();
	//	delay();
	//delay();
	//delay();
	//delay();
	//UARTCharPutNonBlocking(UART0_BASE, '"');
	//delay();
	//delay();
	//delay();
	//delay();
	//delay();
	//UARTprintf("\n");
	//delay();
	//delay();
	//delay();
	//delay();
    //delay();
	//UARTprintf("Device Started Working Properly...");
	//delay();
	//delay();
	//delay();
	//delay();
	//	delay();
	////UARTCharPutNonBlocking(UART0_BASE, "0x1A");
	//UARTprintf("0x1A");

	while(1)									// forever loop
		{

			//UARTprintf("AT\n");
			//UARTscanf();
			//UARTCharPut(UART1_BASE, 'A');
			//UARTCharPut(UART1_BASE, 'T');
			//UARTCharPut(UART1_BASE, '\n');



			//SysCtlDelay(300000*2);
			//SysCtlDelay(300000);

			//gsm = UARTCharGetNonBlocking(UART1_BASE);

			ADCProcessorTrigger(ADC0_BASE, 0);

			while(!ADCIntStatus(ADC0_BASE, 0, false));


			ADCIntClear(ADC0_BASE, 0);
			ADCSequenceDataGet(ADC0_BASE, 0, ulADC0Value);

			p = ulADC0Value[2] ;

			q = ulADC0Value[1] ;

			r = ulADC0Value[0] ;

			LCD_WriteCommand(0x80);
				    //LCD_WriteString("     ");

					for(i=0;i<16;i++)
						num [i] = 0;
					ltoa(p,num);
					LCD_WriteString(num);
					LCD_WriteString("     ");
					UARTprintf(num);
					UARTprintf(";");

					LCD_WriteCommand(0x86);
					for(i=0;i<16;i++)
						num [i] = 0;
					ltoa(q,num);
					LCD_WriteString(num);
					LCD_WriteString("     ");
					UARTprintf(num);
					UARTprintf(";");

					LCD_WriteCommand(0x8C);
					for(i=0;i<16;i++)
					num [i] = 0;
					ltoa(r,num);
					LCD_WriteString(num);
					LCD_WriteString("     ");
					UARTprintf(num);
					UARTprintf(";");

					res = sqrt(ulADC0Value[0] * ulADC0Value[0] + ulADC0Value[1] * ulADC0Value[1] + ulADC0Value[2] * ulADC0Value[2]);

					LCD_WriteCommand(0xCB);
					for(i=0;i<16;i++)
					num [i] = 0;
					ltoa(res,num);
					LCD_WriteString(num);
					LCD_WriteString("     ");
					UARTprintf(num);
					UARTprintf(";\n");


					if (1700< p && p < 2200)
					if (1700< q && q < 2200)
					if (1550< r && r < 2200)

					{

					UARTprintf("Jump_Detect;\n");
					while (count > 0)

					{

					ltoa(count,s);
					UARTprintf(s);
					UARTprintf("\n");
					count--;

					//SysCtlDelay(300000*2);

					//ADCProcessorTrigger(ADC0_BASE, 0);
                    //
					//while(!ADCIntStatus(ADC0_BASE, 0, false));
                    //
                    //
					//ADCIntClear(ADC0_BASE, 0);
					//ADCSequenceDataGet(ADC0_BASE, 0, ulADC0Value);
                    //
					//p = ulADC0Value[2] ;
                    //
					//q = ulADC0Value[1] ;
                    //
					//r = ulADC0Value[0] ;
                    //
					//LCD_WriteCommand(0x80);
				    ////LCD_WriteString("     ");
                    //
					//for(i=0;i<16;i++)
					//	num [i] = 0;
					//ltoa(p,num);
					//LCD_WriteString(num);
					//LCD_WriteString("     ");
					//UARTprintf(num);
					//					UARTprintf(";");
                    //
					//LCD_WriteCommand(0x86);
					//for(i=0;i<16;i++)
					//	num [i] = 0;
					//ltoa(q,num);
					//LCD_WriteString(num);
					//LCD_WriteString("     ");
					//UARTprintf(num);
					//					UARTprintf(";");
                    //
					//LCD_WriteCommand(0x8C);
					//for(i=0;i<16;i++)
					//num [i] = 0;
					//ltoa(r,num);
					//LCD_WriteString(num);
					//LCD_WriteString("     ");
					//UARTprintf(num);
					//					UARTprintf(";");
                    //
					//res = sqrt(ulADC0Value[0] * ulADC0Value[0] + ulADC0Value[1] * ulADC0Value[1] + ulADC0Value[2] * ulADC0Value[2]);
                    //
					//LCD_WriteCommand(0xCB);
					//LCD_WriteString("frfall");
                    //
					//for(i=0;i<16;i++)
					//					num [i] = 0;
					//					ltoa(res,num);
                    //
					//					UARTprintf(num);
					//					UARTprintf(";\n");
                    //
					////LCD_WriteCommand(0xCB);
					////for(i=0;i<16;i++)
					////num [i] = 0;
					////ltoa(res,num);
					////LCD_WriteString(num);
					////LCD_WriteString("     ");



					if (res > 3200)

					{
						temp = res;

						impact(temp);
					}

					}

					}

					SysCtlDelay(300000*2);

					//SysCtlDelay(300000*2);

					//SysCtlDelay(300000*2);





		}



}

//-----------------------
// 5) impact
//-----------------------

void impact (int temp)

	{

	int i,x1,y1,z1,t1,t2,t3;

	LCD_WriteCommand(0xCB);
	LCD_WriteString("Impact");
	UARTprintf("Impact\n");



	if (temp > 3200)

	{

	// Delay for 2 sec

	for (i=0;i<200;i++)

	SysCtlDelay(268000);

	ADCIntClear(ADC0_BASE, 0); // Clear ADC Interrupt

	ADCProcessorTrigger(ADC0_BASE, 0); // Trigger ADC Interrupt

	while(!ADCIntStatus(ADC0_BASE, 0, false)) ;

	ADCSequenceDataGet(ADC0_BASE, 0, ulADC0Value); //Grab the Entire FIFO

	//Wait for interrupt Status flag to go off

	x1 = ulADC0Value[2];

	y1 = ulADC0Value[1];

	z1 = ulADC0Value[0];

	// Checking for change in orientation

	t1 = abs(2700-x1);

	t2 = abs(1800-y1);

	t3 = abs(1800-z1);

	if (((t1 > 600) && (t2 > 600)) || ((t1 > 600) && (t3 > 600)) || ((t2 > 600) && (t3 > 600)))

	{

	// Generate fall detection alert

	//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,0xFF);

	// Delay for 10 sec

	for (i=0;i<500;i++)

	SysCtlDelay(268000);

	ADCIntClear(ADC0_BASE, 0); // Clear ADC Interrupt

	ADCProcessorTrigger(ADC0_BASE, 0); // Trigger ADC Interrupt

	while(!ADCIntStatus(ADC0_BASE, 0, false)) ;

	ADCSequenceDataGet(ADC0_BASE, 0, ulADC0Value); //Grab the Entire FIFO

	//Wait for interrupt Status flag to go off

	x1 = ulADC0Value[2];

	y1 = ulADC0Value[1];

	z1 = ulADC0Value[0];

	t1 = abs(2700-x1);

	t2 = abs(1800-y1);

	t3 = abs(1800-z1);

	if (((t1 > 600) && (t2 > 600)) || ((t1 > 600) && (t3 > 600)) || ((t2 > 600) && (t3 > 600)))

	{

	// Generate Critical Fall alert

	//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,0xFF);
		LCD_WriteCommand(0xCB);
			LCD_WriteString("Critical");
			UARTprintf("Critical\n");
			UARTprintf("ATD 9769380305;\n");

	SysCtlDelay(268000*8);

	}

	}

	}

	//main();

	}

//-----------------------
// 5) For LCD
//-----------------------

void delay(void)
{
	SysCtlDelay(67000);		// creates ~50ms delay - TivaWare fxn

}


void LCDInit(void)
{
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);//(c) Set R/W = 1/0 for read and write operation.
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);//(g) Make En = 0.

	delay();
	LCD_WriteCommand(0x38); // function set command/ 8-bit interface, 2 display lines, 5x7 font
	delay();
	LCD_WriteCommand(0x06); // input set command // entry mode set: increment automatically, no display shift
	delay();
	LCD_WriteCommand(0x0C); // display switch command / turn display on, cursor on, no blinking
	delay();
	LCD_WriteCommand(0x01); // clear screen command // clear display, set cursor position to zero
	delay();
	LCD_WriteCommand(0x80); // clear screen command // clear display, set cursor position to zero
	delay();
}


void LCD_WriteCommand(unsigned char cmd)
{
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0);//(b) Set RS = 0 for command write to LCD.
	delay();
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, cmd);//(d) Put your data on data lines.
	enable();
}

void LCD_WriteChar(unsigned char data)
{
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0xFF);//(b) Set RS = 1 for data write to LCD.
	//delay();
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, data);//(d) Put your data on data lines.
	enable();
}


void enable(void)
{
	delay();
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0xFF);//(a) Set En = 1.
	delay();
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);//(g) Make En = 0.
	delay();
}



void LCD_ClearScreen(void)
{
	LCD_WriteCommand(0x01); // clear screen command // clear display, set cursor position to zero
	delay();
	LCD_WriteCommand(0x80); // clear screen command // clear display, set cursor position to zero
}

void LCD_Go2Line1(void)
{
	LCD_WriteCommand(0x02); // clear screen command // clear display, set cursor position to zero
}


void LCD_Go2Line2(void)
{
	LCD_WriteCommand(0x80 + 0x40); // Set cursor to second line starting
}


void LCD_WriteString(char * string)
{
	int c=0;
	while (string[c]!='\0')
	{
		LCD_WriteChar(string[c]);
		c++;
	}
}

