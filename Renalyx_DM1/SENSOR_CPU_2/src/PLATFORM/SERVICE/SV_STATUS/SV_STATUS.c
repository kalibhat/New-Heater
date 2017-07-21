/*
 * SV_STATUS.c
 *
 * Created: 3/8/2017 2:50:45 PM
 *  Author: Electrical
 */ 


#include "SV_STATUS.h" 
#include "PLATFORM/DRIVER/DD_CAN/DD_CAN.h"
#include "Platform/Driver/DRIVER_CALLS.h"

extern sensor_status_t DD_READ_SENSORS(sv_sensortype ID_SENSOR, uint16_t *sensor_status);
extern uint32_t dd_can_mailbox_write(can_mb_conf_t *p_mailbox);
extern can_mb_conf_t can0_mailbox;
extern volatile uint8_t group_id_reg;
extern volatile uint8_t group_id_reg_id0;
extern volatile uint8_t group_id_reg_id1;
extern volatile uint8_t group_id_reg_id2;
extern volatile uint8_t group_id_reg_id3;
extern volatile uint8_t group_id_reg_id4;
extern volatile uint8_t group_id_reg_id5;
extern volatile uint8_t group_id_reg_id6;
#define DONE 0
#define CAN_MID_MIDvA_dd(value) ((0x1FFC0000 & ((value) << 18)))
uint8_t const No_of_group_id = 6;

static void SV_SEND_CAN_MAILBOX(SV_CAN_MAILBOX_SENSOR_DATA_REG_TYPE*  SV_CAN_MAILBOX_SENSOR_DATA_STRUCTURE)   {
	sv_data_size_type  sv_data_size;
	sv_data_size.bytearray[0] =SV_CAN_MAILBOX_SENSOR_DATA_STRUCTURE->CPU_SENDER_type_reg;
	sv_data_size.bytearray[1] =SV_CAN_MAILBOX_SENSOR_DATA_STRUCTURE->SENSOR_GROUP_ID_type_reg;
	
	sv_data_size.Twobyte[1]   =SV_CAN_MAILBOX_SENSOR_DATA_STRUCTURE->SENSOR_1;	
	sv_data_size.Twobyte[2]   =SV_CAN_MAILBOX_SENSOR_DATA_STRUCTURE->SENSOR_2;
	sv_data_size.Twobyte[3]   =SV_CAN_MAILBOX_SENSOR_DATA_STRUCTURE->SENSOR_3;
	
	can0_mailbox.ul_datal     =sv_data_size.fourbyte[0];
	can0_mailbox.ul_datah     =sv_data_size.fourbyte[1];
	can0_mailbox.uc_length = 8;
	can0_mailbox.ul_mb_idx =   MAILBOX_0;
	can0_mailbox.uc_obj_type = CAN_MB_TX_MODE ;
	can0_mailbox.uc_tx_prio = 1;
	can0_mailbox.uc_id_ver = 0;
	can0_mailbox.ul_id_msk = CAN_MFID_MFID_VA_dd(CAN_MASK);
	can0_mailbox.ul_id = CAN_MID_MIDvA_dd(MASTER_CPU_id);
//	dd_can_mailbox_write(&can0_mailbox);	
	while (CAN_MAILBOX_NOT_READY == dd_can_mailbox_write(&can0_mailbox));	
	dd_can_global_send_transfer_cmd(CAN_TCR_MB0);	
}


bool SV_put_sensor_data(SV_Sensor_status_type* sensor_struct)   {	
	
	SV_CAN_MAILBOX_SENSOR_DATA_REG_TYPE  SV_CAN_MAILBOX_SENSOR_DATA_REG;	
//	#if 0
	switch(group_id_reg)   {
		case 0:
		    SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		    SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_1 ;
		    SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->ps1status;
		    SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->ps2status;
		    SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->ps3status;
		    SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
			
			 SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
			 SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_2 ;
			 SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->thermocouple_status ;//100; //sensor_struct->thermocouple_status;
			 SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->Temp1status ;
			 SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->Temp2status;
			 SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);			 
			
			 
			  SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
			  SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_3 ;
			  SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   =sensor_struct->CS1_Tempstatus;//sensor_struct->Temp3status;
			  SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  =sensor_struct->CS2_Tempstatus;
			  SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->Temp3status;
			  SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG); 
			  
			    SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
			    SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_4;
			    SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->CS3_Tempstatus;
			    SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->CS1status ;
			    SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->CS2status;
			    SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
				
				SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
				SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_5 ;
				SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->CS3status;
				SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->DAC1status ;
				SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->DAAstatus;
				SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
				
				 SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
				 SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_6 ;
				 SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->DABstatus;
				 SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->DAC2status ;
				 SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->flow_sensor_status;
				 SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
		
		break;
		
		case 1:			
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_1 ;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->ps1status;		
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->ps2status;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->ps3status;
           SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
		break;   	
		
		case 2:	
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_2 ;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->thermocouple_status ;//100; //sensor_struct->thermocouple_status;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->Temp1status ;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->Temp2status;
		   SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
 		break;
		 
		case 3: 
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_3 ;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   =sensor_struct->CS1_Tempstatus;//sensor_struct->Temp3status;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  =sensor_struct->CS2_Tempstatus; 
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->Temp3status;
		   SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
		break;
		
		case 4:
		
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_4 ;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->CS3_Tempstatus;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->CS1status ;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->CS2status;
		   SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
		break;
		
		case  5:
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_5 ;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->CS3status;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->DAC1status ;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->DAAstatus;
		   SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
 		break;
		 
		case  6:
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_6 ;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->DABstatus;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->DAC2status ;
		   SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->flow_sensor_status;
		   SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
		break;
		
		default:
		break;
		
		return DONE;				
     } 
	 
	// #endif
}


