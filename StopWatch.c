#include "StopWatch.h"
#include "DIO_interface.h"
#include "_7SEG_interface.h"


/* Globals */

static System_State_type Current_State=IDEL;
static u16 N=0,Buffer;
static u8 start_flag=0;
static u8 up_down_flag=0 , puase_play_flag=0;


void  StopWatch(void)
{
	switch (Current_State)
	{
		
		case IDEL:
		/*Check buttons */
		if (!DIO_ReadPin(START_SET_UP_COUNT))
		{
			
			
			start_flag=1;
			Current_State=CONF_START;
			while(!DIO_ReadPin(START_SET_UP_COUNT));
		}
		if (!DIO_ReadPin(TYPE_FREEZ_DOWN_COUNT))
		{
			 Current_State=SET_COUNTING_MODE;
			while(!DIO_ReadPin(TYPE_FREEZ_DOWN_COUNT));	
		}

		/*Segment Display */
		_7SEG_Disp_DECIMAL_X4(SEGMENT1,N);
		break;
		case CONF_START: 
		if (!DIO_ReadPin(START_SET_UP_COUNT) && start_flag==1) /* Toggle between inc/dec*/
		{
	
			start_flag=2; 
			while(!DIO_ReadPin(START_SET_UP_COUNT));
		 }
		 else if (!DIO_ReadPin(START_SET_UP_COUNT) && start_flag==2)
		 {
			start_flag=1; 			 
			while(!DIO_ReadPin(START_SET_UP_COUNT));
		 }
		 
		 if (start_flag==1 && !DIO_ReadPin(PAUSE_RESUME_INC_DEC_MIN)) /*Min increment*/
		 {
			 if ((N/100)!=99)/*Min overflow up */
			 {
				N= N+(1*100); 
			 }

			 while(!DIO_ReadPin(PAUSE_RESUME_INC_DEC_MIN));
		 }
		 if (start_flag==2 && !DIO_ReadPin(PAUSE_RESUME_INC_DEC_MIN)) /*Min decrement */
		 {
			 if (N/100!=0) /*Min Overflow Down*/ 
			 {
				  N= N-(1*100); 
			 }
			 
			 while(!DIO_ReadPin(PAUSE_RESUME_INC_DEC_MIN));
		 }
		
		 if (start_flag==1 && !DIO_ReadPin(RESET_PLAY_INC_DEC_SEC)) /*Seconds increment*/
		 {
			 
			 N= N+(1);

			 while(!DIO_ReadPin(RESET_PLAY_INC_DEC_SEC));
		 }
		 if (start_flag==2 && !DIO_ReadPin(RESET_PLAY_INC_DEC_SEC)) /*Seconds decrement */
		 {
			 
			 if ((N%100)!=0)/*Seconds overflow Down*/
			 {
				N= N-(1);
			 }
			 while(!DIO_ReadPin(RESET_PLAY_INC_DEC_SEC));
		 }
		 if ((N%100)==60)/*Seconds overflow up */
		 {
			N= N+(1*100);  /*Increment Min*/
			N=(N/100)*100;	   /*Reset Seconds*/
		 }
		 if (start_flag==1 && !DIO_ReadPin(TYPE_FREEZ_DOWN_COUNT)) /* ==> Mode selection*/
		 {
			 Buffer=N; /*  Store latest selected number */
			 Current_State=SET_COUNTING_MODE;
			 while(!DIO_ReadPin(TYPE_FREEZ_DOWN_COUNT));
		 }			
		
			_7SEG_Disp_DECIMAL_X4(SEGMENT1,N); /*Always display because MPX*/		
		break;
		case CONF_RUN:
		if (!DIO_ReadPin(START_SET_UP_COUNT)) /* Toggle between inc/dec*/
		{
			
			up_down_flag^=1;
			while(!DIO_ReadPin(START_SET_UP_COUNT));
		}

		
		if (up_down_flag==0 && !DIO_ReadPin(PAUSE_RESUME_INC_DEC_MIN)) /*Min increment*/
		{
			if ((N/100)!=99)/*Min overflow up */
			{
				N= N+(1*100);
			}

			while(!DIO_ReadPin(PAUSE_RESUME_INC_DEC_MIN));
		}
		if (up_down_flag==1 && !DIO_ReadPin(PAUSE_RESUME_INC_DEC_MIN)) /*Min decrement */
		{
			if (N/100!=0) /*Min Overflow Down*/
			{
				N= N-(1*100);
			}
			
			while(!DIO_ReadPin(PAUSE_RESUME_INC_DEC_MIN));
		}
		
		if (up_down_flag==0 && !DIO_ReadPin(RESET_PLAY_INC_DEC_SEC)) /*Seconds increment*/
		{
			
			N= N+(1);

			while(!DIO_ReadPin(RESET_PLAY_INC_DEC_SEC));
		}
		if (up_down_flag==1 && !DIO_ReadPin(RESET_PLAY_INC_DEC_SEC)) /*Seconds decrement */
		{
			
			if ((N%100)!=0)/*Seconds overflow Down*/
			{
				N= N-(1);
			}
			while(!DIO_ReadPin(RESET_PLAY_INC_DEC_SEC));
		}
		if ((N%100)==60)/*Seconds overflow up */
		{
			N= N+(1*100);  /*Increment Min*/
			N=(N/100)*100;	   /*Reset Seconds*/
		}
		if (!DIO_ReadPin(TYPE_FREEZ_DOWN_COUNT)) /* ==> Mode selection*/
		{
			Buffer=N; /*  Store latest selected number */
			Current_State=SET_COUNTING_MODE;
			while(!DIO_ReadPin(TYPE_FREEZ_DOWN_COUNT));
		}		
		
			_7SEG_Disp_DECIMAL_X4(SEGMENT1,N);		
		break;
		case TIME_UP_ALARM:
		for(u8 i=0;i<20;i++) /* Stand every 50 * 4ms = 200 ms, frequency 2.5 HZ*/
		{
			if (!DIO_ReadPin(START_SET_UP_COUNT)) /* Config and start counting again*/
			{
				N=0;
				Current_State=CONF_START;
				while(!DIO_ReadPin(START_SET_UP_COUNT));
			}
			 if (!DIO_ReadPin(TYPE_FREEZ_DOWN_COUNT)) /* ==> Mode selection*/
			 {
				 
				 Current_State=SET_COUNTING_MODE;
				 while(!DIO_ReadPin(TYPE_FREEZ_DOWN_COUNT));
			 }			
			if (!DIO_ReadPin(PAUSE_RESUME_INC_DEC_MIN)) /* Run time config , work on latest N */
			{
				Current_State=CONF_RUN;	
				while(!DIO_ReadPin(PAUSE_RESUME_INC_DEC_MIN));
			}					
			 if (!DIO_ReadPin(RESET_PLAY_INC_DEC_SEC)) /*Restart Counting , fast switch */
			 {
				if (up_down_flag) 
				{
					N=0;
					Current_State=COUNT_UP;			
				}
				else
				{
					N=Buffer;
					Current_State=COUNT_DOWN;
				}
				 while(!DIO_ReadPin(RESET_PLAY_INC_DEC_SEC));
			 }						
			/* Behavior*/
			
			DIO_WritePin(ALARM_LED,HIGH);
			DIO_WritePin(ALARM_BUZZER,HIGH);	
		   _7SEG_Disp_DECIMAL_X4(SEGMENT1,N);	
		}
		for(u8 i=0;i<20;i++) /* Stand every 50 * 4ms = 200 ms, frequency 2.5 HZ*/
		{
	
			if (!DIO_ReadPin(START_SET_UP_COUNT)) /* Config and start counting again*/
			{
				N=0;
				Current_State=CONF_START;
				while(!DIO_ReadPin(START_SET_UP_COUNT));
			}
			if (!DIO_ReadPin(TYPE_FREEZ_DOWN_COUNT)) /* ==> Mode selection*/
			{
				
				Current_State=SET_COUNTING_MODE;
				while(!DIO_ReadPin(TYPE_FREEZ_DOWN_COUNT));
			}
			if (!DIO_ReadPin(PAUSE_RESUME_INC_DEC_MIN)) /* Run time config , work on latest N */
			{
				Current_State=CONF_RUN;
				while(!DIO_ReadPin(PAUSE_RESUME_INC_DEC_MIN));
			}
			if (!DIO_ReadPin(RESET_PLAY_INC_DEC_SEC)) /*Restart Counting , fast switch */
			{
				if (up_down_flag)
				{
					N=0;
					Current_State=COUNT_UP;
				}
				else
				{
					N=Buffer;
					puase_play_flag=0;
					Current_State=COUNT_DOWN;
				}
				while(!DIO_ReadPin(RESET_PLAY_INC_DEC_SEC));
			}			
			/* Behavior*/
			DIO_WritePin(ALARM_LED,LOW);
			DIO_WritePin(ALARM_BUZZER,LOW);
			_7SEG_Disp_DECIMAL_X4(SEGMENT1,N);
		}	
		break;
		case SET_COUNTING_MODE:
		/*Check buttons */
		if (!DIO_ReadPin(START_SET_UP_COUNT))
		{
			
			
				up_down_flag=1;
			while(!DIO_ReadPin(START_SET_UP_COUNT));
		}		
		if (!DIO_ReadPin(RESET_PLAY_INC_DEC_SEC))
		{
			
			if (up_down_flag) /* ==> UP Counting */
			{
				/*when Counting Up start from 0*/
				N=0;
				Current_State=COUNT_UP;
			}
			else  /* ==> Down Counting*/
			{
				puase_play_flag=0;
			    Current_State=COUNT_DOWN;
			}
			
			
			while(!DIO_ReadPin(RESET_PLAY_INC_DEC_SEC));
		}
		if (!DIO_ReadPin(TYPE_FREEZ_DOWN_COUNT))
		{
			
			 up_down_flag=0;
			while(!DIO_ReadPin(TYPE_FREEZ_DOWN_COUNT));
		}		
		
		_7SEG_Disp_DECIMAL_X4(SEGMENT1,N);			
		break;
		case  COUNT_UP:
		for(u8 i=0;i<240;i++) /* Change number every 1 sec for frequency 1000Hz , then 4ms * 250=1000 ms = 1 sec  */
		{
			/* Check Buttons */
			if (!DIO_ReadPin(PAUSE_RESUME_INC_DEC_MIN)) /* This Ends Up counting*/
			{
				while(!DIO_ReadPin(PAUSE_RESUME_INC_DEC_MIN));
				Current_State=TIME_UP_ALARM;
				break;
			}
			if (!DIO_ReadPin(RESET_PLAY_INC_DEC_SEC)) /* ==> Reset up count*/
			{
				
				N=0;
				i=0;
				while(!DIO_ReadPin(RESET_PLAY_INC_DEC_SEC));
			}
		_7SEG_Disp_DECIMAL_X4(SEGMENT1,N);					
		}
		N++;
		/* Handle Up Overflows */
		 if ((N%100)==60)/*Seconds overflow up */
		 {
			 N= N+(1*100);  /*Increment Min*/
			 N=(N/100)*100;	   /*Reset Seconds*/
		 }
		 if ((N/100)==99)/*Min overflow up */
		 {
			 N=0; /* Reset */		
		 }
		break;
		case  COUNT_DOWN:
		for(u8 i=0;i<240;i++) /* Change number every 1 sec for frequency 1000Hz , then 4ms * 250=1000 ms = 1 sec  */
		{
			/* Check Buttons */
			if (!DIO_ReadPin(PAUSE_RESUME_INC_DEC_MIN)) /* This PAUSE/Play*/
			{
				while(!DIO_ReadPin(PAUSE_RESUME_INC_DEC_MIN));
					puase_play_flag^=1;	

			}
			if (!DIO_ReadPin(RESET_PLAY_INC_DEC_SEC)) /* ==> Reset Down count*/
			{
			
				N=Buffer;
				i=0;
				while(!DIO_ReadPin(RESET_PLAY_INC_DEC_SEC));
			}
			if (!DIO_ReadPin(START_SET_UP_COUNT)) /* Config and start counting again*/
			{
				N=0;
				Current_State=CONF_START;
				while(!DIO_ReadPin(START_SET_UP_COUNT));
			}
			if (!DIO_ReadPin(TYPE_FREEZ_DOWN_COUNT)) /* ==> Run Config*/
			{
				
				Current_State=CONF_RUN;
				while(!DIO_ReadPin(TYPE_FREEZ_DOWN_COUNT));
			}
			_7SEG_Disp_DECIMAL_X4(SEGMENT1,N);
		}
		if (!puase_play_flag)
		{
			 N--;
		}
		/* Handle Down Overflows */
		if ((N%100)==99)/*Seconds overflow down */
		{

			N=((N/100)*100)+59;	   /*Start from 59 sec*/

		}
		if (N==0)/* Counting Done */
		{
			N=0;
			Current_State=TIME_UP_ALARM;
		}

		break;
		default:
		
		break;
		
	}
}