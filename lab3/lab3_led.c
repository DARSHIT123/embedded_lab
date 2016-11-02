#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#define LOCK_F (*((volatile unsigned long *)0x40025520))
#define CR_F (*((volatile unsigned long *)0x40025524))
#define PWM_FREQUENCY 55
char abc;
uint32_t ui32Period;
void hardware_init(void);
void ledToggle(void);
void ledToggle_rgb(void);
void delay(char abc);


volatile int16_t i16ToggleCount = 0;
volatile uint32_t sw2status = 0;
volatile uint32_t ui32Load;
volatile uint32_t ui32PWMClock;
volatile uint8_t ui8Adjustr;
volatile uint8_t ui8Adjustg;
volatile uint8_t ui8Adjustb;
volatile uint8_t ui8Adjust;
volatile uint32_t j=0;


void main(void)
{
   ui32Period = (SysCtlClockGet() / 2);  // for 10ms delay
   ui8Adjustr = 254;
   ui8Adjustg = 10;
   ui8Adjustb = 10;
   uint32_t i = 0;
   ui32PWMClock = SysCtlClockGet() / 64;
   ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;


   hardware_init();                                                        // init hardware via Xware

   while(1)                                                                        // forever loop
   {
           //Auto Mode
           if(i==0 && GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
           {
                   ui8Adjustr--;
                   ui8Adjustg++;
                   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjustb * ui32Load / 1000);
                   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjustr * ui32Load / 1000);
                   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjustg * ui32Load / 1000);

                   if(ui8Adjustg==244)
                   {
                           i=1;
                   }
           }
           else if(i==1 && GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
           {
                   ui8Adjustg--;
                   ui8Adjustb++;
                   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjustb * ui32Load / 1000);
                   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjustr * ui32Load / 1000);
                   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjustg * ui32Load / 1000);

                   if(ui8Adjustb==244)
                   {
                           i=2;
                   }
           }
           else if(i==2 && GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
           {
                   ui8Adjustb--;
                   ui8Adjustr++;
                   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjustb * ui32Load / 1000);
                   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjustr * ui32Load / 1000);
                   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjustg * ui32Load / 1000);

                   if(ui8Adjustr==244)
                   {
                           i=0;
                   }
           }

           else if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
           {
                   while(1)
                   {
                           if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
                           {
                                   j=j+1;
                                   SysCtlDelay(13400000);
                           }
                           else if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
                           {
                                   break;
                           }
                   }
                   if(j==1 || j==2 || j>2)
                   {
                           while(1)
                           {
                                   if(j==1)
                                   {
                                           PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1 * ui32Load / 1000);
                                           PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjustr * ui32Load / 1000);
                                           PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1 * ui32Load / 1000);
                                   }
                                   else if(j==2)
                                   {
                                           PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1 * ui32Load / 1000);
                                           PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1 * ui32Load / 1000);
                                           PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjustg * ui32Load / 1000);
                                   }
                                   else if(j>2)
                                   {
                                           PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjustb * ui32Load / 1000);
                                           PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1 * ui32Load / 1000);
                                           PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1 * ui32Load / 1000);
                                   }

                                   if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) || GPIOPinRead(GPIO_PORTF_BASE,
GPIO_PIN_0))
                                   {
                                           TimerEnable(TIMER0_BASE, TIMER_A);
                                   }
                           }
                   }
           }
           SysCtlDelay(1340000);
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
        //SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
        SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
        SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

        SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
        //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

        // ADD Tiva-C GPIO setup - enables port, sets pins 1-3 (RGB) pins for output
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        //GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);



        //Removing Locks From Switch
        LOCK_F=0x4C4F434B;
        CR_F=GPIO_PIN_0|GPIO_PIN_4;

        // Turn on the LED
        //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);
        GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4);

        //Switch Padding
        GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);

        //Make Port D pin 0 as PWM pin
            GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
                GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
                GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);
                GPIOPinConfigure(GPIO_PF1_M1PWM5);
                GPIOPinConfigure(GPIO_PF2_M1PWM6);
                GPIOPinConfigure(GPIO_PF3_M1PWM7);

        //Timer Initialisation
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
        TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
        TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
        TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
        TimerLoadSet(TIMER1_BASE, TIMER_A, ui32Period -1);
        TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
        TimerLoadSet(TIMER2_BASE, TIMER_A, ui32Period -1);

        //Timer Interrupt Enable
        IntEnable(INT_TIMER0A);
        IntEnable(INT_TIMER1A);
        IntEnable(INT_TIMER2A);
        TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
        TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
        TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
        IntMasterEnable();

        //PWM Configuration
        PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ui32Load);
        PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ui32Load);

        //PWM Generator Enable
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjustb * ui32Load / 1000);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjustr * ui32Load / 1000);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjustg * ui32Load / 1000);
        PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
        PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
        PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
        PWMGenEnable(PWM1_BASE, PWM_GEN_2);
        PWMGenEnable(PWM1_BASE, PWM_GEN_3);

}


