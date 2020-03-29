/*
 * printMsg.h
 *
 *  Created on: Mar 19, 2020
 *      Author: Piotr
 */

#ifndef PRINTMSG_H_
#define PRINTMSG_H_

extern int len;
extern char buff[];
//char buff[];

void usart_1_enable(void);
void printMsg(char *msg, ...);
int readMsg(void);
void USART1_IRQHandler(void);

#endif /* PRINTMSG_H_ */
