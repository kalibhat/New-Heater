/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>

#include "asf.h"
#include "SENSOR_CPU.h"

extern volatile uint8_t group_id_reg_id0;
extern volatile uint8_t group_id_reg_id1;
extern volatile uint8_t group_id_reg_id2;
extern volatile uint8_t group_id_reg_id3;
extern volatile uint8_t group_id_reg_id4;
extern volatile uint8_t group_id_reg_id5;
extern volatile uint8_t group_id_reg_id6;

#define MASTER_CPU_REQUEST  group_id_reg_id0 == 1 || group_id_reg_id1 == 1 || 	group_id_reg_id2 == 1 || group_id_reg_id3 == 1 || group_id_reg_id4 == 1 || group_id_reg_id5 == 1 || group_id_reg_id6 == 1 
//extern SV_Sensor_status_type  ;
extern volatile bool master_requested_sensor_data;
extern void SV_SYSTEM_INIT(void);
extern void SV_get_sensor_data(SV_Sensor_status_type* sensor_struct);
extern bool SV_put_sensor_data(SV_Sensor_status_type* sensor_struct);
extern bool SV_put_sensor_data1(SV_Sensor_status_type* sensor_struct);

SV_Sensor_status_type volatile sensor_data, g_sensor_data;

int main (void){
	/* Insert system clock initialization code here (sysclk_init()). */
	
   
	board_init();
	SV_SYSTEM_INIT(); 
	/* Insert application code here, after the board has been initialized. */
	uint32_t count = 0;
	while(FOREVER) {	
	//for(int i=0;i<10;i++) {}
		SV_get_sensor_data(&sensor_data);
		g_sensor_data.Temp2status = (g_sensor_data.Temp2status *9 + sensor_data.Temp2status)/10;
		g_sensor_data.CS3_Tempstatus = (g_sensor_data.CS3_Tempstatus *9 + sensor_data.CS3_Tempstatus)/10;
		g_sensor_data.CS3status = (g_sensor_data.CS3status *9 + sensor_data.CS3status)/10;
		g_sensor_data.DAC1status = (g_sensor_data.DAC1status *49 + sensor_data.DAC1status/3)/50;
	//	sensor_data.CS3_Tempstatus = g_sensor_data.CS3_Tempstatus;
	//	sensor_data.CS3status = g_sensor_data.CS3status;
	//	sensor_data.Temp2status = g_sensor_data.Temp2status ;
	//	sensor_data.DAC1status = g_sensor_data.DAC1status;
		
	//	if(master_requested_sensor_data==1) 
		while(1)	
		{
			if(MASTER_CPU_REQUEST)
			{			
				

				// SV_put_sensor_data(&sensor_data);	
				 SV_put_sensor_data1(&sensor_data);		
				master_requested_sensor_data=0;
			}	
			else
			{
				break;
			}
	   
		}
	}
}
