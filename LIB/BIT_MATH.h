/*
 * BIT_MATH.h
 *
 *  Created on: Oct 21, 2023
 *      Author: Ziad
 */

#ifndef LIB_BIT_MATH_H_
#define LIB_BIT_MATH_H_

/************************************************************************************/
/*								BIT MANIPULATION									*/
/************************************************************************************/

/* Sets bit number bit_num in variable var to 1 								   	*/
#define SET_BIT(var, bit_num) var |= (1 << bit_num)

/* Sets bit number bit_num in variable var to 0 									*/
#define CLR_BIT(var, bit_num) var &= ~(1 << bit_num)

/* Toggles bit number bit_num in variable var 										*/
#define TOG_BIT(var, bit_num) var ^= (1 << bit_num)

/* Returns bit number bit_num in variable var 										*/
#define GET_BIT(var, bit_num) ( 1 & (var >> bit_num) )

/* Writes bit number bit_num in variable var with the value val 					*/
#define WRITE_BIT(var, bit_num, val) var = (var & (~(1 << bit_num))) | (val << bit_num)

/* Check if a specific bit is set in any register and return true if yes			*/
#define BIT_IS_SET(REG,BIT) ( REG & (1<<BIT) )

/* Check if a specific bit is cleared in any register and return true if yes		*/
#define BIT_IS_CLEAR(REG,BIT) ( !(REG & (1<<BIT)) )

#endif /* LIB_BIT_MATH_H_ */
