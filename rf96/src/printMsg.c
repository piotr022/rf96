/*
 * printMsg.c
 *
 *  Created on: Mar 19, 2020
 *      Author: Piotr
 */


#include "stm32f3xx.h"
#include "stdint.h"
#include <stdio.h>
#include "stdarg.h"
#include "string.h"
#include "printMsg.h"
#include "gpio.h"

#define buffer 10000
char buff[buffer]; //10000 characters! If you need less, then change this value AND (!) value value below as well

void usart_1_enable(void)
{
RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
GPIOA->MODER &= ~(GPIO_MODER_MODER9|GPIO_MODER_MODER10);
GPIOA->MODER |= GPIO_MODER_MODER9_1|GPIO_MODER_MODER10_1;
GPIOA->OTYPER = 0;                                                                                                             //Push-Pull
GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9_0 | GPIO_OSPEEDER_OSPEEDR9_1;     //Fast Speed
GPIOA->PUPDR |= GPIO_PUPDR_PUPDR9_0;
GPIOA->AFR[1] |= (0b0111 << 4) | (0b0111 << 8);

 USART1->BRR = 8000000/9600; //   9600 Baud


// USART1->CR1 |= USART_CR1_TE; //transmitter enable
 USART1->CR1 |= USART_CR1_RE; //receiver enable
 USART1->CR1 |= USART_CR1_UE; //usart enable
}

void printMsg(char *msg, ...)
{
 //char buff[120]; //was 80
 va_list args;
 va_start(args,msg);
 vsprintf(buff,msg,args);
 USART1->CR1 |= USART_CR1_TE;	//enable tx
 for(int i=0;i<strlen(buff);i++)
 {
   USART1->TDR = buff[i];
   while(!(USART1->ISR & USART_ISR_TXE)); //wait for TXE, 1 = data transferred
  }
 USART1->CR1 &= ~USART_CR1_TE;	//disable tx
}

int readMsg(void)
{

 for( len =0;len<buffer;len++)  //20000 characters! If you need less, then change this value AND (!) buff[] value above as well.
 {
    while(!(USART1->ISR & USART_ISR_RXNE));
    buff[len]= USART1->RDR;
    if (buff[len]==10 || buff[len]==13) break; //if enter is pressed, providing that it is 10 or 0xA (line feed), or 13 (0xD) (carriage return)
 }
 return len; //just returning number of entered characters, not including line feed nor carriage return (!)
}
