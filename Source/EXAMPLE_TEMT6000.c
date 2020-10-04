/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: TEMT6000 Ambinet Light sensor
     File Name		: EXAMPLE_TEMT6000.c
     Function		: EXAMPLE_TEMT6000
     Create Date	: 2017/12/31
---------------------------------------------------------------------- */

#include <mega32a.h>
#include <stdio.h>
#include <delay.h>
#include <math.h>
#include "alcd.h"
#include "SENSOR_TEMT6000.h"


void EXAMPLE_TEMT6000(void);


void EXAMPLE_TEMT6000(void)
{

	FLOAT temt6000_raw_data=0.0, lx_data=0.0;

	CHAR8U lcd_char_data[5]={0};
	
		printf("-------------------- Ambinet Light sensor TEMT6000 --------------------\r\n");
   
		while(1)
		{
			/* get the raw data (Voltage) */
			TEMT6000_GET_RAW_DATA(&temt6000_raw_data);

			/*get the lx value */
			TEMT6000_GET_LX_VALUE(temt6000_raw_data,&lx_data);

			/* Print the lx result */	  
			printf("TEMT6000 lux data = %f lux(lx)\r\n",lx_data);	


			if(lx_data > 65535) /* UNSIGNED INT limit */
			{
				lx_data= 65535;
			}

			/* calculate the display data */
			lcd_char_data[0] = (CHAR8U)(lx_data/10000)%10;     
			lcd_char_data[1] = (CHAR8U)(lx_data/1000)%10;
			lcd_char_data[2] = (CHAR8U)(lx_data/100)%10;  
			lcd_char_data[3] = (CHAR8U)(lx_data/10)%10;   
			lcd_char_data[4] = (CHAR8U)(lx_data)%10;   	
 

			/* show data */
			lcd_gotoxy(0,0);
			lcd_putsf("LX:");
					  
			lcd_putchar(48+lcd_char_data[0]);
			lcd_putchar(48+lcd_char_data[1]);
			lcd_putchar(48+lcd_char_data[2]); 
			lcd_putchar(48+lcd_char_data[3]);
			lcd_putchar(48+lcd_char_data[4]); 						

			/* show unit */
			lcd_gotoxy(5,1);
			lcd_putsf("lx(lux)");	/*unit: lx(lux) */
			
			delay_ms(250);/* delay */
			
		}

		printf("--------------------  Ambinet Light sensor TEMT6000 --------------------\r\n");
		
}
