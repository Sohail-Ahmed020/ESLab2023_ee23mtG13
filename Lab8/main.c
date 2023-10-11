#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

#define MASK_BITS   0x11

int main(void)
{
    SYSCTL_RCGC2_R |= 0x00000020;
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x01;
    GPIO_PORTF_DEN_R = 0x1F;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_PUR_R = 0x11;

    GPIO_PORTF_IM_R &= ~MASK_BITS;
    GPIO_PORTF_IS_R &= ~MASK_BITS;
    GPIO_PORTF_IEV_R  &= ~MASK_BITS;
    GPIO_PORTF_IBE_R &= ~MASK_BITS;

    NVIC_EN0_R |= (1 << 30);
    GPIO_PORTF_ICR_R = MASK_BITS;
    GPIO_PORTF_IM_R |= MASK_BITS;

    SYSCTL_RCGCUART_R |= 0x80;
    SYSCTL_RCGC2_R |= 0x10;
    GPIO_PORTE_DEN_R |= 0x03;
    GPIO_PORTE_DIR_R |= 0x02;

    GPIO_PORTE_AFSEL_R |= 0x03;
    GPIO_PORTE_PCTL_R |= 0x000011;
    UART7_CTL_R &= 0xFFFFFFFE;
    UART7_IBRD_R = 104;
    UART7_FBRD_R = 11;
    UART7_LCRH_R |= 0x62;
    UART7_CC_R &= 0xFFFFFFF0;
    UART7_CTL_R |= ((1<<0)|(1<<8)|(1<<9));

    UART7_IM_R &= 0x00;
    UART7_ICR_R &= 0x00;
    UART7_IM_R |=(1<<4);
    NVIC_EN1_R |= (1<<31);

    while(1)
    {

    }
}

void GPIOF_INT_Handler(void)
{
    GPIO_PORTF_IM_R &= ~MASK_BITS;
    int i;

     if(GPIO_PORTF_RIS_R & 0x01)
     {
         for(i=0;i<16000;i++){}
         if(~(GPIO_PORTF_DATA_R)&0x01)
         {
             UART7_DR_R = 0xAA;
         }
     }

     if(GPIO_PORTF_RIS_R & 0x10)
     {
         for(i=0;i<16000;i++){}
         if(~(GPIO_PORTF_DATA_R)&0x10)
         {
             UART7_DR_R = 0xF0;
         }
     }
     GPIO_PORTF_ICR_R = MASK_BITS;
     GPIO_PORTF_IM_R |= MASK_BITS;
}

void UART7_Handler(void)
{
    UART7_IM_R &= 0x00;

    if(UART7_FR_R & (1<<6))
    {
        if(UART7_DR_R == 0xF0)
        {
            GPIO_PORTF_DATA_R |= 0x04;
        }
        else if(UART7_DR_R == 0xAA)
        {
            GPIO_PORTF_DATA_R |= 0x08;
        }
        else if(UART7_RSR_R & 0xF)
        {
            GPIO_PORTF_DATA_R |= 0x02;
        }
    }
    int i;
    for(i=0;i<800000;i++){}
    GPIO_PORTF_DATA_R &= 0x00;
    UART7_ECR_R &= ~(0xF);
    UART7_ICR_R &= 0x00;
    UART7_IM_R |= (1<<4);
}
