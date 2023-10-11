# Embedded Lab UART Communication
## Objectives
Program your microontroller to transmit:<br>
"F0" if SW1 is pressed<br>
"AA" if SW2 is pressed <br>
Over UART with baud rate 9600 and odd parity. <br>
Your program should also listen for incoming data on the UART with the same baud and parity config; if "AA" is received LED should be GREEN; if "F0" is recieved, the LED should be BLUE and if any error is detected LED should be RED. Test this by communicating with your neighboring group.<br>
   
## Theory
### What is UART ?
UART, which stands for Universal Asynchronous Receiver/Transmitter, is a communication protocol widely used for serial communication between devices. It enables the transfer of data between devices using two wires: one for transmitting (TX) and the other for receiving (RX). UART is asynchronous, meaning that data is transmitted without a shared clock signal, and devices synchronize based on agreed-upon parameters such as baud rate, data bits, parity, and stop bits.

Key points about UART:

#### Asynchronous Communication: 
UART does not rely on a shared clock signal between devices. Instead, both the transmitting and receiving devices agree on communication parameters.

#### Basic Components:<br>
UART communication involves two main components—a transmitter and a receiver. The transmitter converts parallel data into serial form for transmission, and the receiver converts the received serial data back into parallel form.

#### Baud Rate: 
Baud rate defines the speed at which data is transmitted and received. Both communicating devices must operate at the same baud rate for successful communication.

#### Data Framing: 
Each piece of data is framed by start and stop bits, indicating the beginning and end of a data byte. This framing helps the receiver identify and extract the transmitted data.

#### Configurable Parameters: 
UART communication parameters include data bits (usually 8 bits per byte), parity (optional error-checking mechanism), and stop bits (typically 1 or 2 bits).

#### Half-Duplex or Full-Duplex: 
UART supports both half-duplex and full-duplex communication. In half-duplex mode, devices take turns transmitting and receiving, while in full-duplex mode, they can simultaneously transmit and receive.

#### Widespread Usage: 
UART is a fundamental communication protocol and is commonly used in various applications, including serial communication between microcontrollers, sensors, and peripheral devices.

### Calculation
 We calculate Baudrate Register parameters using 9600.
 We get integer and fractional part for baud rate speciification to register by  given expression<br>
   > 16000000/(16*9600)=104.1666 <br>
 

Integer part taken as 104 and we calculated fractional part using below expression
  > 0.167*64+0.5=11
## Code
```C
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

```

## Observation
We got these observations


![AA](https://github.com/Sohail-Ahmed020/EditReadme/blob/main/AA.jpeg)


![F0](https://github.com/Sohail-Ahmed020/EditReadme/blob/main/F0.jpeg)

## References
tm4c123gh6pm data sheet.






