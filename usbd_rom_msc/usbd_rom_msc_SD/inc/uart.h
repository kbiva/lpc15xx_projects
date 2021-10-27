/*
 * uart.h
 *
 *  Created on: 2014.07.04
 *      Author: Kestutis Bivainis
 */

#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>

void init_UART0_pinmux(void);
void setup_UART(void);
void putline_UART(const char *send_data);

#endif
