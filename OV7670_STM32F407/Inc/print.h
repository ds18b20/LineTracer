/*
 * print.h
 *
 *  Created on: 2019/12/16
 *      Author: 0000400921
 */

#ifndef PRINT_H_
#define PRINT_H_

#include "usart.h"

void print_str(const char* str2print, unsigned char new_line);
void print_int(int int2print, unsigned char new_line);
void print_int_hex(int int2print, unsigned char new_line);
void print_float(float float2print, unsigned char new_line);

#endif /* PRINT_H_ */
