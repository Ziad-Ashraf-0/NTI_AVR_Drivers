/*
* keypad.c
*
*  Created on: Oct 21, 2023
*      Author: Ziad
*/
#include "../../MCAL/DIO/DIO_Interface.h"
#include "Keypad_interface.h"
#include "Keypad_config.h"
#include "Keypad_private.h"



u8 KEYPAD_getPressedKey(){
	u8 col,row;
	u8 prevKey = 0; // Previous key state
	u8 currentKey = 0; // Current key state
	// Configuration for the columns
	DIO_Config colPins[] = {
		{COL1_PORT, COL1_PIN, DIO_PIN_INPUT, DIO_PIN_HIGH},
		{COL2_PORT, COL2_PIN, DIO_PIN_INPUT, DIO_PIN_HIGH},
		{COL3_PORT, COL3_PIN, DIO_PIN_INPUT, DIO_PIN_HIGH},
		{COL4_PORT, COL4_PIN, DIO_PIN_INPUT, DIO_PIN_HIGH},
	};
	// Configuration for the rows
	DIO_Config rowPins[] = {
		{ROW1_PORT, ROW1_PIN, DIO_PIN_OUTPUT},
		{ROW2_PORT, ROW2_PIN, DIO_PIN_OUTPUT},
		{ROW3_PORT, ROW3_PIN, DIO_PIN_OUTPUT},
		{ROW4_PORT, ROW4_PIN, DIO_PIN_OUTPUT},
	};

	// Initialize the keypad pins
	for (u8 i = 0; i < 4; i++) {
		DIO_U8SetPinDirection(&colPins[i]);
		DIO_U8SetPinDirection(&rowPins[i]);
		DIO_U8SetPinValue(&colPins[i]);
	}

	while(1)
	{
		/*for loop to scan all keypad columns*/
		for (row = 0 ; row < 4 ; row++)
		{
			/*set current row pin low to test all col pins to find out which button has been pressed*/
			rowPins[row].value = DIO_PIN_LOW;
			DIO_U8SetPinValue(&rowPins[row]);
			/*for loop to scan and test all keypad row pins*/
			for (col = 0 ; col < 4 ; col++)
			{
				DIO_U8GetPinValue(&colPins[col], &currentKey);

				/* Check current row pin value */
				if (DIO_PIN_LOW == currentKey && DIO_PIN_HIGH == prevKey) {
					/* Key is pressed, return the pressed button value */
					while (currentKey != DIO_PIN_HIGH) {
						DIO_U8GetPinValue(&colPins[col], &currentKey);
					}
					return KEYPAD_4x4_adjustKeyNumber((row * KEYPAD_NUM_COLS) + col + 1);
				}

				prevKey = currentKey; // Update the previous key state
				/*end of for loop*/
			}
			/*set current column pin HIGH again*/
			rowPins[row].value = DIO_PIN_HIGH;
			DIO_U8SetPinValue(&rowPins[row]);

		}
		/*end of for loop*/
	}
}


/*
* Description :
* Update the keypad pressed button value with the correct one in keypad 4x4 shape
*/
static u8 KEYPAD_4x4_adjustKeyNumber(u8 button_number)
{
	u8 keypad_button = 0;
	switch(button_number)
	{
		case 1: keypad_button = 7;
		break;
		case 2: keypad_button = 8;
		break;
		case 3: keypad_button = 9;
		break;
		case 4: keypad_button = '%'; // ASCII Code of %
		break;
		case 5: keypad_button = 4;
		break;
		case 6: keypad_button = 5;
		break;
		case 7: keypad_button = 6;
		break;
		case 8: keypad_button = '*'; /* ASCII Code of '*' */
		break;
		case 9: keypad_button = 1;
		break;
		case 10: keypad_button = 2;
		break;
		case 11: keypad_button = 3;
		break;
		case 12: keypad_button = '-'; /* ASCII Code of '-' */
		break;
		case 13: keypad_button = 13;  /* ASCII of Enter */
		break;
		case 14: keypad_button = 0;
		break;
		case 15: keypad_button = '='; /* ASCII Code of '=' */
		break;
		case 16: keypad_button = '+'; /* ASCII Code of '+' */
		break;
		default: keypad_button = button_number;
		break;
	}
	return keypad_button;
}
