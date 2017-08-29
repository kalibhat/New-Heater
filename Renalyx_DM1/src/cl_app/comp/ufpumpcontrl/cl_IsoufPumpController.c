/*
 * cl_isoufPumpController.c
 *
 * Created: 7/12/2017 2:53:53 PM
 *  Author: Manav
 */ 

/*
 * UFPumpController.c
 *
 * Created: 11/5/2014 3:36:33 PM
 *  Author: user
 */ 

#include "cl_ufPumpController_include.h"
#include "cl_mac_include.h"
#include "calibration.h"
#include "cl_dlsiscontroller_include.h"
#include "cl_ufPumpController_include.h"
#include "sv_interface_fun.h"


cl_isouf_states cl_isouf_state = CL_ISOUF_STATE_IDLE;



Cl_ReturnCodeType cl_isouf_retcode = CL_OK;
static Cl_Uint32Type cl_uf_rate = 0;
extern Cl_Uint32Type Treatdata[ID_MAX_TREAT_PARAM] ;
extern Cl_Mac_StatesType Cl_MacState;
extern volatile Cl_Uint32Type uf_final;
//extern Cl_ReturnCodeType cl_ufpumpFeedback_start(void);
extern Cl_ReturnCodeType  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , Cl_Uint8Type* ,Cl_Uint8Type );
//extern Cl_ReturnCodeType cl_ufpumpFeedback_set_expected_period(int16_t period);
extern Cl_ConsoleMsgType Cl_ConsoleRxMsg;
extern float res_temp_value;
extern volatile float pressure_final_apt,pressure_final_vpt,pressure_final_ps1,pressure_final_ps2,pressure_final_ps3;
Cl_ReturnCodeType cl_dialysate_ready_isouf();

int Cl_ISOufTotalSecondscounter=0 ;

bool isouf = false;

