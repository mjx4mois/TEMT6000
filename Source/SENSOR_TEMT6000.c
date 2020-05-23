/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: TEMT6000 Ambinet Light sensor
     File Name		: SENSOR_TEMT6000.c
     Function		: SENSOR_TEMT6000
     Create Date	: 2017/12/31
---------------------------------------------------------------------- */
#ifndef __TEMT6000_FUNCTION__
#define __TEMT6000_FUNCTION__  


#include <stdio.h>
#include <math.h>
#include <delay.h>

#include "SENSOR_TEMT6000.h"

#define TEMT6000_DEBUG		0

/* read adc_data[0] -> ADC channel 0 -> PA_ 2 pin */
extern unsigned int	adc_data[1];

//********************************************* SYSTEM *************************************************
/*--------------------------------------------------------------------------------------------------*/
/* read adc value & return ADC value ; note : adc -> 5V*/
void TEMT6000_GET_RAW_DATA(FLOAT *raw_data)
{
	#if TEMT6000_DEBUG	/* FOR DEBUG */
	INT32U data[3]={0};  
	#endif	

	FLOAT get_data=0.0,temp_data=0.0;
	INT32U adc_raw_data=0;

	   	/*get adc sample */
		adc_raw_data=adc_data[0];
		temp_data=(FLOAT)adc_raw_data;
		//temp_data= (FLOAT)adc_data[0];
		
		get_data =  temp_data/1023 *5 ; /* 5V*/

		*raw_data =  get_data ;

		
	#if TEMT6000_DEBUG	/* FOR DEBUG */
		data[0]= (INT32U)(get_data);
	  	data[1]= (INT32U)(get_data*10) %10;   
	  	data[2]= (INT32U)(get_data*100) %10;   	  
		printf("ADC RAW DATA = %d \r\n",adc_raw_data);	 
		printf("ML8511 RAW DATA = %d.%d%d V\r\n",data[0],data[1],data[2]);	
	#endif	
		
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	calculate the raw data turn to lx(lux)
	V= I*R , R fix 10k->10000 , I unit:uA -> 10^-6
	I = V/R -> get_ua
	So obtain V = (I * 10^-6) * 10*1000
	V /(10*1000) = (I * 10^-6)
	V * 10^6 /(10*1000) = I
	-> I = 10^6 * V /(10*1000) -> *** this formula
 	example : V = 1.1V , I= ?? uA?
 	I = 10^6 * 1.1 /(10*1000) = 110uA	
 */
void TEMT6000_GET_LX_VALUE(FLOAT raw_data,FLOAT *lx_data)
{ 
	FLOAT final_data=0;
	FLOAT get_data=0.0,temp_data=0.0;
	FLOAT get_ua=0;
	
	/* Step 1: */
	/* 1000000 -> 10^6 ; raw_data -> vlotage ; (10*1000) -> 10K Ohm */
	get_ua =  1000000* raw_data / (10*1000);	/* note :  10*1000 -> 10k ohm  ; I = V / R    ; *1000000 -> unit : uA */
	
	/* Step 2: */
	/* Fig. 3 - Photo Current vs. Illuminance */
	/* Get A point & B point to calculate m & y-y0 =m(x -x0) function */
	/* A(log10,log5.1) = (1 ,0.7)  ;  B(log1000,log500)=(3, 2.6) */
	/* m = dy/dx = (2.6-0.7)/(3-1)=(1.9/2) = 0.95 */
	/* y0=0.7 , x0=1 -> y-0.7 = 0.95*( x - 1) */

	/* for example 1*/
	/* if get_ua = 110 -> log(110) = 2.0413  -> get the formula : 2.0413-0.7 = 0.95x - 0.95 */
	/* x = (2.0413-0.7 + 0.95) / 0.95 , the x result : 2.411 */
	/* turn the x in log -> 10^2.411  = 257 lux */

	/* for example 2*/
	/* if get_ua = 200 -> log(200) = 2.3010  -> get the formula : 2.3010 -0.7 = 0.95x - 0.95 */
	/* x = (2.3010-0.7 + 0.95) / 0.95 , the x result : 2.68529 */
	/* turn the x in log -> 10^2.68529  = 484 lux */
	
	/* for example 3*/
	/* if get_ua = 20 -> log(20) = 1.3010  -> get the formula :1.3010-0.7 = 0.95x - 0.95 */
	/* x = (1.3010-0.7 + 0.95) / 0.95 , the x result : 1.6326 */
	/* turn the x in log -> 10^1.6326  = 42.9 lux */

	/* for example 4*/
	/* if get_ua = 300 -> log(300) = 2.4771 -> get the formula : 2.4771-0.7 = 0.95x - 0.95 */
	/* x = (2.4771-0.7 + 0.95) / 0.95 , the x result : 2.8706 */
	/* turn the x in log -> 10^2.8706  = 742.33 lux */

	get_data = ((FLOAT)log(get_ua) - 0.7 + 0.95) /0.95 ; /* x = (log(get_ua) -0.7 +0.95) / 0.95 */

	/* final bese 10^(get_data) */
	final_data = pow(10.0, get_data);		/* get the final lux form sepc Fig. 3 - Photo Current vs. Illuminance */

	#if TEMT6000_DEBUG	/* FOR DEBUG */
	printf("final_data= %f lx\r\n",final_data);	
	#endif		
	*lx_data =  final_data ;
}
/*--------------------------------------------------------------------------------------------------*/
//********************************************* SYSTEM *************************************************
#endif //#ifndef __TEMT6000_FUNCTION__ 
