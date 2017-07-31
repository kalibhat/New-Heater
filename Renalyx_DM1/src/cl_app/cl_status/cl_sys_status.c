/*
 * cl_sys_status.c
 *
 * Created: 1/7/2014 2:55:10 PM
 *  Author: user
 */ 
#include "cl_status_include.h"
#include "cl_status_proto.h"
#include "sv_interface_fun.h"


extern Cl_testsensorType test_sensor_status[SENSOR_MAX_INPUT];
extern float res_temp_value;

extern volatile Cl_Uint16Type switch_time1,switch_time2;
extern Cl_Uint8Type error_flag;

// ***************** following added to print on script
extern  float avgtmp2_cel ,longavgtmp2_cel;
extern  int16_t avg_duty,long_avg_duty;
//**********************************

#if 0
typedef union{
	
	Cl_Uint8Type bytearray[4] ;
	Cl_Uint16Type Twobyte  ;
	Cl_Uint32Type word ;
	
}cl_Datastreamtype;

#endif

typedef union{
	
	Cl_Uint8Type bytearray[4] ;
	Cl_Uint16Type Twobyte  ;
	Cl_Uint32Type word ;
	
}cl_Datastream_bulk_type;
Cl_ReturnCodeType 		Cl_SysStat_System_Status_Query(void);

extern Cl_Sys_statusType cl_sys_statbuffer,cl_sys_statbuffer_test;
extern Cl_BoolType test_enabled;
extern Cl_ReturnCodeType cl_memset(Cl_Uint8Type* , Cl_Uint8Type );

volatile Cl_Uint8Type flag_bp_on=0,flag_bp_off=0,flag_inc=0,flag_dec=0;
static float ps1avg=0, ps2avg = 0,ps3avg = 0,ps3avg_1sec = 0, aptavg=0,vptavg=0, ps3avg_max = 0;
static float avgtmp3   = 0 , avgtmp2   = 0, avgtmp1   = 0 , avgcond = 0,averge=0,avgcond_acid=0, avgcond_bicarb=0;
extern volatile float temprature_final_value_1,temprature_final_value_2,temprature_final_value_3;
extern volatile float pressure_final_apt,pressure_final_vpt,pressure_final_ps1,pressure_final_ps2,pressure_final_ps3;
extern volatile float cond_final_cs3;
Cl_Uint16Type valve_4_count = 0;
extern Cl_BoolType syncdone;
extern float dummy1,dummy2,dummy3,dummy4,dummy5,dummy6,dummy7;
volatile Cl_Uint8Type temp_data=1,temp_data2=1,temp_data3=1,count_dummy=0,count_dummy1=0;

extern float slope;
volatile float avgtemp2_dummy=0,avgtemp3_dummy=0,cond_dummy=0;

