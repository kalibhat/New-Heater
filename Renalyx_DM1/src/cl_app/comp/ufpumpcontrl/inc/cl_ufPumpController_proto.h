/*
 * cl_ufPumpController_proto.h
 *
 * Created: 4/25/2017 7:31:12 PM
 *  Author: user
 */ 


#ifndef CL_UFPUMPCONTROLLER_PROTO_H_
#define CL_UFPUMPCONTROLLER_PROTO_H_


Cl_ReturnCodeType cl_uf_controller(cl_uf_events, int32_t);
Cl_ReturnCodeType cl_isouf_controller(Cl_Mac_EventsType);

Cl_ReturnCodeType cl_checkforblooddetection();
Cl_ReturnCodeType cl_isoufcompleted();
Cl_ReturnCodeType cl_dialysate_ready_isouf();
Cl_ReturnCodeType Cl_isouf_init(void);
Cl_ReturnCodeType  cl_isouf_translatemacevent(Cl_Mac_EventsType,cl_isouf_events* );
Cl_ReturnCodeType Cl_Iso_UpdateTimeInfo(void);
Cl_ReturnCodeType Cl_Iso_Senddata();
Cl_ReturnCodeType Cl_Isouf_stop();
Cl_ReturnCodeType Cl_Isouf_start();

#endif /* CL_UFPUMPCONTROLLER_PROTO_H_ */