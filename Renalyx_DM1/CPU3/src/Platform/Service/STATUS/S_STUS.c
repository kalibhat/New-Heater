/*
 * S_STUS.c
 *
 * Created: 12/11/2014 11:08:47 AM
 *  Author: wid7
 */ 
#include "S_STUS.h"

uint16_t cpu3_sv_status_getsystemstatus(Sys_statusType* sensor_struct)
{
	
	Sys_statusType sensor_struct1;

	sensor_struct1.cond1_status_voltage			= CPU3_D_READ_SENSORS(SV_SENSOR_ID1);
	sensor_struct1.cond1_status_current			= CPU3_D_READ_SENSORS(SV_SENSOR_ID2);
	sensor_struct1.cond2_status_voltage			= CPU3_D_READ_SENSORS(SV_SENSOR_ID3);
	sensor_struct1.cond2_status_current			= CPU3_D_READ_SENSORS(SV_SENSOR_ID4);
	sensor_struct1.Tempstatus					= CPU3_D_READ_SENSORS(SV_SENSOR_ID5);
	
	sensor_struct1.rudncy_irq					= CPU3_D_RD_SENSIN(SV_SENSIN_ID7);
	
	sensor_struct->cond1_status_voltage			= sensor_struct1.cond1_status_voltage;
	sensor_struct->cond1_status_current			= sensor_struct1.cond1_status_current;
	sensor_struct->cond2_status_voltage			= sensor_struct1.cond2_status_voltage;
	sensor_struct->cond2_status_current			= sensor_struct1.cond2_status_current;
	sensor_struct->Tempstatus			= sensor_struct1.Tempstatus;	
	
	sensor_struct->rudncy_irq			= sensor_struct1.rudncy_irq;
	
	return 0;

}