Cl_ReturnCodeType 		Cl_SysStat_System_Status_Query(void)
{
	Cl_ReturnCodeType RetVal;
	Sys_statusType sv_sys_statbuffer;
	int16_t temp,temp1;
	
	 RetVal = cl_memset((Cl_Uint8Type*)&sv_sys_statbuffer , sizeof(sv_sys_statbuffer)) ;
	 #ifndef PLATFORMSTUB
	 cl_sys_statbuffer.FPcurrentstatus = 1;
	 cl_sys_statbuffer.DGPstatus = 1;


	sv_status_getsystemstatus(&sv_sys_statbuffer);

	cl_sys_statbuffer.cond_wien_status = sv_sys_statbuffer.cond_wien_status;
	cl_sys_statbuffer.abdstatus = sv_sys_statbuffer.abdstatus;
	if(sv_sys_statbuffer.aptstatus > 0  && sv_sys_statbuffer.aptstatus < 65535)
	cl_sys_statbuffer.aptstatus = sv_sys_statbuffer.aptstatus;
	if(sv_sys_statbuffer.vptstatus > 0 && sv_sys_statbuffer.vptstatus < 65535)
	cl_sys_statbuffer.vptstatus = sv_sys_statbuffer.vptstatus;
	cl_sys_statbuffer.bdstatus = sv_sys_statbuffer.bdstatus;
	cl_sys_statbuffer.BP_ON_OFF = sv_sys_statbuffer.BP_ON_OFF;
	cl_sys_statbuffer.bldstatus = sv_sys_statbuffer.bldstatus;
	cl_sys_statbuffer.BPstatus = sv_sys_statbuffer.BPstatus;
	cl_sys_statbuffer.cond_status = sv_sys_statbuffer.cond_status;
	cl_sys_statbuffer.cond_status_dac = sv_sys_statbuffer.cond_status_dac;
	cl_sys_statbuffer.cond_status  = cl_sys_statbuffer.cond_status_dac; // sanjeer - for now  as hardware connection opending  - to be removed
	cl_sys_statbuffer.cond_acid = sv_sys_statbuffer.cond_acid;
	cl_sys_statbuffer.cond_bicarb = sv_sys_statbuffer.cond_bicarb;
	cl_sys_statbuffer.FPcurrentstatus = sv_sys_statbuffer.FPcurrentstatus;
	cl_sys_statbuffer.DGPstatus = sv_sys_statbuffer.DGPstatus;
	cl_sys_statbuffer.UFPstatus = sv_sys_statbuffer.UFPstatus;
	cl_sys_statbuffer.holder1status = 	sv_sys_statbuffer.holderstatus1;
	cl_sys_statbuffer.holder2status =   sv_sys_statbuffer.holderstatus2;
	cl_sys_statbuffer.bloodpumpdoor = sv_sys_statbuffer.bloodpumpdoor;
	cl_sys_statbuffer.levelswitch1 = sv_sys_statbuffer.levelswitch1;

	cl_sys_statbuffer.ps1status = sv_sys_statbuffer.ps1status;
	cl_sys_statbuffer.ps2status = sv_sys_statbuffer.ps2status;
	cl_sys_statbuffer.ps3status = sv_sys_statbuffer.ps3status;
	cl_sys_statbuffer.ps4status = sv_sys_statbuffer.ps4status;
	cl_sys_statbuffer.UFPstatus = sv_sys_statbuffer.UFPstatus;
	cl_sys_statbuffer.Temp1status = sv_sys_statbuffer.Temp1status;
	
				uint16_t cal_data  = (402 *100* sv_sys_statbuffer.Temp1status)/(2*32768); 
				//cal_data = cal_data*100;
				res_temp_lookuptable(cal_data);
				
				
	cl_sys_statbuffer.Temp2status = sv_sys_statbuffer.Temp2status;
	cl_sys_statbuffer.Temp3status = sv_sys_statbuffer.Temp3status;
	cl_sys_statbuffer.Temp4status = sv_sys_statbuffer.Temp4status;
	cl_sys_statbuffer.Flowstatus = sv_sys_statbuffer.Flowstatus;
	cl_sys_statbuffer.Heparin_full_marker = sv_sys_statbuffer.Heparin_full_marker;
	cl_sys_statbuffer.Heparin_empty_marker = sv_sys_statbuffer.Heparin_empty_marker;
	cl_sys_statbuffer.Heparin_rotation_marker = sv_sys_statbuffer.Heparin_rotation_marker;
	cl_sys_statbuffer.acid_inlet_status = sv_sys_statbuffer.acid_inlet_status;
	cl_sys_statbuffer.bicarb_inlet_status = sv_sys_statbuffer.bicarb_inlet_status;
	cl_sys_statbuffer.BP_DEC = sv_sys_statbuffer.BP_DEC;
	cl_sys_statbuffer.BP_INC = sv_sys_statbuffer.BP_INC;
	
	#else
	
	cl_sys_statbuffer.abdstatus = sv_test_sys_statbuffer.abdstatus;
	cl_sys_statbuffer.aptstatus = sv_test_sys_statbuffer.aptstatus;
	cl_sys_statbuffer.bdstatus = sv_test_sys_statbuffer.bdstatus;
	cl_sys_statbuffer.bldstatus = sv_test_sys_statbuffer.bldstatus;
	cl_sys_statbuffer.BPstatus = sv_test_sys_statbuffer.BPstatus;
	cl_sys_statbuffer.vptstatus = sv_test_sys_statbuffer.vptstatus;
	cl_sys_statbuffer.cond_status = sv_test_sys_statbuffer.cond_status;
	cl_sys_statbuffer.FPcurrentstatus = sv_test_sys_statbuffer.FPcurrentstatus;
	cl_sys_statbuffer.DGPstatus = sv_test_sys_statbuffer.DGPstatus;
	cl_sys_statbuffer.UFPstatus = sv_test_sys_statbuffer.UFPstatus;
	cl_sys_statbuffer.holder1status = 	sv_test_sys_statbuffer.holder1status;
	cl_sys_statbuffer.holder2status = sv_test_sys_statbuffer.holder2status;
	cl_sys_statbuffer.Hpstatus = sv_test_sys_statbuffer.Hpstatus;
	cl_sys_statbuffer.levelswitch1 = sv_test_sys_statbuffer.levelswitch1;
	//cl_sys_statbuffer.levelswitch2 = sv_test_sys_statbuffer.levelswitch2;
	cl_sys_statbuffer.ps1status = sv_test_sys_statbuffer.ps1status;
	cl_sys_statbuffer.ps2status = sv_test_sys_statbuffer.ps2status;
	cl_sys_statbuffer.ps3status = sv_test_sys_statbuffer.ps3status;
	cl_sys_statbuffer.ps4status = sv_test_sys_statbuffer.ps4status;
	cl_sys_statbuffer.UFPstatus = sv_test_sys_statbuffer.UFPstatus;
	cl_sys_statbuffer.Temp1status = sv_test_sys_statbuffer.Temp1status;
	cl_sys_statbuffer.Temp2status = sv_test_sys_statbuffer.Temp2status;
	cl_sys_statbuffer.Temp3status = sv_test_sys_statbuffer.Temp3status;
	cl_sys_statbuffer.Flowstatus = sv_test_sys_statbuffer.Flowstatus;
	cl_sys_statbuffer.Heparin_full_marker = sv_test_sys_statbuffer.Heparin_full_marker;
	cl_sys_statbuffer.Heparin_empty_marker = sv_test_sys_statbuffer.Heparin_empty_marker;
	cl_sys_statbuffer.acid_inlet_status = sv_test_sys_statbuffer.acid_in;
	cl_sys_statbuffer.bicarb_inlet_status = sv_test_sys_statbuffer.bicarb_in;
	cl_sys_statbuffer.BP_DEC = sv_test_sys_statbuffer.BP_DEC;
	cl_sys_statbuffer.BP_INC = sv_test_sys_statbuffer.BP_INC;
	cl_sys_statbuffer.BP_ON_OFF = sv_test_sys_statbuffer.BP_ON_OFF;
	
	#endif
	//testing
	static Cl_BoolType flag = true;
#if 0	
	if (flag)
	{
		cl_sys_statbuffer.Flowstatus = 0;
		flag =  false;
	}
	else{
		cl_sys_statbuffer.Flowstatus = 1;
		flag =  true;
	}
	#endif
//	cl_sys_statbuffer.Temp3status = 700;
	//cl_sys_statbuffer.Temp2status = 710;
	
	#if 0
	if(cl_sys_statbuffer.UFPstatus)
		{

					DD_SET_VALVE (VALVE_ID15);
		}

		else
		{
			DD_RESET_VALVE (VALVE_ID15);
		}
	
			
	#endif	
//	if(cl_sys_statbuffer.Temp2status > 3500)

	{
//		RetVal = sv_cntrl_poweroffheater();
	//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,( Cl_Uint8Type*)"TS2_?",5);
	}
//	if(cl_sys_statbuffer.Temp3status > 3500)

	{
//		RetVal = sv_cntrl_poweroffheater();
	//	Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTTEXT,( Cl_Uint8Type*)"TS3_?",5);
	}
	
#if 0
	if(cl_sys_statbuffer.Flowstatus )
	{
		sv_cntrl_activate_valve((sv_valvetype) 1  );
	}
	else
	{
		sv_cntrl_deactivate_valve((sv_valvetype)1  );
	}
	//testing
#endif
	return CL_OK;
}