bool SV_put_sensor_data1(SV_Sensor_status_type* sensor_struct)   {
	
	SV_CAN_MAILBOX_SENSOR_DATA_REG_TYPE  SV_CAN_MAILBOX_SENSOR_DATA_REG;
	//	#if 0
	if(group_id_reg_id0 == 1 )   
	//	
	{
		SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_1 ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->ps1status;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->ps2status;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->ps3status;
		SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
		
		SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_2 ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->thermocouple_status ;//100; //sensor_struct->thermocouple_status;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->Temp1status ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->Temp2status;
		SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
		
		
		SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_3 ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   =sensor_struct->CS1_Tempstatus;//sensor_struct->Temp3status;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  =sensor_struct->CS2_Tempstatus;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->Temp3status;
		SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
		
		SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_4;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->CS3_Tempstatus;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->CS1status ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->CS2status;
		SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
		
		SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_5 ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->CS3status;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->DAC1status ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->DAAstatus;
		SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
		
		SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_6 ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->DABstatus;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->DAC2status ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->flow_sensor_status;
		SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
		group_id_reg_id0 = 0;
	}
						   for(unsigned int count=0; count<500000; count++ )   {
		                ;
	                     }
		
		if(group_id_reg_id1 == 1 ) 
		{
			
		
		SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_1 ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->ps1status;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->ps2status;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->ps3status;
		SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
		group_id_reg_id1 = 0;
		}
							   for(unsigned int count=0; count<500000; count++ )   {
		                ;
	                     }
		
		if(group_id_reg_id2 == 1 )
		{
		SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_2 ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->thermocouple_status ;//100; //sensor_struct->thermocouple_status;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->Temp1status ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->Temp2status;
		SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
		group_id_reg_id2 = 0;
		}
							   for(unsigned int count=0; count<500000; count++ )   {
		                ;
	                     }
		
		if(group_id_reg_id3 == 1 )
		{
		SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_3 ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   =sensor_struct->CS1_Tempstatus;//sensor_struct->Temp3status;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  =sensor_struct->CS2_Tempstatus;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->Temp3status;
		SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
		group_id_reg_id3 = 0;
		}
		
							   for(unsigned int count=0; count<500000; count++ )   {
		                ;
	                     }
		if(group_id_reg_id4 == 1 )
		{
		
		SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_4 ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->CS3_Tempstatus;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->CS1status ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->CS2status;
		SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
		group_id_reg_id4 = 0;
		}					   for(unsigned int count=0; count<500000; count++ )   {
		                ;
	                     }
		if(group_id_reg_id5 == 1 )
		{
		SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_5 ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->CS3status;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->DAC1status ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->DAAstatus;
		SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
		group_id_reg_id5 = 0;
		}
							   for(unsigned int count=0; count<500000; count++ )   {
		                ;
	                     }
		if(group_id_reg_id6 == 1 )
		{
		SV_CAN_MAILBOX_SENSOR_DATA_REG.CPU_SENDER_type_reg        = SENSOR_CPU_id ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_GROUP_ID_type_reg	  = SENSOR_GROUP_ID_6 ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_1                   = sensor_struct->DABstatus;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_2	                  = sensor_struct->DAC2status ;
		SV_CAN_MAILBOX_SENSOR_DATA_REG.SENSOR_3	                  = sensor_struct->flow_sensor_status;
		SV_SEND_CAN_MAILBOX(&SV_CAN_MAILBOX_SENSOR_DATA_REG);
		group_id_reg_id6 = 0;
		}
							   for(unsigned int count=0; count<500000; count++ )   {
		                ;
	                     }
	
		
		return DONE;
	
	
	// #endif
}