Cl_ReturnCodeType cl_isouf_controller(Cl_Mac_EventsType Cl_MacDlsisEvent)
{
	
//	cl_ufpump_states uf_pumpstate = CL_UFP_STATE_IDLE;
	Cl_Uint16Type uf_delta_value = 0 ;
	static Cl_Uint16Type pwm_period = 0;
	Cl_Uint32Type temp = 0;
	Cl_Uint32Type temptime = 0;
	
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	Cl_Uint8Type data = 0;
	Cl_Uint8Type datasize = 0;
	Cl_Uint8Type dataarray[4] =  {0,0,0,0};
	
	cl_isouf_events cl_isouf_event = EVENT_ISOUF_NULL_EVENT;
	
	cl_isouf_retcode =  cl_isouf_translatemacevent( Cl_MacDlsisEvent, &cl_isouf_event);
	
	switch(cl_isouf_state)
	{
			case CL_ISOUF_STATE_IDLE:
			switch (cl_isouf_event)
				
				{
					case EVENT_ISOUF_START:
					
					Cl_Isouf_start();
				
					cl_isouf_state = CL_ISOUF_STATE_STARTED;
					break;
					default: break;
				}
				break;
				
					
			case CL_ISOUF_STATE_STARTED:
			
			switch(cl_isouf_event){
				
// 				case  EVENT_ISOUF_STOP :
// 				cl_uf_controller(CL_UF_EVENT_STOP,0);
// 				cl_hep_controller(CL_HEP_EVENT_STOP_NORMAL_DELIVERY,0);
// 				break;
				
				case EVENT_ISOUF_TICK_500MS :
				
					cl_isouf_retcode == cl_checkforblooddetection();
				
				break;
				
				case EVENT_ISOUF_TICK_SECOND :
				
					Cl_ISOufTotalSecondscounter ++ ;
				
					if(cl_isouf_retcode == CL_OK){
					
					// check for completion
					//cl_sendisoufdata();
						Cl_Iso_Senddata();
						if(Cl_ISOufTotalSecondscounter >= Treatdata[ID_treattime]*60){
						
						// complete
						cl_isouf_retcode == cl_isoufcompleted();
						if(cl_isouf_retcode == CL_OK){
							
												
							cl_isouf_state = CL_ISOUF_STATE_COMPLETED;
							isouf = false;
							command = CON_TX_COMMAND_COMMAND_ISOUF_COMPLETED;
							data = (Cl_Uint8Type)COMMAND_RESULT_SUCCESS;
							cl_isouf_retcode = Cl_SendDatatoconsole(command,&data,1);
							cl_isouf_retcode = Cl_mac_apprequesthandler(MACREQ_ISOUF_COMPLETED);
							
							
						}
					}
					
					// check for alarms
				}	
				else{
					// raise alarm
				}
				
				break;
				
				
				case EVENT_ISOUF_STOP :
				
					Cl_Isouf_stop();
				// stop uf pump
					isouf = false;
					cl_isouf_state = CL_ISOUF_STATE_STOPPED;
				
				break;
				
				// implement iso uf stop state
				// implement bp on event
				// implement bp off event  , iso uf should stop
				
				
				default: break;
				
			}
			
			break;
			
			case CL_ISOUF_STATE_STOPPED:
			
				switch(cl_isouf_event){
					
					case EVENT_ISOUF_START:
				
				//Cl_ISOufTotalSecondscounter = 0;
					cl_dialysate_ready_isouf();
				
					cl_isouf_state = CL_ISOUF_STATE_STARTED;
				break;
					
				default: break;	
					
					
				}
			
			break;
			
			case CL_ISOUF_STATE_COMPLETED:
			switch(cl_isouf_event){
				
				case EVENT_ISOUF_TICK_SECOND :
				
				break;
				
				case EVENT_ISOUF_START:
				
				//Cl_ISOufTotalSecondscounter = 0;
					cl_dialysate_ready_isouf();
				
					cl_isouf_state = CL_ISOUF_STATE_STARTED;
				break;
			
			default : break;	
			
			}
			break;
			
			case CL_ISOUF_STATE_POST_COMPLETION:
			
			switch(cl_isouf_event){
				
				case EVENT_ISOUF_TICK_SECOND :
				
				break;
				
				case EVENT_ISOUF_START_RETURN:
				
				break;
				
				
				
				default:break;
			}
			
			break;
			
			default:break;
			
			
	}
	return CL_OK;
	
	
}

Cl_ReturnCodeType Cl_Isouf_stop(){
	
	Cl_Uint8Type data;
	
	cl_uf_controller(CL_UF_EVENT_STOP,0);
	
	cl_isouf_retcode = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_ISOUF_STOPPED,&data,0);
	
}

Cl_ReturnCodeType Cl_Isouf_start(){
	
	Cl_Uint8Type data;
	
	Cl_ISOufTotalSecondscounter = 0;
	//	cl_dialysate_ready_isouf();
	sv_cntrl_deactivate_valve(VALVE_ID13);
	sv_cntrl_activate_valve(VALVE_ID14);
	sv_cntrl_activate_valve(VALVE_ID15);
	cl_hep_controller(CL_HEP_EVENT_START_NORMAL_DELIVERY,0);
	cl_uf_controller(CL_UF_EVENT_START,0);
	
	isouf = true;
	//cl_sendisoufdata();

	cl_isouf_retcode = Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_ISOUF_STARTED,&data,0);
	
		Cl_Iso_UpdateTimeInfo();
}

