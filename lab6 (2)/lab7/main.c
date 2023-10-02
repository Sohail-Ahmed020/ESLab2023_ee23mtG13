#include<stdio.h>
#include<stdint.h>
#include"tm4c123gh6pm.h"
#define STCTRL *((volatile long *) 0xE000E010)
#define STRELOAD *((volatile long *) 0xE000E014)
#define STCURRENT *((volatile long *) 0xE000E018)
#define Sw_Bits 0x11
void sys_config()
{
    SYSCTL_RCGCPWM_R |=(1<<1);
    SYSCTL_RCGCGPIO_R |= (1<<5);
    SYSCTL_RCC_R &= ~(1<<20);
GPIO_PORTF_DIR_R = 0x0E;
GPIO_PORTF_DEN_R = 0x1F;
GPIO_PORTF_PUR_R = 0x11;
    GPIO_PORTF_AFSEL_R |= 1<<3;
    GPIO_PORTF_PCTL_R |= 0x5000;

    GPIO_PORTF_PUR_R |= 0x11;
}
void pwm_config()
{   PWM1_3_CTL_R &= ~(1<<0);
    PWM1_3_CTL_R &= ~(1<<1);
    PWM1_3_LOAD_R |= 160;
    PWM1_3_CMPB_R = 80;
    PWM1_3_GENB_R = (1<<3) | (1<<2) | (1<<11);
    PWM1_3_CTL_R |= (1<<0);
    PWM1_ENABLE_R |= (1<<7);
}
void prio()
{
    GPIO_PORTF_IM_R &= ~(0x11);
    GPIO_PORTF_IS_R &= ~(0x11);
    GPIO_PORTF_IBE_R &= (0x11);

    GPIO_PORTF_IM_R |= 0x11;


    NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF1FFFFF) |(1<<21);
    NVIC_EN0_R |=(1<<30);
}
void sys_handler()
{
    STCTRL = 0x00;
        if((GPIO_PORTF_DATA_R) & 0x10)
            {
                if(PWM1_3_CMPB_R > 8)
                {
                    PWM1_3_CMPB_R -=8;
                }
            }
        else
            {
                if(PWM1_3_CMPB_R < 152)
                {
                    PWM1_3_CMPB_R +=8;
                }
            }
        GPIO_PORTF_ICR_R = Sw_Bits;
        GPIO_PORTF_IM_R |= Sw_Bits;
}
void portF_handler()
{  GPIO_PORTF_IM_R &= ~Sw_Bits;
    if(GPIO_PORTF_DATA_R)
    STCURRENT = 0x00;
    STRELOAD = (8*1000000);
    STCTRL |=0x07;
    int j;
    for(j = 0; j <1600*1000/8; j++){}

}


void main()
 {
    sys_config();
    pwm_config();
    prio();
    while(1){}// do nothing

}