Cl_ReturnCodeType 		Cl_SysStat_GetSensor_Status_Query(Cl_SensorDeviceIdType dataID, uint16_t* pdata)
{
	Cl_ReturnCodeType RetVal;
	Cl_Uint16Type tempdata=0;
	
		switch(dataID)
		{
			case SENSOR_ACID_COND:
			tempdata =  cl_sys_statbuffer.cond_acid;
			*pdata  = tempdata;
			break;
			case SENSOR_BICARB_COND:
			tempdata =  cl_sys_statbuffer.cond_bicarb;
			*pdata  = tempdata;
			break;
			case SENSOR_ACID_INLET:
			tempdata =  cl_sys_statbuffer.acid_inlet_status;
			*pdata  = tempdata;
			break;
			case SENSOR_BICARB_INLET:
			
			tempdata =  cl_sys_statbuffer.bicarb_inlet_status;
			*pdata  = tempdata;
			
			break;
			case SENSOR_BP_INC_M:
			tempdata =  cl_sys_statbuffer.BP_INC;
			*pdata  = tempdata;
			break;
			case SENSOR_BP_DEC_M:
			tempdata =  cl_sys_statbuffer.BP_DEC;
			*pdata  = tempdata;
			break;
			case SENSOR_BP_ON_OFF_M:
			tempdata =  cl_sys_statbuffer.BP_ON_OFF;
			*pdata  = tempdata;
			break;
			case SENSOR_HOLDER1STATUS:
			tempdata =  cl_sys_statbuffer.holder1status;
			*pdata  = tempdata;
			break;
			case SENSOR_HOLDER2STATUS:
			tempdata =  cl_sys_statbuffer.holder2status;
			*pdata  = tempdata;
			break;
			case SENSOR_LEVELSWITCH:
			tempdata =  cl_sys_statbuffer.levelswitch1;
			*pdata  = tempdata;
			break;

			case SENSOR_COND_STATUS:
			if(!test_sensor_status[SENSOR_COND_STATUS].test_flag)
			{
				tempdata =  cl_sys_statbuffer.cond_status;
				*pdata  = tempdata;
			}
			else
			{
				tempdata =  test_sensor_status[SENSOR_COND_STATUS].test_sensord_data;
				*pdata  = tempdata;
			}
			break;
			case  SENSOR_COND_WIEN_STATUS:
			tempdata =  cl_sys_statbuffer.cond_wien_status;
			*pdata  = tempdata;
			break;
			case SENSOR_COND_DAC:
			tempdata =  cl_sys_statbuffer.cond_status_dac;
			*pdata  = tempdata;
			break;
			case SENSOR_FPCURRENTSTATUS:
			tempdata =  cl_sys_statbuffer.FPcurrentstatus;
			*pdata  = tempdata;
			break;
			case SENSOR_UFP_FB:
			tempdata =  cl_sys_statbuffer.UFPstatus;
			*pdata  = tempdata;
			break;
			case SENSOR_ABDSTATUS:
			tempdata =  cl_sys_statbuffer.abdstatus;
			*pdata  = tempdata;
			break;
			case SENSOR_BDSTATUS:
			tempdata =  cl_sys_statbuffer.bdstatus;
			*pdata  = tempdata;
			break;
			case SENSOR_APTSTATUS:
			if(!test_sensor_status[SENSOR_APTSTATUS].test_flag)
			{
							tempdata =  cl_sys_statbuffer.aptstatus;
							*pdata  = tempdata;
			}
			else
			{
				tempdata =  test_sensor_status[SENSOR_APTSTATUS].test_sensord_data;
				*pdata  = tempdata;
			}

			break;
			case SENSOR_VPTSTATUS:
			if(!test_sensor_status[SENSOR_VPTSTATUS].test_flag)
			{
				tempdata =  cl_sys_statbuffer.vptstatus;
				*pdata  = tempdata;
			}
			else
			{
				tempdata =  test_sensor_status[SENSOR_VPTSTATUS].test_sensord_data;
				*pdata  = tempdata;
			}
			break;
			case SENSOR_BLDSTATUS:
			tempdata =  cl_sys_statbuffer.bldstatus;
			*pdata  = tempdata;
			break;
			case SENSOR_PS1STATUS:
				if(!test_sensor_status[SENSOR_PS1STATUS].test_flag)
					{
						tempdata =  cl_sys_statbuffer.ps1status;
						*pdata  = tempdata;
					}
				else
					{
						tempdata =  test_sensor_status[SENSOR_PS1STATUS].test_sensord_data;
						*pdata  = tempdata;
					}
			break;
			case SENSOR_PS2STATUS:
				if(!test_sensor_status[SENSOR_PS2STATUS].test_flag)
					{
						tempdata =  cl_sys_statbuffer.ps2status;
						*pdata  = tempdata;
					}
				else
					{
						tempdata =  test_sensor_status[SENSOR_PS2STATUS].test_sensord_data;
						*pdata  = tempdata;
					}
			break;
			case SENSOR_PS3STATUS:

				if(!test_sensor_status[SENSOR_PS3STATUS].test_flag)
				{
					tempdata =  cl_sys_statbuffer.ps3status;
					*pdata  = tempdata;
				}
				else
				{
					tempdata =  test_sensor_status[SENSOR_PS3STATUS].test_sensord_data;
					*pdata  = tempdata;
				}
			

			//tempdata =  cl_sys_statbuffer.ps3status;
			//*pdata  = tempdata;
			break;

			case SENSOR_TEMP1STATUS:
						if(!test_sensor_status[SENSOR_TEMP1STATUS].test_flag)
						{
							tempdata =  cl_sys_statbuffer.Temp1status;
							*pdata  = tempdata;
						}
						else
						{
							tempdata =  test_sensor_status[SENSOR_TEMP1STATUS].test_sensord_data;
							*pdata  = tempdata;
						}


			break;
			case SENSOR_TEMP2STATUS:
			if(!test_sensor_status[SENSOR_TEMP2STATUS].test_flag)
			{
				tempdata =  cl_sys_statbuffer.Temp2status;
				*pdata  = tempdata;
			}
			else
			{
				tempdata =  test_sensor_status[SENSOR_TEMP2STATUS].test_sensord_data;
				*pdata  = tempdata;
			}
			break;
			case SENSOR_TEMP3STATUS:
			
			if(error_flag == 0)
			{
				tempdata =  cl_sys_statbuffer.Temp3status;
				//Cl_Uint16Type data=0;
				//tempdata =  Cl_get_Temperature_Value(TS3,&data);
				*pdata  = tempdata;
			}
			else
			{
				tempdata =  400;
				*pdata  = tempdata;
			}
			break;
			case HEATER_HIGH_TEMP:
				tempdata =  cl_sys_statbuffer.Temp4status;
				*pdata  = tempdata;
			break;
			case 	SENSOR_HP_START:
				tempdata =  cl_sys_statbuffer.Heparin_full_marker;
				*pdata  = tempdata;
			break;
			case SENSOR_HP_END:
				tempdata =  cl_sys_statbuffer.Heparin_empty_marker;
				*pdata  = tempdata;
			break;
			case SENSOR_HP_FB:
				tempdata =  cl_sys_statbuffer.Heparin_rotation_marker;
				*pdata  = tempdata;
			break;
			case SENSOR_BP_FB:
				tempdata =  cl_sys_statbuffer.BPstatus;
				*pdata  = tempdata;
			break;
			case SENSOR_FLOW_SWITCH:
				tempdata =  cl_sys_statbuffer.Flowstatus;
				*pdata  = tempdata;
			break;
			case SENSOR_BLOODDOOR_STATUS:
				tempdata =  cl_sys_statbuffer.bloodpumpdoor;
				*pdata  = tempdata;
			break;
			default:
			break;
		}
	return CL_OK;
}