Cl_ReturnCodeType Cl_Iso_Senddata(){
	
	static float cond = 0;
	Cl_Uint8Type systemdataarray[40] =  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	cl_DlsisDatatype tempdata;
	Cl_Uint16Type temp=0, temp1=0;
	static float avgtmp3=0,apt=0,vpt=0,ps1=0,ps2=0;
	static Cl_Uint32Type timecount = 0;
	Cl_ReturnCodeType Cl_dlsisretcode = CL_OK;
	Cl_Uint8Type count = 0;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	Cl_Uint16Type sensordatamillivolts;
	command = CON_TX_COMMAND_CL_SYS_STATE_DATA ;
	systemdataarray[0] = ISOUF_DATA;
	count++;
/*	
	COND, // eg 138
	TEMP3, // eg 370
	TEMP3,
	APT,
	VPT,
	TMP,
	UF_REMOVED
	*/
		Cl_SysStat_GetSensor_Status_Query(SENSOR_TEMP3STATUS,&temp);
		{
//							float sensordata,ftemp1,temp1;
// 							sensordata = temp;
// 							uint16_t cal_data  = (402 *100* sensordata)/(2*32768);
// 							res_temp_lookuptable(cal_data);
// 							//	tmp3 =	(tmp3*5 + res_temp_value)/6;
// //							temp1 = res_temp_value/10;
// //							temp1 = temp1 - 31 -20;
// //							avgtmp3 =(avgtmp3*5 + temp1)/6;
// 							avgtmp3 = (res_temp_value  - 0.4)/10;
							avgtmp3 = (float)temp/10;
		}
		Cl_SysStat_GetSensor_Status_Query(SENSOR_COND_STATUS,&temp);
		{
			float cond_comp;
				float temp2;
				
				if (temp > 50)
			{
				//	calibration_cond(temp);
					cond =  -0.0001 * temp* temp  + 0.36 * temp  - 38.39 ;
				//	cond =(cond*5 + cond_final_cs3)/6;
					cond_comp= cond/(1+(avgtmp3/10-25.0)*0.020);
				
					
					tempdata.word = (cond_comp);
					systemdataarray[count++] = tempdata.bytearray[0];
					systemdataarray[count++] = tempdata.bytearray[1];
					systemdataarray[count++] = tempdata.bytearray[2];
					systemdataarray[count++] = tempdata.bytearray[3];
			}
				else
				{
					tempdata.word = 0;
			systemdataarray[count++] = tempdata.bytearray[0];
			systemdataarray[count++] = tempdata.bytearray[1];
			systemdataarray[count++] = tempdata.bytearray[2];
			systemdataarray[count++] = tempdata.bytearray[3];
		}
					
		}
		{
			
		//	float ftemp,ftemp1;
		//	ftemp = temp * 0.803;
		//	calibration_tmp(ftemp,TS3);
		//	avgtmp3 =(avgtmp3*5 + temprature_final_value_3)/6;
			tempdata.word = (avgtmp3);
			systemdataarray[count++] = tempdata.bytearray[0];
			systemdataarray[count++] = tempdata.bytearray[1];
			systemdataarray[count++] = tempdata.bytearray[2];
			systemdataarray[count++] = tempdata.bytearray[3];
		}
		Cl_SysStat_GetSensor_Status_Query(SENSOR_APTSTATUS,&tempdata.Twobyte);
		{
			
			sensordatamillivolts = (tempdata.Twobyte * 0.8036) ;
			calibration_apt(sensordatamillivolts);
			apt =(apt*5 + pressure_final_apt)/6;
			tempdata.word	= apt;
			systemdataarray[count++] = tempdata.bytearray[0] ;
			systemdataarray[count++] = tempdata.bytearray[1] ;
			systemdataarray[count++] = tempdata.bytearray[2] ;
			systemdataarray[count++] = tempdata.bytearray[3] ;
		}
		Cl_SysStat_GetSensor_Status_Query(SENSOR_VPTSTATUS,&tempdata.Twobyte);
		{
			sensordatamillivolts = (tempdata.Twobyte * 0.8036) ;
			calibration_vpt(sensordatamillivolts);
			vpt =(vpt*5 + pressure_final_vpt)/6;
			tempdata.word	 = vpt;
			systemdataarray[count++] = tempdata.bytearray[0] ;
			systemdataarray[count++] = tempdata.bytearray[1] ;
			systemdataarray[count++] = tempdata.bytearray[2] ;
			systemdataarray[count++] = tempdata.bytearray[3] ;
		}
			Cl_SysStat_GetSensor_Status_Query(SENSOR_PS1STATUS,&tempdata.Twobyte);
			sensordatamillivolts = (tempdata.Twobyte * 0.8036) ;
			calibration_ps1(sensordatamillivolts);
			ps1=(ps1*5 + pressure_final_ps1)/6;
	
			Cl_SysStat_GetSensor_Status_Query(SENSOR_PS2STATUS,&tempdata.Twobyte);
			sensordatamillivolts = (tempdata.Twobyte * 0.8036) ;
			calibration_ps2(sensordatamillivolts);
			ps2=(ps2*5 + pressure_final_ps2)/6;
			
			tempdata.word	 = (((apt + vpt) - (ps1+ps2))/2)-25;


			systemdataarray[count++] = tempdata.bytearray[0] ;
			systemdataarray[count++] = tempdata.bytearray[1] ;
			systemdataarray[count++] = tempdata.bytearray[2] ;
			systemdataarray[count++] = tempdata.bytearray[3] ;
			
			systemdataarray[count++] = 0x0A; // tmp
			systemdataarray[count++] = 0x0A; // tmp
			systemdataarray[count++] = 0x00;
			systemdataarray[count++] = 0x00;
			cl_isouf_retcode = Cl_SendDatatoconsole(CON_TX_COMMAND_CL_SYS_STATE_DATA,&systemdataarray,count);
	//Cl_dlsisretcode = Cl_SendDatatoconsole(command,&systemdataarray,count);
	
	return CL_OK;
	
	
}

