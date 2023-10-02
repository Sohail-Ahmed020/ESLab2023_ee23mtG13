#include<stdio.h>
#include<stdint.h>
#include"tm4c123gh6pm.h"

void sys_config()
{
    SYSCTL_RCGCPWM_R |=(1<<1);
    SYSCTL_RCGCGPIO_R |= (1<<5);
    SYSCTL_RCC_R &= ~(1<<20);
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
GPIO_PORTF_CR_R = 0x01;
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
    GPIO_PORTF_IBE_R &= ~(0x11);
    GPIO_PORTF_IEV_R |= 0x11;
    GPIO_PORTF_IM_R |= 0x11;
    NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF1FFFFF) |(1<<21);
    NVIC_EN0_R |=(1<<30);
}
void portF_handler()
{ int k=0,i = 10000;
    k=GPIO_PORTF_MIS_R;
    GPIO_PORTF_IM_R &= ~(0x11);
    if(k & 0X01)
    {
        if(PWM1_3_CMPB_R < 152)
            PWM1_3_CMPB_R += 8;
    }
    if(k & 0X10)
    {        if(PWM1_3_CMPB_R > 8)
            PWM1_3_CMPB_R -= 8;
    }
    while(i--);
    GPIO_PORTF_IM_R |= 0x11;
    GPIO_PORTF_ICR_R |= 0x11;

}


void main()
{
    sys_config();
    pwm_config();
    prio();
    while(1){}

}