void SV_get_sensor_data(SV_Sensor_status_type* sensor_struct)   {
	 uint16_t sensor_data;
	
	  SV_Sensor_status_type sensor_struct1;
	  uint16_t temp_sensor_data =0;
	  uint16_t sensor_status_flag;
	   
	   
 	    sensor_status_flag	= DD_READ_SENSORS(SV_PS1_ID, &sensor_data );	
		 if(sensor_status_flag == SENSOR_READ_OK)	{			 
			sensor_struct1.ps1status = sensor_data;
			sensor_struct->ps1status = sensor_struct1.ps1status	; 
		 }
		 else {}		
	   
	   sensor_status_flag			= DD_READ_SENSORS(SV_PS2_ID,&sensor_data);	   
	    if(sensor_status_flag == SENSOR_READ_OK)	{
		    sensor_struct1.ps2status = sensor_data;
		    sensor_struct->ps2status = sensor_struct1.ps2status	;
	    }
	    else {}
			
		sensor_status_flag			= DD_READ_SENSORS(SV_PS3_ID, &sensor_data);
		 if(sensor_status_flag == SENSOR_READ_OK)	{
			 sensor_struct1.ps3status = sensor_data;
			 sensor_struct->ps3status = sensor_struct1.ps3status	;
		 }
		 else {}
	  
   	      sensor_status_flag		= DD_READ_SENSORS(SV_TS1_ID, &sensor_data);
		  if(sensor_status_flag == SENSOR_READ_OK)	{
			  sensor_struct1.Temp1status = sensor_data;
			  sensor_struct->Temp1status = sensor_struct1.Temp1status	;
		  }
		  else {}
	 
  	   
 	  sensor_status_flag			= DD_READ_SENSORS(SV_TS2_ID, &sensor_data); 
	    if(sensor_status_flag == SENSOR_READ_OK)	{
		    sensor_struct1.Temp2status = sensor_data;
		    sensor_struct->Temp2status = sensor_struct1.Temp2status	;
	    }
	    else {}	   
	 	  
 	  
 	  sensor_status_flag		= DD_READ_SENSORS(SV_CS1_TS_ID, &sensor_data);
	    if(sensor_status_flag == SENSOR_READ_OK)	{
		    sensor_struct1.CS1_Tempstatus = sensor_data;
		    sensor_struct->CS1_Tempstatus = sensor_struct1.CS1_Tempstatus	;
	    }
	    else {}
	    
 	  
  	  sensor_status_flag   	= DD_READ_SENSORS(SV_CS2_TS_ID, &sensor_data);
		if(sensor_status_flag == SENSOR_READ_OK)	{
			sensor_struct1.CS2_Tempstatus = sensor_data;
			sensor_struct->CS2_Tempstatus = sensor_struct1.CS2_Tempstatus	;
		}
		else {}
			
	 sensor_status_flag   	= DD_READ_SENSORS(SV_CS3_TS_ID, &sensor_data);
	 if(sensor_status_flag == SENSOR_READ_OK)	{
		 sensor_struct1.CS3_Tempstatus = sensor_data;
		 sensor_struct->CS3_Tempstatus = sensor_struct1.CS3_Tempstatus	;
	 }
	 else {}
  	   
	   
 	 
	   
  	   sensor_struct1.Temp3status			= DD_READ_SENSORS(SV_TS3_ID, &sensor_data);
 	   sensor_struct->Temp3status         = sensor_data	;	
     sensor_struct1.CS1status			= DD_READ_SENSORS(SV_CS1_ID, &sensor_data);	
     sensor_struct->CS1status           = sensor_data		;
	  	
 	sensor_struct1.CS2status			= DD_READ_SENSORS(SV_CS2_ID, &sensor_data);	
     sensor_struct->CS2status           = sensor_data	;	
	
	sensor_struct1.CS3status			= DD_READ_SENSORS(SV_CS3_ID, &sensor_data);	
    sensor_struct->CS3status           = sensor_data		;	
	
	 sensor_struct1.DAC2status			= DD_READ_SENSORS(SV_DAC2_ID, &sensor_data);
	 sensor_struct->DAC2status           = sensor_data	;	
     sensor_struct1.DAAstatus			= DD_READ_SENSORS(SV_DAA_ID, &sensor_data);
	 sensor_struct->DAAstatus            = sensor_struct1.DAAstatus	;	
	 
	 sensor_struct1.DABstatus			= DD_READ_SENSORS(SV_DAB_ID, &sensor_data);
	 sensor_struct->DABstatus            = sensor_data	;	
	 
	 sensor_struct1.DAC1status       	= DD_READ_SENSORS(SV_DAC1_ID, &sensor_data);
	 sensor_struct->DAC1status           = sensor_data ;  
	
	
	
	 
	 
	 
	 
	
	/* sensor_struct1.flow_sensor_status	= DD_READ_SENSORS(SV_FLOW_SENSOR_ID);*/
	//sensor_struct->thermocouple_status  = sensor_struct1.thermocouple_status; 
	
	
		
			
				
		
		
		
	/*sensor_struct->flow_sensor_status   = sensor_struct1.flow_sensor_status	;	*/
}