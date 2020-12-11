#ifndef HHD_CONFIG_H_
#define HHD_CONFIG_H_
/*==========================CONFIGURATIONS======================*/
/*********** Pin Configurations to Mega*********************/

#define KEYPAD_MODE  'K'
#define PC_MODE      'P'

/*Keypad Pins*/
#define KEY_1_PIN   50
#define KEY_2_PIN   51
#define KEY_3_PIN   52
#define KEY_4_PIN   53
#define KEY_COMMON  48

/* Home Button Pin*/
#define HOME_BTN    21

/* Interrupt Pin to Update Position */
#define POS_PIN     20

/* RS-485 PIN */
#define TXEN_485    42

/* Minimum Hold Time in milliseconds*/
#define MINHOLD_TIME 100

/************ Data Frame Commands*********************/
#define POS_CHANGE 246
#define SET_HOME  247
#define GO_HOME   248
#define INCREMENT 249
#define MOVE      250
#define STOP      251
#define SPEED     252
/* Direction*/
#define CW        253   /* Motor Direction - Clockwise*/
#define CCW       254  /* Motor Direction - Counter Clockwise*/
/* Terminate frame*/
#define TERMINATE 255
/* No.of bytes in data frame*/
#define FRAME     3

/* PC commands */
#define PC_START        'I'

#define PC_JOG_MODE     'A'
#define PC_INCR_MODE    'B'
#define PC_SPEED        'S'

#define PC_INCRMOVE_CW  'G'
#define PC_INCRMOVE_CCW 'H'
#define PC_SETHOME      'F'
#define PC_JOGMOVE_CW   'D'
#define PC_JOGMOVE_CCW  'E'
#define PC_JOG_STOP     'X'
#define PC_GOHOME       'J'

#define PC_BACK         'C'
#define PC_TERMINATE    'Z'


#define PC_RESET        'R'
#define PC_ACK          'Y'

#define PORTRAIT 0
#define LANDSCAPE 1

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

/*================================================================*/
#endif   /* HHD_CONFIG_H_*/
