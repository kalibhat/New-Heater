/*
 * flowswitchcntrl.c
 *
 * Created: 11/9/2014 2:08:25 PM
 *  Author: user
 */ 

#include "flowswitchcntrl_include.h"
#include "flowswitchcntrl_proto.h"

// extern Cl_ReturnCodeType Cl_AlarmResetAlarm(Cl_NewAlarmIdType  );
// extern Cl_ReturnCodeType Cl_Alarm_TriggerAlarm(Cl_NewAlarmIdType cl_alarm_id , Cl_BoolType cl_status);
// extern Cl_ReturnCodeType  Cl_SendDatatoconsole(Cl_ConsoleTxCommandtype , Cl_Uint8Type* ,Cl_Uint8Type );

Cl_Flowswitch_msgType Flowswitch_msg = {FLOW_SWITCH_EVENT_UNDEFINED,0 };
flowswitchstate_type flow_switch_state = FLOW_SWITCH_UNDEFINED;
flowswitchcntrlstate_type  flowswitchcntrlstate  = FLOW_SWITCH_CNTRL_IDLE;
static Cl_Uint16Type toggle_count_flow = 0;
	
	Cl_ReturnCodeType cl_flow_switchcontroller(flowswitchcntrl_event_type cntrl_event)
	{
		cl_Datastreamtype cl_tdata;
		static uint8_t three_sec_count = 0;
		
		switch(flowswitchcntrlstate)
		{
			case FLOW_SWITCH_CNTRL_ACTIVE:
					switch(cntrl_event)
					{
						case FLOW_SWITCH_CNTRLER_EVENT_START:
						flowswitchcntrlstate  = FLOW_SWITCH_CNTRL_READY;
			
						break;
						case FLOW_SWITCH_CNTRLER_EVENT_STOP:
						Cl_Alarm_TriggerAlarm(FLOW_LOW_FLOWRATE,0);
						Cl_Alarm_TriggerAlarm(FLOW_HIGH_FLOWRATE,0);
						flowswitchcntrlstate  = FLOW_SWITCH_CNTRL_IDLE;
						break;
						case FLOW_SWITCH_CNTRLER_EVENT_SEC:
						three_sec_count++;
						if(three_sec_count > 3)
						{
							three_sec_count = 0;
						
						Cl_AlarmResetAlarm(FLOW_NO_FLOW);
						Cl_AlarmResetAlarm(FLOW_LOW_FLOWRATE);
						
										cl_tdata.word = 0;

										
										cl_tdata.Twobyte = toggle_count_flow;
										cl_tdata.bytearray[2] = 21;
										
										
										
									//	 Cl_SendDatatoconsole(CON_TX_COMMAND_COMMAND_SCRIPT_PRNIT,&cl_tdata,4);
						if(toggle_count_flow < 2 )
						{
							// low  flow alarm 
							Cl_Alarm_TriggerAlarm(FLOW_LOW_FLOWRATE,1);
						//	flowswitchcntrlstate  = FLOW_SWITCH_CNTRL_STOPPED;
							//flowswitchcntrlstate  = FLOW_SWITCH_CNTRL_IDLE;
						}
						if(toggle_count_flow > 1500 )
						{
							//  high  flow alarm 
							Cl_Alarm_TriggerAlarm(FLOW_HIGH_FLOWRATE,1);
						//	flowswitchcntrlstate  = FLOW_SWITCH_CNTRL_STOPPED;
							//flowswitchcntrlstate  = FLOW_SWITCH_CNTRL_IDLE;
						}
						Cl_SendDatatoconsole(CON_TX_COMMAND_PRINTDATA,&toggle_count_flow,2);
						toggle_count_flow = 0;
						}
						
						break;
						case FLOW_SWITCH_CNTRLER_EVENT_100MS:
						break;
						case FLOW_SWITCH_CNTRLER_EVENT_10MS:
						//break;
						case FLOW_SWITCH_CNTRLER_EVENT_INSTANT:
			
							if(Flowswitch_msg.flowswitch_event == FLOW_SWITCH_EVENT_LEVEL_HIGH)
							{
								Flowswitch_msg.flowswitch_event =  FLOW_SWITCH_EVENT_UNDEFINED;
				
								if(flow_switch_state != FLOW_SWITCH_LEVEL_HIGH )
								{
									
									toggle_count_flow++;
									flow_switch_state = FLOW_SWITCH_LEVEL_HIGH;
								}

							}
							else if(Flowswitch_msg.flowswitch_event ==  FLOW_SWITCH_EVENT_LEVEL_LOW)
							{

								Flowswitch_msg.flowswitch_event =  FLOW_SWITCH_EVENT_UNDEFINED;

								if(flow_switch_state != FLOW_SWITCH_LEVEL_LOW )
								{
									toggle_count_flow++;
									flow_switch_state = FLOW_SWITCH_LEVEL_LOW;
								}

					
							}
						break;
						
						default:break;
					}
			break;
			case FLOW_SWITCH_CNTRL_IDLE:
								switch(cntrl_event)
								{
									case FLOW_SWITCH_CNTRLER_EVENT_START:
									flowswitchcntrlstate  = FLOW_SWITCH_CNTRL_READY;
									break;
									case FLOW_SWITCH_CNTRLER_EVENT_STOP:
									break;
								}
			break;
			
			case FLOW_SWITCH_CNTRL_READY:
								switch(cntrl_event)
								{			
									case FLOW_SWITCH_CNTRLER_EVENT_SEC:
									flowswitchcntrlstate  = FLOW_SWITCH_CNTRL_ACTIVE;
									break;
									case FLOW_SWITCH_CNTRLER_EVENT_INSTANT:
			
									if(Flowswitch_msg.flowswitch_event == FLOW_SWITCH_EVENT_LEVEL_HIGH)
									{
										Flowswitch_msg.flowswitch_event =  FLOW_SWITCH_EVENT_UNDEFINED;
				
										if(flow_switch_state != FLOW_SWITCH_LEVEL_HIGH )
										{
									
											toggle_count_flow++;
											flow_switch_state = FLOW_SWITCH_LEVEL_HIGH;
										}

									}
									else if(Flowswitch_msg.flowswitch_event ==  FLOW_SWITCH_EVENT_LEVEL_LOW)
									{

										Flowswitch_msg.flowswitch_event =  FLOW_SWITCH_EVENT_UNDEFINED;

										if(flow_switch_state != FLOW_SWITCH_LEVEL_LOW )
										{
											toggle_count_flow++;
											flow_switch_state = FLOW_SWITCH_LEVEL_LOW;
										}

					
									}
								break;
									default:break;
								}
			break;
			case FLOW_SWITCH_CNTRL_STOPPED:
								switch(cntrl_event)
								{
									case FLOW_SWITCH_CNTRLER_EVENT_START:
									flowswitchcntrlstate  = FLOW_SWITCH_CNTRL_ACTIVE;
									
									break;
									case FLOW_SWITCH_CNTRLER_EVENT_STOP:
									break;
								}
			break;
		}
		return CL_OK;
	}