void detectKeyPress()
{

        //Timer Disable
        TimerDisable(TIMER0_BASE, TIMER_A);

        //Clear the timer interrupt
        TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

        //Check switch is still pressed or not
        while(1)
        {
                SysCtlDelay(100000);
        if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
        {
                //TimerEnable(TIMER1_BASE, TIMER_A);
                if(j==1)
                {
                        ui8Adjustr = ui8Adjustr + 1;
                                                           if (ui8Adjustr > 254)
                                                           {
                                                           ui8Adjustr = 254;
                                                           }
                        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjustr * ui32Load / 1000);
                }
                else if(j==2)
                {
                        ui8Adjustg = ui8Adjustg + 1;
                                                           if (ui8Adjustg > 254)
                                                           {
                                                           ui8Adjustg = 254;
                                                           }
                        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjustg * ui32Load / 1000);
                }
                else if(j>2)
                {
                        ui8Adjustb = ui8Adjustb + 1;
                                                           if (ui8Adjustb > 254)
                                                           {
                                                           ui8Adjustb = 254;
                                                           }
                        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjustb * ui32Load / 1000);
                }
        }
        else if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
        {
                                                if(j==1)
                                                {
                                                        ui8Adjustr = ui8Adjustr - 1;
                                                                   if (ui8Adjustr < 10)
                                                                   {
                                                                   ui8Adjustr = 10;
                                                                   }
                                                    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjustr * ui32Load / 1000);
                                                }
                                                else if(j==2)
                                                {
                                                        ui8Adjustg = ui8Adjustg - 1;
                                                                   if (ui8Adjustg < 10)
                                                                   {
                                                                   ui8Adjustg = 10;
                                                                   }
                                                    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjustg * ui32Load / 1000);
                                                }
                                                else if(j>2)
                                                {
                                                        ui8Adjustb = ui8Adjustb - 1;
                                                                   if (ui8Adjustb < 10)
                                                                   {
                                                                   ui8Adjustb = 10;
                                                                   }
                                                    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjustb * ui32Load / 1000);
                                                }
        }
        }
}

//----------------------------------------------------------------------------
// Detect Key Press Confirmation
//----------------------------------------------------------------------------

void detectKeyConfirm_sw2()
{
        //Clear the timer interrupt
        TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

        //Timer Disable
                if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
                {
                TimerDisable(TIMER1_BASE, TIMER_A);
                if(j==1)
                {
                        ui8Adjustr = ui8Adjustr + 10;
                                   if (ui8Adjustr > 254)
                                   {
                                   ui8Adjustr = 254;
                                   }
                    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjustr * ui32Load / 1000);
                }
                else if(j==2)
                {
                        ui8Adjustg = ui8Adjustg + 10;
                                   if (ui8Adjustg > 254)
                                   {
                                   ui8Adjustg = 254;
                                   }
                    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjustg * ui32Load / 1000);
                }
                else if(j>2)
                {
                        ui8Adjustb = ui8Adjustb + 10;
                                   if (ui8Adjustb > 254)
                                   {
                                   ui8Adjustb = 254;
                                   }
                    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjustb * ui32Load / 1000);
                }
                }
}

void detectKeyConfirm_sw1(void)
{
        //Clear the timer interrupt
        TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

        //Timer Disable
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
        {
                TimerDisable(TIMER2_BASE, TIMER_A);
                                if(j==1)
                                {
                                        ui8Adjustr = ui8Adjustr - 10;
                                                   if (ui8Adjustr < 10)
                                                   {
                                                   ui8Adjustr = 10;
                                                   }
                                    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjustr * ui32Load / 1000);
                                }
                                else if(j==2)
                                {
                                        ui8Adjustg = ui8Adjustg - 10;
                                                   if (ui8Adjustg < 10)
                                                   {
                                                   ui8Adjustg = 10;
                                                   }
                                    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjustg * ui32Load / 1000);
                                }
                                else if(j>2)
                                {
                                        ui8Adjustb = ui8Adjustb - 10;
                                                   if (ui8Adjustb < 10)
                                                   {
                                                   ui8Adjustb = 10;
                                                   }
                                    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjustb * ui32Load / 1000);
                                }
                }
}


