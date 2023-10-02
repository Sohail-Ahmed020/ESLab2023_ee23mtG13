# Embedded Lab 7 PWM Generation
## Objectives
1. Create a PWM waveform with frequency = 100KHz and variable duty cycle.The program should begin with d = 50%.
   On pressing one switch the duty should be increased by 5% and on pressing other switch it should be decreased by 5%.
2. Implement the same but using only 1 switch (SW1 OR SW2) – short press for d increase and long press for decrease.
   
## Theory
### What is PWM ?
PWM, which stands for Pulse Width Modulation, is a technique used in embedded systems and microcontrollers, including those based on the ARM Cortex-M4 architecture, to generate analog-like signals digitally.

PWM is implemented using the inbuild PWM peripheral.The peripherals allow us to generate square wave form with a variable duty cycle.

### Functional Description 
#### Clock Configuration 
The PWM has 2 clock source
1. The system clock
2. Apredivided sustem clock

The clock source can be seting the bit in the RCC(Run Mode Clock Configuration) register at the System control offset.

#### PWM Timer
The timer in each of the PWM generator work in 2 mode : Count-Up/Down and Count-Dowm Mode. In the Count Down mode the counter count from load value to zero value and then goes back to load value. In Count-Up/Down the counter start from the zero to load value then it goes back to zero.

#### To Access the PWM Timer we have to :
1. Initialize the PWM Peripheral:
2. Configure PWM Pins
3. Configure PWM Pins
4. Set the PWM Duty Cycle
5. Set the PWM Duty Cycle
6. Start the timer to get the PWM Siginal as the output
### Calculation
100KHz=16000000samples/sec
so in one time period there are 160 samples.
for Initial 50% duty cycle 80 samples and for each five percent equal to 8 samples increase and decrease as per the task.
## Code
 ### Part 1

 We have to create 100kHz PWM signal with initial 50% duty cycle and then we use Switch SW1 to increase and Switch Sw2 to decrease the duty cycle.
 >#include<stdio.h><br>
#include<stdint.h><br>
#include"tm4c123gh6pm.h"<br>
void sys_config()<br>
{<br>
    SYSCTL_RCGCPWM_R |=(1<<1);    // enable and provide clock to pwm 1 module<br>
    SYSCTL_RCGCGPIO_R |= (1<<5);  //enable clock to PORTF<br>
    SYSCTL_RCC_R &= ~(1<<20);<br>
    GPIO_PORTF_LOCK_R = 0x4C4F434B;<br>
GPIO_PORTF_CR_R = 0x01;<br>
GPIO_PORTF_DIR_R = 0x0E;<br>
GPIO_PORTF_DEN_R = 0x1F;<br>
GPIO_PORTF_PUR_R = 0x11;<br>
    GPIO_PORTF_AFSEL_R |= 1000;          // setting port f pin 2 as alternate function reg<br>
    GPIO_PORTF_PCTL_R |= 0x5000; // setting PF2 as M1PWM6<br>
    // initializing switch 1 and switch 2 as input pins<br>
    GPIO_PORTF_PUR_R |= 0x11; // enabling pull up resistors for sw1 and sw2<br>
}<br>
void pwm_config()<br>
{ <br>  PWM1_3_CTL_R &= ~(1<<0); // disabling the counter before generating PWM<br>
    PWM1_3_CTL_R &= ~(1<<1); // setting down counting mode<br>
    PWM1_3_LOAD_R |= 160; // loading 160 in the register for a time period of 100khz<br>
    PWM1_3_CMPB_R = 80;   // comparator value for 50% duty cycle<br>
    PWM1_3_GENB_R = (1<<3) | (1<<2) | (1<<11);<br>
    PWM1_3_CTL_R |= (1<<0); // enabling the counter<br>
    PWM1_ENABLE_R |= (1<<7);<br>
}<br>
void prio()<br>
{<br>
    GPIO_PORTF_IM_R &= ~(0x11); // masking sw1 and sw2.<br>
    GPIO_PORTF_IS_R &= ~(0x11); // edge triggered<br>
    GPIO_PORTF_IBE_R &= ~(0x11);// interrupt generation controlled by GPIOIEV<br>
    GPIO_PORTF_IEV_R |= 0x11; // rising edge causes and interrupt<br>
    GPIO_PORTF_IM_R |= 0x11;  // unmasking sw1 and sw2<br>
    // setting priorities using NVIC<br>
    NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF1FFFFF) |(1<<21);<br>
    NVIC_EN0_R |=(1<<30);<br>
}<br>
void portF_handler()<br>
{<br> int k=0,i = 10000;<br>
    k=GPIO_PORTF_MIS_R;<br>
    GPIO_PORTF_IM_R &= ~(0x11);//sw1 and sw2 masked<br>
    if(k & 0X01) //interrupt on switch 1<br>
    {<br>
        if(PWM1_3_CMPB_R < 152)<br>
            PWM1_3_CMPB_R += 8; // increasing duty cycle by 5%<br>
    }<br>
    if(k & 0X10) // Interrupt on switch 2<br>
    {<br>        if(PWM1_3_CMPB_R > 8)<br>
            PWM1_3_CMPB_R -= 8;// decreasing the duty cycle by 5%<br>
    }<br>
    while(i--);// for debounce<br>
    GPIO_PORTF_IM_R |= 0x11; // unmasking sw1 and sw2<br>
    GPIO_PORTF_ICR_R |= 0x11;<br>
}<br>
void main()<br>
{<br>
    sys_config();<br>
    pwm_config();<br>
    prio();<br>
    while(1)<br>{<br>}// do nothing<br>
}<br>

