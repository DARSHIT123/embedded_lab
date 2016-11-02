//---------------------------------------------------------------------------
// Project: Experiment - 2 : Lab Assignment : Interrupts And Timers
//
// Date: 07/August/2016
//
//---------------------------------------------------------------------------


//------------------------------------------
// TivaWare Header Files
//------------------------------------------
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


//----------------------------------------
// Defines
//----------------------------------------
#define LOCK_F (*((volatile unsigned long *)0x40025520))
#define CR_F (*((volatile unsigned long *)0x40025524))
#define PWM_FREQUENCY 55
char abc;
uint32_t Period;


//----------------------------------------
// Prototypes
//----------------------------------------
void hardware_init(void);
void ledToggle(void);
void ledToggle_rgb(void);
void delay(char abc);


//---------------------------------------
// Globals
//---------------------------------------
volatile int16_t i16ToggleCount = 0;
volatile uint32_t sw2status = 0;
volatile uint32_t ui32Load;
volatile uint32_t ui32PWMClock;
volatile uint8_t ui8Adjust;


//---------------------------------------------------------------------------
// main()
//---------------------------------------------------------------------------
void main(void)
{
   Period = (SysCtlClockGet() / 2);  // for 10ms delay
   ui8Adjust = 83;
   ui32PWMClock = SysCtlClockGet() / 64;
   ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;


   hardware_init();                                                        // init hardware via Xware

   while(1)                                                                        // forever loop
   {
           if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
           {
                   TimerEnable(TIMER0_BASE, TIMER_A);

           }

           if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
           {
                   TimerEnable(TIMER0_BASE, TIMER_A);
           }

   }
}

void hardware_init(void)
{
        SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
        SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
        GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
        GPIOPinConfigure(GPIO_PD0_M1PWM0);
        LOCK_F=0x4C4F434B;
        CR_F=GPIO_PIN_0|GPIO_PIN_4;
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);
        GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4);
        GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
        TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
        TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
        TimerLoadSet(TIMER0_BASE, TIMER_A, Period -1);
        TimerLoadSet(TIMER1_BASE, TIMER_A, Period -1);
        TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
        TimerLoadSet(TIMER2_BASE, TIMER_A, Period -1);
        IntEnable(INT_TIMER0A);
        IntEnable(INT_TIMER1A);
        IntEnable(INT_TIMER2A);
        TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
        TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
        TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
        IntMasterEnable();
        PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, ui32Load);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 360);
        PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
        PWMGenEnable(PWM1_BASE, PWM_GEN_0);

}

void detectKeyConfirm_sw2()
{
        TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
                {
                TimerDisable(TIMER1_BASE, TIMER_A);
                ledToggle_rgb();
                ui8Adjust = ui8Adjust + 10;
                                   if (ui8Adjust > 111)
                                   {
                                   ui8Adjust = 111;
                                   }
                                   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 360);
                }
}

void detectKeyConfirm_sw1(void)
{
        TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
                {
                TimerDisable(TIMER2_BASE, TIMER_A);
                sw2status = sw2status + 1;
                ui8Adjust = ui8Adjust - 10;
                                   if (ui8Adjust < 56)
                                   {
                                   ui8Adjust = 56;
                                   }
                                   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 360);
                }
}

void detectKeyPress()
{

        TimerDisable(TIMER0_BASE, TIMER_A);
        TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
        if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
        {
                TimerEnable(TIMER1_BASE, TIMER_A);
        }
        else if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
        {
                TimerEnable(TIMER2_BASE, TIMER_A);
        }
}