Cl_ReturnCodeType Cl_Iso_UpdateTimeInfo(void)
{
	
	Cl_ReturnCodeType Cl_rinseretcode = CL_ERROR;
	Cl_ConsoleTxCommandtype command = CON_TX_COMMAND_COMMAND_MAX;
	Cl_Uint8Type data[7] = {0,0,0,0,0 ,0,0};
	
	command = CON_TX_COMMAND_REM_TIME;

	
	data[0]= (Cl_Uint8Type)ISOUF_DATA;
	data[1]= (Cl_Uint8Type)1;

	
	Cl_rinseretcode = Cl_SendDatatoconsole(command,&data,2);
	return CL_OK;
	
}

Cl_ReturnCodeType cl_checkforblooddetection(){
	
	Cl_Uint16Type bldtemp = 0;
	
	Cl_SysStat_GetSensor_Status_Query(SENSOR_BDSTATUS , &bldtemp);
	
	if(bldtemp == 1){
		return CL_OK;
	}
	else{
		return CL_OK; // should be CL_REJECTED
	}
	
	
}

Cl_ReturnCodeType  cl_isouf_translatemacevent(Cl_Mac_EventsType Cl_MacISOufEvt,cl_isouf_events* cl_isouf_event)
{
	switch(Cl_MacISOufEvt)
	{
		
		case EVT_CONSOLE_COMMAND_ISOUF_START :
		*cl_isouf_event = EVENT_ISOUF_START;
		break;
		case EVT_CONSOLE_COMMAND_ISOUF_STOP :
		*cl_isouf_event = EVENT_ISOUF_STOP;
		break;
		
		case EVT_CONSOLE_COMMAND_CLR_ALARM:
		*cl_isouf_event = EVENT_ISOUF_CLR_ALARM;
		break;
		case 	EVT_CONSOLE_COMMAND_SET_BLDPUMPRATE:
		*cl_isouf_event = EVENT_ISOUF_COMMAND_SET_BLDPUMPRATE;
		break;
		case EVT_CONSOLE_COMMAND_SET_BLDPMP_ON:
		*cl_isouf_event = EVENT_ISOUF_COMMAND_SET_BLDPMP_ON;
		break;
		case EVT_CONSOLE_COMMAND_SET_BLDPMP_OFF:
		*cl_isouf_event = EVENT_ISOUF_COMMAND_SET_BLDPMP_OFF;
		break;
		
		case EVT_CONSOLE_COMMAND_GET_DATA:
		*cl_isouf_event = EVENT_ISOUF_GET_DATA;
		break;
		case EVT_CONSOLE_COMMAND_SET_DATA:
		*cl_isouf_event = EVENT_ISOUF_SET_DATA;
		break;
		case EVT_CONSOLE_COMMAND_RINSE_START:
		*cl_isouf_event= EVENT_ISOUF_START_RINSE;
		break;
		case  EVT_CONSOLE_COMMAND_DIALYSIS_START:
		*cl_isouf_event= EVENT_ISOUF_START_DIALYSIS;
		break;
		case  EVT_TIMER_EXPIRED:
		
		break;
		case EVT_TICK_50M:
		*cl_isouf_event =   EVENT_ISOUF_TICK_50MS;
		break;
		case EVT_TICK_500M:
		*cl_isouf_event = EVENT_ISOUF_TICK_500MS;
		break;
		case EVT_TICK_SEC:
		*cl_isouf_event =   EVENT_ISOUF_TICK_SECOND;
		break;
		case EVT_TICK_MIN:
		*cl_isouf_event =   EVENT_ISOUF_TICK_MINUTE;
		break;
		case EVT_TICK_HOUR:
		*cl_isouf_event =   EVENT_ISOUF_TICK_HOUR;
		break;
		case EVT_ALARM_TRIGGERED:
		*cl_isouf_event =   EVENT_ISOUF_ALARM;
		break;
		case EVT_ALERT_TRIGGERED:
		*cl_isouf_event =   EVENT_ISOUF_ALERT;
		break;
		
		case EVT_CONSOLE_COMMAND_START_RETURN:
		*cl_isouf_event =   EVENT_ISOUF_STOP_RETURN;
		break;
		case EVT_CONSOLE_COMMAND_STOP_RETURN:
		*cl_isouf_event =   EVENT_ISOUF_STOP_RETURN;
		break;
		
		case EVT_CONSOLE_COMMAND_HEP_PMP_RATE:
		*cl_isouf_event =   EVENT_ISOUF_HEP_RATE;
		break;
		case EVT_CONSOLE_COMMAND_HEPARIN_START:
		*cl_isouf_event =   EVENT_ISOUF_HEP_ON;
		break;
		case EVT_CONSOLE_COMMAND_HEPARIN_STOP:
		*cl_isouf_event =   EVENT_ISOUF_HEP_OFF;
		
		break;
		
		case MACREQ_ISOUF_COMPLETED:
		*cl_isouf_event =   CL_ISOUF_STATE_POST_COMPLETION;
		break;
		
		
		default:
		break;
	}
	return CL_OK;
}


