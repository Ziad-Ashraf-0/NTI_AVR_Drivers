/*
 * STD_TYPES.h
 *
 *  Created on: Oct 21, 2023
 *      Author: Ziad
 */

#ifndef LIB_STD_TYPES_H_
#define LIB_STD_TYPES_H_


/*** 	NULL DEFINITION		***/
#define NULL 		( (void*) 0 )

#define true	1
#define false	0

/* PORTABLE DATA TYPES	*/

typedef unsigned char u8;
typedef signed char s8;

typedef unsigned short int u16;
typedef signed short int s16;

typedef unsigned long int u32;
typedef signed long int s32;

typedef unsigned long long int u64;
typedef signed long long int s64;

typedef float f32;
typedef double f64;
typedef long double f96;

#endif /* LIB_STD_TYPES_H_ */