### Part 2

We have to create the PWM signal of 100kHz and changes its duty cycle using short press of switch to increase the duty cycle by 5% and long press of the switch to decrease the duty cycle by 5%.
> #include<stdio.h><br>
#include<stdint.h><br>
#include"tm4c123gh6pm.h"<br>
#define STCTRL *((volatile long *) 0xE000E010)<br>
#define STRELOAD *((volatile long *) 0xE000E014)<br>
#define STCURRENT *((volatile long *) 0xE000E018)<br>
#define Sw_Bits 0x11<br>
void sys_config()<br>
{<br>
    SYSCTL_RCGCPWM_R |=(1<<1);<br>
    SYSCTL_RCGCGPIO_R |= (1<<5);<br>
    SYSCTL_RCC_R &= ~(1<<20);<br>
GPIO_PORTF_DIR_R = 0x0E;<br>
GPIO_PORTF_DEN_R = 0x1F;<br>
GPIO_PORTF_PUR_R = 0x11;<br>
    GPIO_PORTF_AFSEL_R |= 1<<3;<br>
    GPIO_PORTF_PCTL_R |= 0x5000;<br>
GPIO_PORTF_PUR_R |= 0x11;<br>
}<br>
void pwm_config()<br>
{ <br>  PWM1_3_CTL_R &= ~(1<<0);<br>
    PWM1_3_CTL_R &= ~(1<<1);<br>
    PWM1_3_LOAD_R |= 160;<br>
    PWM1_3_CMPB_R = 80;<br>
    PWM1_3_GENB_R = (1<<3) | (1<<2) | (1<<11);<br>
    PWM1_3_CTL_R |= (1<<0);<br>
    PWM1_ENABLE_R |= (1<<7);<br>
}<br>
void prio()<br>
{<br>
    GPIO_PORTF_IM_R &= ~(0x11);<br>
    GPIO_PORTF_IS_R &= ~(0x11);<br>
    GPIO_PORTF_IBE_R &= (0x11);<br>
    GPIO_PORTF_IM_R |= 0x11;<br>
    NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF1FFFFF) |(1<<21);<br>
    NVIC_EN0_R |=(1<<30);<br>
}<br>
void sys_handler()<br>
{<br>
    STCTRL = 0x00;<br>
        if((GPIO_PORTF_DATA_R) & 0x10)<br>
            {<br>
                if(PWM1_3_CMPB_R > 8)<br>
                {<br>
                    PWM1_3_CMPB_R -=8;<br>
                }<br>
            }<br>
        else<br>
            {<br>
                if(PWM1_3_CMPB_R < 152)<br>
                {<br>
                    PWM1_3_CMPB_R +=8;<br>
                }<br>
            }<br>
        GPIO_PORTF_ICR_R = Sw_Bits;<br>
        GPIO_PORTF_IM_R |= Sw_Bits;<br>
}<br>
void portF_handler()<br>
{  GPIO_PORTF_IM_R &= ~Sw_Bits;<br>
    if(GPIO_PORTF_DATA_R)<br>
    STCURRENT = 0x00;<br>
    STRELOAD = (8*1000000);<br>
    STCTRL |=0x07;<br>
    int j;<br>
    for(j = 0; j <1600*1000/8; j++)<br>{<br>}<br>
}<br>
void main()<br>
 {<br>
    sys_config();<br>
    pwm_config();<br>
    prio();<br>
    while(1)<br>{<br>}// do nothing<br>
}<br>
## Observation
We get the 100kHz pwm signal with 50 percent duty cycle.All the result are shown below:
![D of approx 50%](https://github.com/Sohail-Ahmed020/EditReadme/blob/main/image/49%25dutycycle.jpeg)

![D change by 5% using Switch](image/54%25duty.jpeg)

![D initial approx 50%](/image/49%25d.jpeg)

![D change by 5%](image/54%25duty.jpeg)

![D change by 10%](image/59%25d.jpeg)





