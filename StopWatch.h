/*
 * StopWatch.h
 *
 * Created: 4/30/2021 4:19:49 PM
 *  Author: Mohamed Allam
 */ 


#ifndef STOPWATCH_H_
#define STOPWATCH_H_



/*	States enum */
typedef enum{
	IDEL,							/* Time based  */
	CONF_START,						/* Event based */
	SET_COUNTING_MODE,				/* Event based */
	COUNT_UP,						/* Event based */
	COUNT_DOWN,						/* Event based */
	CONF_RUN,						/* Event based */
	TIME_UP_ALARM					/* Event based */
}System_State_type;


#define					PAUSE_RESUME_INC_DEC_MIN						PINC1
#define					RESET_PLAY_INC_DEC_SEC						PINC0
#define					START_SET_UP_COUNT							PINC2
#define					TYPE_FREEZ_DOWN_COUNT						PINC3
#define					ALARM_LED									PINB4
#define					ALARM_BUZZER								PINB5



/* Function Prototypes */
void  StopWatch(void);



#endif /* STOPWATCH_H_ */