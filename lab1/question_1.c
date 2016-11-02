#include<stdint.h>
#include<stdbool.h>
#include"inc/hw_memmap.h"
#include"inc/hw_types.h"
#include"driverlib/sysctl.h"
#include"driverlib/gpio.h"

#define LOCK_F (*((volatile unsigned long *)0x40025520))
#define CR_F (*((volatile unsigned long *)0x40025524))


uint8_t ui8PinData=2;
volatile uint8_t switch1_press=0;
void hardware_configure(void);

int main()
{
hardware_configure();

while(1)
{
//	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x02);
	if(!GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0))
				{
					switch1_press = switch1_press + 1;
						if(switch1_press == 4)
						{
							switch1_press = 0;

						}
				}
		if(switch1_press == 1)
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3, 4);

		}else if(switch1_press == 2)
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3, 8);

		}else if(switch1_press == 3)
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3, 2);

		}


}
return 0;
}
void hardware_configure()
{
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	LOCK_F=0x4C4F434B;
	CR_F=GPIO_PIN_0|GPIO_PIN_4;
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);



}

