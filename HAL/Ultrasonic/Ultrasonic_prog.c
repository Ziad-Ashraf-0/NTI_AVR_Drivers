/*
 * Ultrasonic.c
 *
 * Created: 10/30/2023 10:26:37 AM
 *  Author: Ziad
 */ 
#define F_CPU 16000000UL
#include "../../MCAL/DIO/DIO_Interface.h"
#include "../../MCAL/TIMER1/TIMER1_interface.h"
#include "../../LIB/BIT_MATH.h"
#include "Ultrasonic_config.h"
#include "Ultrasonic_interface.h"
#include "Ultrasonic_private.h"
#include <util/delay.h>

static u16 g_distance;
static u16 g_secondRead;
static u8 g_edges_counter;
static u16 g_OVRflow = 0;



void TimerOVFCallBack(){
	g_OVRflow++;
}

/*
 *  Description :1- Initialize the ICU driver as required.
                 2- Setup the ICU call back function.
                 3- Setup the direction for the trigger pin as output pin through the GPIO driver.
     Inputs      : None
     Return      : None
 * */
void Ultrasonic_init(void){
	/*
	 * setup the icu : 1- frequency -> fcpu/1024
	 *                 2- start with first rising edge
	 * */
	Timer1_Config timerConfig1 = {Timer1_NORMAL,Timer1_PRESCALER_1024};
	M_TIMER1_void_setCallBack(Ultrasonic_edgeProcessing,TIMER1_ICU);
	M_TIMER1_void_setCallBack(TimerOVFCallBack,TIMER1_OVERFLOW);
	M_TIMER1_void_Init(&timerConfig1);
	M_TIMER1_void_IntEnable(TIMER1_ICU);
	M_TIMER1_void_IntEnable(TIMER1_OVERFLOW);
	M_TIMER1_void_setEdgeDetectionType(RISING);
	M_TIMER1_void_start();
	
	
	/* setup the direction of the trigger to be output pin */
	DIO_Config trig_bin = {ULTRASONIC_TRIGGER_PORTID,ULTRASONIC_TRIGGER_PINID,DIO_PIN_OUTPUT};
	DIO_U8SetPinDirection(&trig_bin);
	
	/* setup the direction of the echo to be input pin */
	DIO_Config trig_bin1 = {DIO_PORTD,DIO_PIN6,DIO_PIN_INPUT};
	DIO_U8SetPinDirection(&trig_bin1);
}


/*
� Description : Send the Trigger pulse to the ultrasonic.
� Inputs      : None
� Return      : None
 */
void Ultrasonic_Trigger(void){
	/*give the trigger logic high (1) to start send signal */
	DIO_Config trig_bin = {ULTRASONIC_TRIGGER_PORTID,ULTRASONIC_TRIGGER_PINID,DIO_PIN_OUTPUT,DIO_PIN_HIGH};
	DIO_U8SetPinValue(&trig_bin);
	/*delay for sending signal*/
	_delay_ms(10);
	/*give the trigger logic low (0) to stop send signal */
	trig_bin.value=DIO_PIN_LOW;
	DIO_U8SetPinValue(&trig_bin);
}


/*
� Description : 1-Send the trigger pulse by using Ultrasonic_Trigger function.
                2-Start the measurements by the ICU from this moment.
� Inputs      : None
� Return      : The measured distance in Centimeter
 */
u16 Ultrasonic_readDistance(void){
	/* sending pulses by the trigger*/
	Ultrasonic_Trigger();
	_delay_ms(100);
	if(g_edges_counter == 2){ //raising & falling -> 1 cycle
		g_edges_counter=0; //reset the counter of the edges
		/* equation to convert from time to distance*/
		/* 16MHz Timer freq, sound speed =34300 cm/s */
		g_distance = (double)g_secondRead * (34300.0 / (2.0 * 16.0 * 1024.0)); // distance in centimeters
		return g_distance;
	}
	return -1;
}


/*
   Description : 1-This is the call back function called by the ICU driver.
               : 2-This is used to calculate the high time (pulse time) generated by the ultrasonic sensor.
   Inputs      : None
   Return      : None
 */

void Ultrasonic_edgeProcessing(void){
	/*increment the edge by one */
	g_edges_counter++;
	if(g_edges_counter==1){
		/*clear the timer register to start the timer when the rising edge is detected*/
		M_TIMER1_void_clearTimerValue();
		/*change the edge select to the falling edge to know the time of the pulse*/
		M_TIMER1_void_setEdgeDetectionType(FALLING);
	}
	/*this condition will be true when the falling edge is detected*/
	else if(g_edges_counter==2){
		/*we will read the input capture register and save it inside the g_timeHigh*/
		g_secondRead = M_TIMER1_void_getInputCaptureValue() + (65535 * g_OVRflow);
		/*clear the input capture register to start detect again*/
		M_TIMER1_void_clearTimerValue();
		/*To start detecting from the rising edge*/
		M_TIMER1_void_setEdgeDetectionType(RISING);
	}
}