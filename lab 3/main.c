#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#define SYST_CSR             (*((volatile uint32_t *)0xE000E010))
#define SYST_RVR             (*((volatile uint32_t *)0xE000E014))
#define SYST_CVR             (*((volatile uint32_t *)0xE000E018))
#define COUNTFLAG (1 << 16)
void system_config(void)
{
    SYSCTL_RCGC2_R |= 0x00000020;;
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x01;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_DEN_R = 0x1F;
    GPIO_PORTF_PUR_R = 0x11;
}


void delay(int ms)
{
    SYST_CVR&=0x00;
    SYST_RVR=ms;
    SYST_CSR|=0x05;
    while(((SYST_CSR & COUNTFLAG) == 0))
    {

    }
    SYST_CSR&=0x00;
}


int main(void)

 {
    system_config();
    GPIO_PORTF_DATA_R&=0x00;
    while(1)
    {
        GPIO_PORTF_DATA_R=0x02;
        delay(3200);
        GPIO_PORTF_DATA_R=0x00;
        delay(12800);

    }

}
