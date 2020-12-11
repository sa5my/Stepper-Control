#ifndef STATES_H_
#define STATES_H_

void WelcomeScreen(void);
void SelectMode_Screen(void);
void SelectSpeed_Screen(void);
void SelectHome_Screen(void);
void LCD_SpdDisplay(void);
void LCD_PosDisplay(void);

void PC_Read(void);
void (*resetFunc)(void) = 0;

typedef enum {
  INCR_MODE = 1,
  JOG_MODE
} t_Mode;


typedef enum {
  WELCOME_SCRN = 1,
  MODE_SELECT,
  SPEED_CTRL,
  HOME_POSITION
} t_State;

t_State currentState, lastState, state;
t_Mode mode;

#endif        