Cl_ReturnCodeType cl_isoufcompleted(){
	
	// stop uf pump
	cl_uf_controller(CL_UF_EVENT_STOP,0);
	
	return CL_OK;
}

Cl_ReturnCodeType cl_dialysate_ready_isouf(){

						sv_cntrl_deactivatepump(DCMOTOR1 );
						sv_cntrl_deactivatepump(DCMOTOR2 );
						Cl_bc_controller(BC_EVENT_STOP);
						
						
						
						sv_cntrl_poweroffheater();
						sv_cntrl_deactivate_valve(VALVE_ID1);
						cl_flow_switchcontroller(FLOW_SWITCH_CNTRLER_EVENT_STOP);

						sv_cntrl_deactivate_valve(VALVE_ID1);
						sv_cntrl_activate_valve(VALVE_ID18);
						cl_hep_controller(CL_HEP_EVENT_START_NORMAL_DELIVERY,0);
						cl_uf_controller(CL_UF_EVENT_START,0);
						
						Cl_Isouf_start();


return CL_OK;

}

 Cl_ReturnCodeType Cl_isouf_init(void)
 {
	 
	 cl_isouf_state = CL_ISOUF_STATE_IDLE;

	 //cl_isouf_event = EVENT_NULL_EVENT;
	 
	 Cl_ISOufTotalSecondscounter=0 ;
	 return CL_OK;
 }

