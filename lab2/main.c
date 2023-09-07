#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#define SYST_CSR             (*((volatile uint32_t *)0xE000E010))
#define SYST_RVR             (*((volatile uint32_t *)0xE000E014))
#define SYST_CVR             (*((volatile uint32_t *)0xE000E018))
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
    SYST_CSR|=0x01;
    while((SYST_CSR==0x00001))
    {

    }
    SYST_CSR&=0x00;
}


void c1(void)

{
    system_config();

        while(1)
        {
        if(GPIO_PORTF_DATA_R & 0x01){GPIO_PORTF_DATA_R&=0x00;}
        if(~GPIO_PORTF_DATA_R & 0x01){GPIO_PORTF_DATA_R|=0x02;
        }}
}
/*
--------------------------------------------------------------------------------------------------------------------------------------------------------

b
--------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void c2(void)

 {
    system_config();
    int t=0,c=0,n;

        while(1)
        {
            n=(~GPIO_PORTF_DATA_R & 0x01);
            delay(150);
            if(n==1 && t==0)
            {

                                if(c==0){GPIO_PORTF_DATA_R=0x02;}
                                if(c==1){GPIO_PORTF_DATA_R=0x08;}
                                if(c==2){GPIO_PORTF_DATA_R=0x04;}
                                c++;
                                if(c==3){c=0;}


            }t=n;

        }
}

int main(void)
{
int x=2;
if (x==1)
        {
           c1();
        }
if (x==2)
        {
           c2();
        }
}
