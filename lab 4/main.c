#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#define led  int[3]  [0x02,0x04,0x08]
#define SYST_CSR             (*((volatile uint32_t *)0xE000E010))
#define SYST_RVR             (*((volatile uint32_t *)0xE000E014))
#define SYST_CVR             (*((volatile uint32_t *)0xE000E018))
#define MASK_BITS 0x11

void system_config(void)
{
    SYSCTL_RCGC2_R |= 0x00000020;;
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x01;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_DEN_R = 0x1F;
    GPIO_PORTF_PUR_R = 0x11;
}


/*id delay(int ms)
{
    SYST_CVR&=0x00;
    SYST_RVR=ms;
    SYST_CSR|=0x01;
    while((SYST_CSR==0x00001))
    {

    }
    SYST_CSR&=0x00;
}*/
void SysTick_Handler(void)
{
    GPIO_PORTF_DATA_R =0x00;
    SYST_CSR=0x00;
    GPIO_PORTF_ICR_R = MASK_BITS;
    GPIO_PORTF_IM_R |= MASK_BITS;
}

void portf_Handler(void)
{
    SYST_CVR&=0x00;
    if(GPIO_PORTF_MIS_R & 0X01)
    {
        GPIO_PORTF_DATA_R =0x04;
        GPIO_PORTF_ICR_R = MASK_BITS;
        GPIO_PORTF_IM_R &= ~MASK_BITS;
        SYST_RVR = 16*1000000;
        SYST_CSR|=0x07;
        NVIC_EN0_R &= 0 << 30;


    }

    //GPIO_PORTF_ICR_R = MASK_BITS;
}

int main(void)

 {
    system_config();
    SYST_CVR&=0x00;
    SYST_RVR=16000000;
    //SYST_CSR|=0x03;
    GPIO_PORTF_DATA_R&=0x00;
    GPIO_PORTF_IM_R &= ~MASK_BITS; // mask interrupt by clearing bits
    GPIO_PORTF_IS_R &= ~MASK_BITS; // edge sensitive interrupts
    GPIO_PORTF_IBE_R &= ~MASK_BITS; // interrupt NOT on both edges
    GPIO_PORTF_IEV_R &= ~MASK_BITS;

    /* Prioritize and enable interrupts in NVIC */
    NVIC_PRI7_R = (NVIC_PRI7_R & 0xF1FFFFFF) | (7 << 21);
     // interrupt priority register 7
     // bits 21-23 for interrupt 30 (port F)
    NVIC_EN0_R |= 1 << 30; // enable interrupt 30 (port F)
    /* Enable interrupt generation in GPIO */
    GPIO_PORTF_ICR_R = MASK_BITS; // clear any prior interrupt
    GPIO_PORTF_IM_R |= MASK_BITS;
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x1FFFFFFF) | (3 << 29) ;
    while(1)
    {


    }

}