void Cl_SysStat_mean_status_update(void)
{
	Cl_ReturnCodeType cl_thretval = CL_ERROR;
	int16_t Ps1,Ps2,Ps3,Atp,Vtp,TMP = 0;
	int16_t sensordata=0,temp2=0;
	static int16_t prev_flow_status=0;
	int16_t Pressuredata=0;
	int16_t sensordatamillivolts =0;
	int16_t tempdata =0,count1;
	int16_t Pressuredatainmillibars =0;
	Cl_Uint8Type* pdataarray;
	cl_Datastreamtype cl_tdata;
	int16_t bulk_data[14];
	uint16_t raw_cond =0;
	static Cl_Uint8Type  flow_counter =0 ;
	static Cl_Uint16Type counter=0,threeseccounter=0,CS_ontimecnter=0,cnt = 0,levelsw_cnter = 0;
	if(syncdone)
	{
	
		if(syncdone)
		{
			Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP1STATUS, &sensordata);
			{
							
				uint16_t cal_data  = (402 *100* sensordata)/(2*32768); 
				res_temp_lookuptable(cal_data);
				avgtmp1 =	(avgtmp1*5 + res_temp_value)/6;
				uint16_t temp = avgtmp1/10;
				temp = temp - 25 +12;
				bulk_data[0] = temp;
				//bulk_data[0] = toggle_count_flow;
			}
	
			Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP2STATUS,&sensordata);
			{
					
				uint16_t cal_data  = (402 *100* sensordata)/(2*32768); 
				res_temp_lookuptable(cal_data);
				avgtmp2 =	((avgtmp2*5 + res_temp_value)/6 );
				uint16_t temp = avgtmp2/10;
			//	temp = temp -45;                                      // commented for machine 2
				temp = temp - 25 +12;
				bulk_data[1] = temp;
			}
			Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP3STATUS,&sensordata);
			{
				uint16_t cal_data  = (402 *100* sensordata)/(2*32768); 
				res_temp_lookuptable(cal_data);
				avgtmp3 =	(avgtmp3*5 + res_temp_value)/6;
				float temp = avgtmp3/10;
				temp = temp - 31 + 14;
				bulk_data[2] = temp;
			}
			Cl_SysStat_GetSensor_Status_Query(SENSOR_COND_STATUS,&raw_cond);
			{
			float	 cond1,	cond_final, cond1x100, cond_final_X10000;
	
			
			cond1 = raw_cond/100;
			cond_final = 0.0001*cond1*cond1 + 0.032*cond1 +0.91 + 0.4;
			cond1x100 = cond_final*100; ;
			cond_final_X10000= (cond1x100/(1+(avgtmp3/100  -25.0)*0.021));
		//	cond_final_X10000=  cond_final_X10000 * 2 + 100;
	//		cond_comp= (cond/(1+(cl_temp3_cel-25.0)*0.020))/10;
			avgcond = (avgcond * 5 + cond_final_X10000)/6;
			
				if(avgcond == 0)
				{
			//		avgcond = sensordata;
				}
				else
				{
			//		avgcond =cond_final * 10 ;
				}
	//			bulk_data[0] = cond1x100;
//				bulk_data[3] = avgcond;					//   chnaged on 01072017
		//		bulk_data[3] = cond1x100;                    //   chnaged on 01072017
		
				bulk_data[3] = raw_cond;
			}
		Cl_SysStat_GetSensor_Status_Query(SENSOR_ACID_COND,&sensordata);
		{
			#if 0
			float temp=0,temp1=0;
			temp = sensordata * 0.8036;
			//avgcond = (avgcond*5+temp)/6;
			//bulk_data[3] = avgcond;
			if (temp > 300)
			{
				float cond_comp=0;
				calibration_cond(temp);
				avgcond =(avgcond*5 + cond_final_cs3)/6;
				cond_comp= avgcond/(1+(avgtmp3-25.0)*0.019);
				cond_dummy=cond_comp/100;
				cl_tdata.word = 0;
				cl_tdata.Twobyte = cond_comp;
				cl_tdata.bytearray[2] = 12;
				bulk_data[3] = (cond_comp/10);
			}
			else
			{
				cl_tdata.word = 0;
				cl_tdata.Twobyte = 0;
				cl_tdata.bytearray[2] = 12;
				bulk_data[3] = 0;
				
			
				
			}
			#endif
			//	bulk_data[0] = sensordata;
			
				if(avgcond == 0)
				{
					avgcond_acid = sensordata;
				}
				else
				{
					avgcond_acid =(avgcond_acid*5 + sensordata)/6;
				}
				
			//	bulk_data[3] = avgcond;
			
			
				cl_Datastreamtype cl_tdata;
				cl_tdata.word =0;
				cl_tdata.Twobyte = avgcond_acid;
				cl_tdata.bytearray[2] = 4;
				Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
		}
		Cl_SysStat_GetSensor_Status_Query(SENSOR_BICARB_COND,&sensordata);
		{
			#if 0
			float temp=0,temp1=0;
			temp = sensordata * 0.8036;
			//avgcond = (avgcond*5+temp)/6;
			//bulk_data[3] = avgcond;
			if (temp > 300)
			{
				float cond_comp=0;
				calibration_cond(temp);
				avgcond =(avgcond*5 + cond_final_cs3)/6;
				cond_comp= avgcond/(1+(avgtmp3-25.0)*0.019);
				cond_dummy=cond_comp/100;
				cl_tdata.word = 0;
				cl_tdata.Twobyte = cond_comp;
				cl_tdata.bytearray[2] = 12;
				bulk_data[3] = (cond_comp/10);
			}
			else
			{
				cl_tdata.word = 0;
				cl_tdata.Twobyte = 0;
				cl_tdata.bytearray[2] = 12;
				bulk_data[3] = 0;
				
				
			//	bulk_data[1] = sensordata;
				
			}
			#endif
		//	bulk_data[1] = sensordata;
		
					if(avgcond == 0)
				{
					avgcond_bicarb = sensordata;
				}
				else
				{
					avgcond_bicarb =(avgcond_bicarb*5 + sensordata)/6;
				}
				
				 // remove comment after  removing heter data updates
				cl_Datastreamtype cl_tdata;
				cl_tdata.word =0;
				cl_tdata.Twobyte = avgcond_bicarb;
				cl_tdata.bytearray[2] = 6;
				Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
		}
			Cl_SysStat_GetSensor_Status_Query(SENSOR_FLOW_SWITCH,&sensordata);
			{

				cl_tdata.word = 0;
				cl_tdata.Twobyte = sensordata;
				cl_tdata.bytearray[2] = 3;
				bulk_data[4] = sensordata;
				if(prev_flow_status == sensordata)
				{
					if(flow_counter++ > 10)
					{
						//	cl_thretval = sv_cntrl_poweroffheater();
					}
					
				}
				else
				{
					flow_counter = 0;
				}
				prev_flow_status = sensordata;
				
				//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
			}

			
			//	cl_testgetsensordata(APTSTATUS_HIGH, &Atp);//0x0F
			
			Cl_SysStat_GetSensor_Status_Query(SENSOR_APTSTATUS, &Atp);
			

				calibration_apt(Atp);
				aptavg = ((aptavg* 2) + pressure_final_apt)/3;
				cl_tdata.word = 0;
				cl_tdata.Twobyte = aptavg + 1000;
				cl_tdata.bytearray[2] = 14;
				bulk_data[5] = cl_tdata.Twobyte;
								
			Cl_SysStat_GetSensor_Status_Query(SENSOR_VPTSTATUS, &Vtp);
			
			calibration_vpt(Vtp);
			vptavg = ((vptavg* 2) + pressure_final_vpt)/3;
			cl_tdata.word = 0;
			cl_tdata.Twobyte = vptavg + 1000;
			cl_tdata.bytearray[2] = 15;
			bulk_data[6] = cl_tdata.Twobyte;
	
			
			Cl_SysStat_GetSensor_Status_Query(SENSOR_PS3STATUS,&Ps3);
	
			calibration_ps3(Ps3);
			ps3avg = ((ps3avg* 2) + pressure_final_ps3)/3;
			ps3avg_1sec = ((ps3avg_1sec* 2) + Pressuredatainmillibars) /3;
			if ( ps3avg_1sec < (ps3avg ))
			{
				
				if(ps3avg_1sec < (ps3avg - 5) )
				{
					//switch chamber
				//	Cl_Alarm_TriggerAlarm(FPCURRENTSTATUS,1);
					
				}
				ps3avg_max = ps3avg;
			}
			if ( ps3avg < ps3avg_max)
			{
				

				ps3avg_max = ps3avg;
			}			
			cl_tdata.word = 0;
			cl_tdata.Twobyte = 1000 + ps3avg;
			cl_tdata.bytearray[2] = 20;
			bulk_data[7] = cl_tdata.Twobyte;
	
	//		cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
			/////////////////
			
			Cl_SysStat_GetSensor_Status_Query(SENSOR_PS2STATUS,&Ps2);
	
		calibration_ps2(Ps2);
		ps2avg = ((ps2avg* 2) + pressure_final_ps2)/3;
			

			cl_tdata.word = 0;
			cl_tdata.Twobyte = 1000 + ps2avg;
			cl_tdata.bytearray[2] = 19;
	
			bulk_data[8] = cl_tdata.Twobyte;

		//	cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
			
			
			Cl_SysStat_GetSensor_Status_Query(SENSOR_PS1STATUS,&Ps1);

			calibration_ps1(Ps1);
			ps1avg = ((ps1avg* 2) + pressure_final_ps1)/3;
			
			cl_tdata.word = 0;
			cl_tdata.Twobyte = 1000 + ps1avg;
			cl_tdata.bytearray[2] = 18;
			bulk_data[9] = cl_tdata.Twobyte;
	
			if(ps1avg > 300)
			{
				//  sv_cntrl_activate_valve(VALVE_ID4 );
				  valve_4_count++;
			}
			else
			{
				 valve_4_count++;
				if(valve_4_count >= 1)
				{
				//  sv_cntrl_deactivate_valve(VALVE_ID4 );
				  valve_4_count = 0;
				}
			}
	
		cl_thretval = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_BULK_PRINT,&bulk_data,30);
			}
		
	}
}
