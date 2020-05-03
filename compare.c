/*******************************************************************************************
* File Name          : Compare.c
* Description        : Compare.c: Comparision of two variables
*					   when the user enters the command with two numbers, its going to compare both of them 
*					   and it will show which one is greater amoung them
*					   the output will be A>B:A<B:A=B one of these
*
* Author:              Santhosh Nagendran
* Date:                26-02-2019
********************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "common.h"

uint SN_AFC( uint x,uint y );///////////////////function declaration for the comapre function which is going to the .s file
uint32_t a,b;////////////static declaration of the variables we are using in this
/* Parser command to access the functions mentioned above */
ParserReturnVal_t Cmdcomp(int mode)
{
	int x,y;
  	if(mode != CMD_INTERACTIVE) 
	return CmdReturnOk;
	x=fetch_uint32_arg(&a);////////////fetches the signed integer data 1 from the user which is to be compared
	y=fetch_uint32_arg(&b);////////////fetches the signed integer data 2 from the user which is to be compared
	if(x<0)
	{
	printf("please enter positive values \n");
	}
	else if(y<0)
	{
	printf("please enter positive values \n");
	}
	else
	{
	SN_AFC(a,b);/////////this is to call the function which is mentioned into the .s file
  	}
	return CmdReturnOk;
}
ADD_CMD("compare",Cmdcomp,"        Comparision")
