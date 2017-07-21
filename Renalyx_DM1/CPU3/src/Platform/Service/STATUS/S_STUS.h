/*
 * S_STUS.h
 *
 * Created: 12/11/2014 11:09:01 AM
 *  Author: wid7
 */ 


#ifndef S_STUS_H_
#define S_STUS_H_

#include "asf.h"
#include "Platform/Service/sv_interface.h"
#include "Platform/Driver/Driver_call.h"

uint16_t cpu3_sv_status_getsystemstatus(Sys_statusType* sensor_struct);



#endif /* S_STUS_H_ */