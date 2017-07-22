/*
 * cl_dprep_controller.h
 *
 * Created: 12/27/2013 12:25:00 PM
 *  Author: user
 */ 


#ifndef CL_DPREP_CONTROLLER_H_
#define CL_DPREP_CONTROLLER_H_
#include "cl_alarmdetector.h"

#define CL_ALARM_TRIGGERED 1
#define CL_ALARM_CLEARED 0
typedef enum
{
		EVENT_DPREP_EVENT_NULL,
		EVENT_DPREP_CLR_ALARM,
		EVENT_DPREP_DIALYSIS_PREP,
		EVENT_DPREP_COMMAND_SET_DATA,
		EVENT_DPREP_COMMAND_GET_DATA,
		EVENT_DPREP_COMMAND_SET_BLDPMP_ON,
		EVENT_DPREP_COMMAND_SET_BLDPMP_OFF,
		EVENT_DPREP_COMMAND_SET_BLDPUMPRATE,
		EVENT_DPREP_START_DIALYSATE_FILL,
		EVENT_DPREP_STOP_DIALYSATE_FILL,
		EVENT_DPREP_START_PRIME,
		EVENT_DPREP_STOP_PRIME,
		EVENT_DPREP_DILYSER_CONNECTED,
		EVENT_DPREP_START_DIALISER_PRIME,
		EVENT_DPREP_STOP_DIALYSER_PRIME,
		EVENT_DPREP_HEP_PMP_RATE,
		EVENT_DPREP_MAC_PRIME_COMPLETED,
		EVENT_DPREP_MAC_DIALYSER_PRIME_COMPLETED,
		EVENT_DPREP_MAN_PREP_COMPLETED,
		EVENT_DPREP_TICK_50MS,
		EVENT_DPREP_TICK_100MS,
		EVENT_DPREP_TICK_500MS,
		EVENT_DPREP_TICK_SECOND,
		EVENT_DPREP_TICK_MINUTE,
		EVENT_DPREP_TICK_HOUR,
		EVENT_DPREP_ALARM,
		EVENT_DPREP_ALERT,
		EVENT_DPREP_PATIENT_CONNECTED,
		EVENT_DPREP_PATIENT_READY,
		EVENT_DPREP_MIXING_PREP_START,
		EVENT_DPREP_MIXING_PREP_STOP,
		EVENT_DPREP_START_RECIRC,
		EVENT_DPREP_STOP_RECIRC,
		EVENT_HEAPARIN_BOLUS,
		EVENT_DPREP_HEP_PMP_START,
		EVENT_DPREP_HEP_PMP_STOP,
		EVENT_DPREP_MAXEVENT
	
}
Cl_Dprep_Events;

typedef enum {
CL_DPREP_ALARM_BLOODDOOR_STATUS_OPEN,
CL_DPREP_ALARM_HOLDER1STATUS_CLOSED,
CL_DPREP_ALARM_HOLDER2STATUS_CLOSED,
CL_DPREP_ALARM_HOLDER1STATUS_OPEN,
CL_DPREP_ALARM_HOLDER2STATUS_OPEN,
CL_DPREP_ALARM_ACID_IN,
CL_DPREP_ALARM_BICAR_IN,
CL_DPREP_ALARM_COND_STATUS_LOW,
CL_DPREP_ALARM_COND_STATUS_HIGH,
CL_DPREP_ALARM_COND_DAC_OPEN,
CL_DPREP_ALARM_COND_DAC_RO,
CL_DPREP_ALARM_COND_DAC_HIGH,
CL_DPREP_ALARM_FLOW_NO_FLOW,
CL_DPREP_ALARM_FLOW_LOW_FLOWRATE,
CL_DPREP_ALARM_ABD_EVENT,
CL_DPREP_ALARM_BD_EVENT,
CL_DPREP_ALARM_BLD_EVENT,
CL_DPREP_ALARM_APTSTATUS_HIGH,
CL_DPREP_ALARM_VPTSTATUS_HIGH,
CL_DPREP_ALARM_PS1_HIGH_THRESHOLD,
CL_DPREP_ALARM_PS1_LOW_THRESHOLD,
CL_DPREP_ALARM_PS2_HIGH_THRESHOLD,
CL_DPREP_ALARM_PS2_LOW_THRESHOLD,
CL_DPREP_ALARM_PS3_HIGH_THRESHOLD,
CL_DPREP_ALARM_PS3_LOW_THRESHOLD,
CL_DPREP_ALARM_TEMP1_HIGH_THRESHOLD,
CL_DPREP_ALARM_TEMP1_LOW_THRESHOLD,
CL_DPREP_ALARM_TEMP2_HIGH_THRESHOLD,
CL_DPREP_ALARM_TEMP2_LOW_THRESHOLD,
CL_DPREP_ALARM_TEMP3_HIGH_THRESHOLD,
CL_DPREP_ALARM_TEMP3_LOW_THRESHOLD,
CL_DPREP_ALARM_FPCURRENTSTATUS,               // added on 18.03.17
CL_DPREP_ALARM_CONSOLE_SYNC_LOST,           // added on 18.03.17
CL_DPREP_ALARM_HEATER_UNSTABLE,              // added on 18.03.17
CL_DPREP_ALRM_MAX
	
} ClDprepAlarmIdType;


