
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

//#define LOCK_F (*((volatile unsigned long *)0x40025520))
//#define CR_F (*((volatile unsigned long *)0x40025524))

//----------------------------------------
// Prototypes
//----------------------------------------
void hardware_init(void);
void delay(void);
void inituart (void);

uint32_t ui32Period;

volatile uint32_t X,Y;

//---------------------------------------------------------------------------
// main()
//---------------------------------------------------------------------------
void main(void)
{
	char num [32] = {0};
	uint32_t ADC_Raw_X[4],ADC_Raw_Y[4];

	hardware_init();	// init hardware via Xware
	inituart();
	delay();
	delay();
	while(1)									// forever loop
	{
		ltoa(X,num);
		//UARTprintf(" The value of coordinate X = ");
		UARTprintf(num);
		UARTprintf(".");
		ltoa(Y,num);
		//UARTprintf(" The value of coordinate Y = ");
		UARTprintf(num);
		UARTprintf("\n");
		ADCProcessorTrigger(ADC0_BASE, 1);
		ADCProcessorTrigger(ADC1_BASE, 1);

		while(!ADCIntStatus(ADC0_BASE, 1, false) && !ADCIntStatus(ADC1_BASE, 1, false));
		{
		}
		ADCIntClear(ADC0_BASE, 1);
		ADCIntClear(ADC1_BASE, 1);

		ADCSequenceDataGet(ADC0_BASE, 1, ADC_Raw_Y);
		ADCSequenceDataGet(ADC1_BASE, 1, ADC_Raw_X);

		X = (ADC_Raw_X[0] + ADC_Raw_X[1] + ADC_Raw_X[2] + ADC_Raw_X[3])/4;
		Y = (ADC_Raw_Y[0] + ADC_Raw_Y[1] + ADC_Raw_Y[2] + ADC_Raw_Y[3])/4;
		SysCtlDelay(670000*3);
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

	/* Configure Buzzer pin as output */
	 GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
	 GPIODirModeSet(GPIO_PORTC_BASE,GPIO_PIN_4,GPIO_DIR_MODE_OUT);
	/* Send a high output on buzzer to turn it off(inverted logic, refer
	schematic) */
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

UARTStdioConfig(0, 9600, 16000000);

}
