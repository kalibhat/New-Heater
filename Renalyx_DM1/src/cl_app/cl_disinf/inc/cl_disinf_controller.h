/*
 * cl_dinf_controller.h
 *
 * Created: 12/27/2013 12:27:45 PM
 *  Author: user
 */ 


#ifndef CL_DINF_CONTROLLER_H_
#define CL_DINF_CONTROLLER_H_


typedef enum
{
	EVENT_NULL_EVENT,
	EVENT_START_DISINF,
	EVT_RESUME_DISINF_STAGE,
	EVENT_STOP_DISINF,
	EVENT_DISINF_TIMEOUT,
	EVENT_DISINF_OPENFILL_TIMEOUT,
	EVENT_DISINF_V2OPEN_TIMEOUT,
	EVENT_DISINF_BO1_V4_TIMEOUT,
	EVENT_DISINF_BO2_V4_TIMEOUT,
	EVENT_DISINF_BO1_V13V14_TIMEOUT,
	EVENT_DISINF_BO2_V13V14_TIMEOUT,
	EVENT_DISINF_DIALYSIS_PREP,
	EVENT_DISINF_TICK_50MS,
	EVENT_DISINF_TICK_100MS,
	EVENT_DISINF_TICK_500MS,
	EVENT_DISINF_TICK_SECOND,
	EVENT_DISINF_TICK_MINUTE,
	EVENT_DISINF_TICK_HOUR,
	EVENT_DISINF_ALERT,
	EVENT_DISINF_ALARM,
	EVT_DISINF_GET_DATA,
	EVT_DISINF_SET_DATA,
	EVENT_MAC_DISINF_COMPLETED,
	EVT_DISINF_START_PRIME,
	EVT_DISINF_STOP_PRIME,
	EVENT_DISINF_SET_BLDPMP_ON,
	EVENT_DISINF_SET_BLDPMP_OFF,
	EVENT_DISINF_SET_BLDPUMPRATE,
	EVT_DISINF_RCIRC_START,
	EVT_DISINF_RCIRC_STOP,
	EVT_DISINF_COMMAND_SET_BLDPMP_ON,
	EVT_DISINF_COMMAND_SET_BLDPMP_OFF,
	EVT_DISINF_COMMAND_SET_BLDPUMPRATE,
	EVENT_DISINF_BC_OPENFILL_COMPLETED,
	EVENT_PRE_DISINFECT_RINSE_COMPLETED,
	EVENT_POST_DISINFECT_RINSE_COMPLETED,
	EVENT_DISINF_START_RINSE,
	EVENT_DISINF_CLR_ALARM,
	EVENT_DISINF_MAXEVENT
}Cl_disinf_Eevents;


typedef enum {
	
	CL_DISINF_ALARM_ACIDIN,
	CL_DISINF_ALARM_BICARBIN,
	CL_DISINF_ALARM_HOLDER1STATUS_OPEN,
	CL_DISINF_ALARM_HOLDER2STATUS_OPEN,
	CL_DISINF_ALARM_TEMP3_HIGH_THRESHOLD,
	CL_DISINF_ALARM_PS1_HIGH_THRESHOLD,
	CL_DISINF_ALARM_PS1_LOW_THRESHOLD,
	CL_DISINF_ALARM_PS2_HIGH_THRESHOLD,
	CL_DISINF_ALARM_PS2_LOW_THRESHOLD,
	CL_DISINF_ALARM_PS3_HIGH_THRESHOLD,
	CL_DISINF_ALARM_PS3_LOW_THRESHOLD,
	CL_DISINF_ALARM_COND_STATUS_HIGH,
	CL_DISINF_ALRM_MAX
	
} CldisinfAlarmIdType;

typedef enum
{
	CL_DISINF_STATE_IDLE,
	CL_DISINF_STATE_INIT,
	CL_DISINF_STATE_DISINF_PREPARATION,
	CL_DISINF_STATE_FIRST_RO_RINSE,
	CL_DISINF_STATE_FIRST_RO_RINSCOMPLETED,
	CL_DISINF_STATE_DISINF_OPENFILL,
	CL_DISINF_STATE_DISINF_INTAKE,
	CL_DISINF_STATE_DISINF_INTAKE_COMPLETED,
	CL_DISINF_STATE_DISINF_RINSING,
	CL_DISINF_STATE_DISINF_RINSING_COMPLETED,
	CL_DISINF_STATE_SECOND_RO_RINSE,
	CL_DISINF_STATE_SECOND_RO_RINSE_COMPLETED,
	CL_DISINF_STATE_CRITICAL_ALARM,
	CL_DISINF_STATE_PAUSED,
	CL_DISINF_STATE_STOPPED,
	CL_DISINF_STATE_POST_DISINF_STANDBY,
	CL_DISINF_STATE_INTER,
	CL_DISINF_STATE_INTER_1,
	CL_DISINF_STATE_MAXSTATE
} Cl_disinf_StatesType;

typedef enum
{
	CL_DISINF_RINSING_IDLE,
	CL_DISINF_RINSING_OPENFILL,
	CL_DISINF_RINSING_BC_SWITCHING,
	CL_DISINF_RINSING_MAX_STATE
	
	
	
}Cl_disinf_SubStatesType;

typedef union{
	
	Cl_Uint8Type bytearray[8] ;
	Cl_Uint16Type Twobyte  ;
	Cl_Uint32Type word ;
	
}cl_DlsInfDatatype;

typedef struct
{
	
	Cl_NewAlarmIdType CldisinfAlarmId;
	Cl_AlarmType Cl_RinseAlarmType;
	Cl_BoolType IsActive;
	Cl_BoolType IsRaised;
	Cl_BoolType userclear;
	Cl_BoolType critical;
	Cl_BoolType Mute;
	
}DisinfAlarmsType;


#define CL_DISINF_TIMEOUT  15 // min
#endif /* CL_DINF_CONTROLLER_H_ */