/*
 * cl_utilities.c
 *
 * Created: 1/15/2014 10:09:34 AM
 *  Author: user
 */ 
#include "cl_types.h"

Cl_ReturnCodeType cl_systemtickcallback(void);
Cl_ReturnCodeType cl_wait(Cl_Uint32Type ul_dly_ticks);
Cl_ReturnCodeType Cl_MilliTickcallBack(void);
Cl_ReturnCodeType cl_memset(Cl_Uint8Type* , Cl_Uint8Type );

volatile Cl_Uint32Type g_ul_ms_ticks = 0;

static void mdelay(Cl_Uint32Type ul_dly_ticks)
{
	Cl_Uint32Type ul_cur_ticks;

	ul_cur_ticks = g_ul_ms_ticks;
	while ((g_ul_ms_ticks - ul_cur_ticks) < ul_dly_ticks);
}

Cl_ReturnCodeType cl_wait(Cl_Uint32Type ul_dly_ticks)
{
	mdelay(ul_dly_ticks);
	return CL_OK;
}

Cl_ReturnCodeType cl_systemtickcallback(void)
{
	Cl_ReturnCodeType Cl_retVal;
	Cl_retVal = Cl_MilliTickcallBack();
	g_ul_ms_ticks++;
	return  CL_OK;
}


Cl_ReturnCodeType cl_memset(Cl_Uint8Type* pdata, Cl_Uint8Type datasize)
{
	Cl_Uint8Type tempcnt = 0 ;
	Cl_Uint8Type* pdata1;
	
	pdata1 = (Cl_Uint8Type*)pdata;
	
	for (tempcnt = 0 ; tempcnt < datasize ; tempcnt++)
	{
		pdata[tempcnt] = 0;
	//	*pdata = 1;
	//	pdata++;
	//	*pdata1 = (Cl_Uint8Type*)0;
	//	pdata1 =  pdata1 +1;
	}
	return CL_OK;
}