typedef enum
{
	CL_DPREP_STATE_IDLE,
	CL_DPREP_STATE_INIT,
	CL_DPREP_STATE_DPREP_FILLING,
	CL_DPREP_STATE_DPREP_FILLING_DONE,
	CL_DPREP_STATE_DPREP_FILLING_PRIME_DONE,
	CL_DPREP_STATE_POST_PRIME_STANDBY,
	CL_DPREP_STATE_DIALISER_PRIME,
	CL_DPREP_STATE_DIALISER_PRIME_STOPPED,
	CL_DPREP_STATE_POST_DPREP_STANDBY,
	CL_DPREP_STATE_READY_FOR_DALYSIS,
	CL_DPREP_STATE_PAUSED,
	CL_DPREP_STATE_STOPPED,
	CL_DPREP_STATE_CRITICAL_ALARM,
	CL_DPREP_STATE_DREP_PRIME_ALARM_BYPASS,
	CL_DPREP_STATE_MAXSTATE
} Cl_Dprep_States;
typedef enum
{
	CL_DPREP_BYPASS_NONE,
	CL_DPREP_BYPASS_ON,
	CL_DPREP_BYPASS_OFF
}Cl_Dprep_BypassStates;

typedef enum
{
	CL_DPREP_PATIENT_STATE_NOT_CONNECTED,
	CL_DPREP_PATIENT_STATE_WAITING_FOR_BD,
	CL_DPREP_PATIENT_STATE_BLOOD_DETECTED,
	CL_DPREP_PATIENT_STATE_DIALYIS,
	
}Cl_PatientStateType;
	
	typedef enum
	{
		CL_DPREP_FILLING_IDLE,
		CL_DPREP_STATE_OPENFILL_TRANSITION,
		CL_DPREP_STATE_OPENFILL,
		CL_DPREP_STATE_V15OPEN_TRANSITION,
		CL_DPREP_STATE_V15OPEN,
		CL_DPREP_STATE_V2OPEN_TRANSITION,
		CL_DPREP_STATE_V2OPEN,
		CL_DPREP_STATE_BO1_V4_TRANSITION,
		CL_DPREP_STATE_BO1_V4,
		CL_DPREP_STATE_BO2_V4_TRANSITION,
		CL_DPREP_STATE_BO2_V4,
		CL_DPREP_STATE_BO1_V13V14_TRANSITION,
		CL_DPREP_STATE_BO1_V13V14,
		CL_DPREP_STATE_BO2_V13V14_TRANSITION,
		CL_DPREP_STATE_BO2_V13V14,
		CL_DPREP_STATE_POST_FILLING_STANDBY,
		CL_DPREP_STATE_MAXFILLINGSTATE
	} Cl_Dprep_filling_States;

typedef union{
	
	Cl_Uint8Type bytearray[8] ;
	Cl_Uint16Type Twobyte  ;
	Cl_Uint32Type word ;
	
}cl_PrepDatatype;


typedef struct
{
	
	Cl_NewAlarmIdType Cl_DprepAlarmId;
	Cl_AlarmType Cl_DprepAlarmType;
	Cl_BoolType IsActive;
	Cl_BoolType IsRaised;
	Cl_BoolType userclear;
	Cl_BoolType critical;
	Cl_BoolType Mute;
	
}DprepAlarmsType;



#define CL_DPREP_PRIME_TIMEOUT_MIN 1
#define CL_DPREP_PRIME_TIMEOUT_HRS 0
#define CL_DPREP_DIALISER_PRIME_TIMEOUT_MIN 1
#define CL_DPREP_DIALISER_PRIME_TIMEOUT_HRS 0

#define CL_DPREP_FILLING_TIMEOUT_MIN 10


#endif /* CL_DPREP_CONTROLLER_H_ */
