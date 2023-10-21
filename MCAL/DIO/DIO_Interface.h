/*
 * DIO_Interface.h
 *
 *  Created on: Oct 20, 2023
 *      Author: Ziad Ashraf
 */

#ifndef _DIO_INTERFACE_H_
#define _DIO_INTERFACE_H_

#include "../../LIB/STD_TYPES.h"


/*************************************************************************************/
/* 						USEFUL MACROS AS FUNCTIONS' ARGUMENTS   					 */
/*************************************************************************************/

/*************************************************************************************/
/* 								 	DIO PORTS   					 			     */
/*************************************************************************************/
#define DIO_PORTA 	0
#define DIO_PORTB 	1
#define DIO_PORTC 	2
#define DIO_PORTD 	3
/*************************************************************************************/

/*************************************************************************************/
/* 								 	DIO PINS	   					 			     */
/*************************************************************************************/
#define DIO_PIN0 	0
#define DIO_PIN1 	1
#define DIO_PIN2 	2
#define DIO_PIN3 	3
#define DIO_PIN4 	4
#define DIO_PIN5 	5
#define DIO_PIN6 	6
#define DIO_PIN7 	7
/*************************************************************************************/

/*************************************************************************************/
/* 								 DIO PINS DIRECTION					 			     */
/*************************************************************************************/
#define DIO_PIN_INPUT 	0
#define DIO_PIN_OUTPUT 	1
/*************************************************************************************/

/*************************************************************************************/
/* 								 DIO PORTS DIRECTION					 	         */
/*************************************************************************************/
#define DIO_PORT_INPUT 		0
#define DIO_PORT_OUTPUT 	255
/*************************************************************************************/

/*************************************************************************************/
/* 								DIO PINS VALUE					 			         */
/*************************************************************************************/
#define DIO_PIN_LOW 	0
#define DIO_PIN_HIGH 	1
/*************************************************************************************/


/*************************************************************************************/
/* 								FUNCTIONS PROTOTYPE 								 */
/*************************************************************************************/

/*************************************************************************************/
/* Description: Sets a whole port to either input or output   						 */
/* Input      : Port - Direction                                                     */
/* Output     : Error Checking                                                       */
/*************************************************************************************/
extern u8 DIO_U8SetPortDirection (const u8 LOC_U8Port, const u8 LOC_U8Direction);
/*************************************************************************************/

/**************************************************************************************/
/* Description: Sets a specific pin to either input or output  						  */
/* Input      : Port - Pin - Direction                                                */
/* Output     : Error Checking  	                                                  */
/**************************************************************************************/
extern u8 DIO_U8SetPinDirection (const u8 LOC_U8Port, const u8 LOC_U8Pin, const u8 LOC_U8Direction);
/**************************************************************************************/

/**************************************************************************************/
/* Description: Assign a certain value to a whole port 		  						  */
/* Input      : Port - Value	                                                      */
/* Output     : Error Checking                                                        */
/**************************************************************************************/
extern u8 DIO_U8SetPortValue (const u8 LOC_U8Port, const u8 LOC_U8Value);
/**************************************************************************************/

/**************************************************************************************/
/* Description: Assign either 1 or 0 to a specific pin		  						  */
/* Input      : Port - Pin - Value	                                                  */
/* Output     : Error Checking                                                        */
/**************************************************************************************/
extern u8 DIO_U8SetPinValue (const u8 LOC_U8Port, const u8 LOC_U8Pin, const u8 LOC_U8Value);
/**************************************************************************************/

/**************************************************************************************/
/* Description: Toggles a certain pin				 		  						  */
/* Input      : Port - Pin		                                                      */
/* Output     : Error Checking                                                        */
/**************************************************************************************/
extern u8 DIO_U8TogglePin (const u8 LOC_U8Port, const u8 LOC_U8Pin);
/**************************************************************************************/

/**************************************************************************************/
/* Description: Toggles a certain port				 		  						  */
/* Input      : Port			                                                      */
/* Output     : Error Checking                                                        */
/**************************************************************************************/
extern u8 DIO_U8TogglePort (const u8 LOC_U8Port);
/**************************************************************************************/

/**************************************************************************************/
/* Description: Stores the value of a certain pin in a given pointer				  */
/* Input      : Port - Pin - Pointer to value                                         */
/* Output     : Error Checking                                                        */
/**************************************************************************************/
extern u8 DIO_U8GetPinValue (const u8 LOC_U8Port, const u8 LOC_U8Pin, u8* const LOC_U8Value);
/**************************************************************************************/

/**************************************************************************************/
/* Description: Stores the value of a whole port in a given pointer					  */
/* Input      : Port - Pointer to value		                                          */
/* Output     : Error Checking                                                        */
/**************************************************************************************/
extern u8 DIO_U8GetPortValue (const u8 LOC_U8Port, u8* const LOC_U8Value);
/**************************************************************************************/


#endif
