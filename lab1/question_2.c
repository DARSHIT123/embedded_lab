#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include <time.h>


#define LOCK_F (*((volatile unsigned long *)0x40025520))
#define CR_F (*((volatile unsigned long *)0x40025524))
char change;
char freq;
void hardware_init(void);
void ledtoggle(char change, char freq);
void delay(char freq);
void main(void)
{

   hardware_init();							// init hardware via Xware
   change='8';
   freq='1';
   while(1)									// forever loop
   {
	   if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
	   {
		   if(freq == '1')
			   freq = '2';
		   else if(freq == '2')
			   freq = '3';
		   else if(freq == '3')
			   freq = '1';
	   }
	   if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
	   	   {
	   		   if(change == '2')
	   			   change = '4';
	   		   else if(change == '4')
	   			   change = '8';
	   		   else if(change == '8')
	   			   change = '2';
	   	   }
	   ledtoggle(change, freq);
   }
}

void hardware_init(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	LOCK_F=0x4C4F434B;
	CR_F=GPIO_PIN_0|GPIO_PIN_4;
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4);
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
}


void ledtoggle( char change,char freq)
{
		if(change == '2')
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);
			delay(freq);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
			delay(freq);

		}
		else if(change == '4')
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);
			delay(freq);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
			delay(freq);
		}
		else if(change == '8')
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);
			delay(freq);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
			delay(freq);
		}
}

void delay(char freq)
{
	 switch(freq) {
	       case '1' :
	    	  SysCtlDelay(6700000);		// creates ~500ms delay - TivaWare fxn
	          break;
	       case '2' :
	    	  SysCtlDelay(13400000);		// creates ~1s delay - TivaWare fxn
	    	  break;
	       default :
	    	  SysCtlDelay(26800000);		// creates ~2s delay - TivaWare fxn
	          break;

	    }

}



