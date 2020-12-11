
#ifndef HANDHELD_DEVICE_C_
#define HANDHELD_DEVICE_C_
/******************* Header files inclusion *********************/
#include <MCUFRIEND_kbv.h>
#include <Keypad.h>
#include "States_PC.h"
#include "HHD_Config_PC.h"

/******************* Object instances Creation ******************/
/* TFT Instance */
MCUFRIEND_kbv tft;       // hard-wired for UNO shields anyway

/******************* Global Variables *********************/

const byte ROWS = 4; //four rows
const byte COLS = 1; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'2'},
  {'1'},
  {'4'},
  {'3'}
};
byte rowPins[ROWS] = {KEY_1_PIN, KEY_2_PIN, KEY_3_PIN, KEY_4_PIN}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {KEY_COMMON}; //connect to the column pinouts of the keypad

/************** Global Variables *************************/
char customKey;                    /* Store Keypad Number */
signed long spd;                         /* Motor Speed */
signed long pos;                   /* Motor Position*/
int minHoldTime = 0;               /* Minimum Hold Time for Key Press */
int lastKeyState = RELEASED;       /* Last Key Press State */
float data_spd;                      /* Speed index to be transmitted */
byte data[FRAME];                  /* Data array */
char ScrnDisplay_Cnt;              /* Flag to program TFT screen once */
char holdKey = '0';                      /* To identify which key is held */
unsigned long holdTime = 0;        /* To update hold Time */
boolean pos_update = false;

char ControlMode = PC_MODE;         /* PC_MODE = 1, KEYPAD_MODE =0 */
boolean pc_MoveCmd= false;
int pc_indata = 0;
int pc_Spddata = 0;
boolean pc_data = false;
char pc_RxData[5]={0,0,0,0,0};

extern const unsigned long MtrSpd_Delay[];
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

/******************* Function Definitions *********************/
void SelectMode_Screen()
{
  tft.fillScreen(BLACK);
  tft.drawRect(0, 0, 319, 239, WHITE);
  tft.setCursor(100, 10);
  tft.setTextSize(2);
  tft.setTextColor(CYAN);
  tft.println("Mode Select");
  tft.setCursor(20, 60);
  tft.setTextColor(WHITE);
  /*tft.print("Press ");*/
  tft.setTextColor(RED);
  tft.print("1 ");
  tft.setTextColor(WHITE);
  tft.print("-> Jog Mode");
  tft.setCursor(20, 90);
  /*tft.print("Press ");*/
  tft.setTextColor(RED);
  tft.print("2 ");
  tft.setTextColor(WHITE);
  tft.print("-> Incremental Mode");
  tft.setCursor(20, 120);
  /*tft.print("Press ");*/
  tft.setTextColor(RED);
  tft.print("3 ");
  tft.setTextColor(WHITE);
  tft.print("-> Back to Main Screen");
  tft.setCursor(20, 150);
  /*tft.print("Press ");*/
  /*tft.setTextColor(RED);
    tft.print("4 ");
    tft.setTextColor(WHITE);
    tft.print("-> Return ");*/

}

void SelectSpeed_Screen()
{
  tft.fillScreen(BLACK);
  tft.drawRect(0, 0, 319, 239, WHITE);
  tft.setCursor(90, 10);
  tft.setTextSize(2);
  tft.setTextColor(CYAN);
  tft.println("Speed Select");
  tft.setCursor(70, 40);
  tft.setTextColor(RED);
  tft.print("SPEED: ");
  tft.print(data_spd);
  tft.println(" RPM");
  tft.setTextColor(GREEN);
  if (mode == JOG_MODE)
  {
    tft.setCursor(80, 70);
    tft.print("MODE: ");
    tft.print("Jog Mode");
  }
  else if (mode == INCR_MODE)
  {
    tft.setCursor(40, 70);
    tft.print("MODE: ");
    tft.print("Incremental mode");
  }
  tft.setTextColor(WHITE);
  tft.setCursor(10, 100);
  /*tft.print("Press ");*/
  tft.setTextColor(RED);
  tft.print("1 ");
  tft.setTextColor(WHITE);
  tft.print("-> Increase speed");
  tft.setCursor(10, 130);
  /*tft.print("Press ");*/
  tft.setTextColor(RED);
  tft.print("2 ");
  tft.setTextColor(WHITE);
  tft.print("-> Decrease speed");
  tft.setCursor(10, 160);
  /*tft.print("Press ");*/
  tft.setTextColor(RED);
  tft.print("3 ");
  tft.setTextColor(WHITE);
  tft.print("-> Go to motor control");
  tft.setCursor(10, 190);
  /*tft.print("Press ");*/
  tft.setTextColor(RED);
  tft.print("4 ");
  tft.setTextColor(WHITE);
  tft.print("-> Back to Mode select");
}

void SelectHome_Screen()
{
  tft.fillScreen(BLACK);
  tft.drawRect(0, 0, 319, 239, WHITE);
  tft.setCursor(90, 10);
  tft.setTextSize(2);
  tft.setTextColor(CYAN);
  tft.println("Motor Control");
  tft.fillRect(0, 40, 319, 15, BLACK);
  tft.setCursor(20, 40);
  tft.setTextColor(RED);
  tft.print("Current Pos: ");
  tft.print(pos );
  tft.println(" degrees");
  tft.setCursor(70, 70);
  tft.setTextColor(GREEN);
  tft.print("SPEED: ");
  tft.print(data_spd);
  tft.println(" RPM");
  if (mode == JOG_MODE)
  {
    tft.setCursor(80, 100);
    tft.print("MODE: ");
    tft.print("Jog Mode");
  }
  else if (mode == INCR_MODE)
  {
    tft.setCursor(20, 100);
    tft.print("MODE: ");
    tft.print("Incremental mode");
  }
  tft.setTextColor(WHITE);
  tft.setCursor(10, 130);
  /*tft.print("Press ");*/
  tft.setTextColor(RED);
  tft.print("1 ");
  tft.setTextColor(WHITE);
  tft.print("-> Move Clockwise");
  tft.setCursor(10, 160);
  /*tft.print("Press ");*/
  tft.setTextColor(RED);
  tft.print("2 ");
  tft.setTextColor(WHITE);
  tft.print("-> Move Anti-Clockwise");
  tft.setCursor(10, 190);;
  /*tft.print("Press ");*/
  tft.setTextColor(RED);
  tft.print("3 ");
  tft.setTextColor(WHITE);
  tft.print("-> Set Home");
  tft.setCursor(10, 220);;
  /*tft.print("Press ");*/
  tft.setTextColor(RED);
  tft.print("4 ");
  tft.setTextColor(WHITE);
  tft.print("-> Back to speed select");
}


void WelcomeScreen()
{
  tft.fillScreen(BLACK);
  // put your setup code here, to run once:
  tft.drawRect(0, 0, 319, 239, WHITE);
  tft.setCursor(50, 10);
  tft.setTextSize(2);
  tft.setTextColor(CYAN);
  tft.println("SANDVIC COMPONENTS");
  tft.setCursor(120, 60);
  tft.setTextColor(WHITE);
  tft.println("Welcome");
  tft.setCursor(40, 120);
  tft.print("Press ");
  tft.setTextColor(RED);
  tft.print("4 ");
  tft.setTextColor(WHITE);
  tft.println("to continue");
}

void LCD_SpdDisplay() {
  tft.fillRect(0, 40, 319, 15, BLACK);
  tft.setCursor(70, 40);
  tft.setTextColor(RED);
  tft.print("SPEED: ");
  tft.print(data_spd);
  tft.println(" RPM");
}

void LCD_PosDisplay()
{
  tft.fillRect(0, 40, 319, 15, BLACK);
  tft.setCursor(30, 40);
  tft.setTextColor(RED);
  tft.print("Current pos: ");
  tft.print(pos );
  tft.println(" degrees");

}


void LCD_PosDisplay_Home()
{
  tft.fillRect(0, 40, 319, 15, BLACK);
  tft.setCursor(30, 40);
  tft.setTextColor(GREEN);
  tft.print("Current pos: ");
  tft.print(pos );
  tft.println(" degrees");
}


/************** Setup function ************************/
void setup() {

  tft.reset();
  /*ID = tft.readID();*/
  tft.begin(0x9341);
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600); /*debug*/
  pinMode(HOME_BTN, INPUT_PULLUP);
  pinMode(TXEN_485, OUTPUT);
  /*digitalWrite(TXEN_485, HIGH);*/
  pinMode(POS_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(HOME_BTN), goToHome, FALLING);
  attachInterrupt(digitalPinToInterrupt(POS_PIN), updatePos, FALLING);
  tft.setRotation(LANDSCAPE);
  customKeypad.setHoldTime(1000);
  state = WELCOME_SCRN;
  lastState = WELCOME_SCRN;
  ScrnDisplay_Cnt = 0;
  minHoldTime = MINHOLD_TIME;
  data[2] = TERMINATE;

}
/*********************Main function *******************/
void loop() {
  // put your main code here, to run repeatedly:
  /* down = Touch_getXY();*/
  customKey = customKeypad.getKey();
  if(ControlMode == PC_MODE)
  {
    PC_Read();
  }
  if(pc_RxData[0] == PC_RESET)
  {
    resetFunc();
  }
  /*Serial.println(customKeypad.getState());*/
  if (customKey)
  {
    holdKey = customKey;
  }
  switch (state)
  {
    case WELCOME_SCRN:              // WELCOME_SCRN
      currentState = WELCOME_SCRN;
      if (currentState != lastState)
      {
        ScrnDisplay_Cnt = 0;
      }
      if (ScrnDisplay_Cnt < 1)
      {
        WelcomeScreen();
        attachInterrupt(digitalPinToInterrupt(HOME_BTN), goToHome, FALLING);
        attachInterrupt(digitalPinToInterrupt(POS_PIN), updatePos, FALLING);
        ScrnDisplay_Cnt++;
      }
      if ((customKey == '4'))
      {
        ControlMode = KEYPAD_MODE;
        state = MODE_SELECT;
      }
      else if (pc_RxData[0] == PC_START && pc_data == true)
      {
        detachInterrupt(digitalPinToInterrupt(HOME_BTN));
        ControlMode = PC_MODE;
        state = MODE_SELECT;
        tft.fillScreen(BLACK);
        Serial.write(PC_ACK);
      }
      else
      {
        
      }
      pc_data = false;
      break;

    case MODE_SELECT:             // MODE SELECT
      currentState = MODE_SELECT;
      if (ControlMode == KEYPAD_MODE)
      {
        if (currentState != lastState)
        {
          ScrnDisplay_Cnt = 0;
        }
        if (ScrnDisplay_Cnt < 1)
        {
          SelectMode_Screen();
          ScrnDisplay_Cnt++;
        }
        if (customKey == '1')
        {
          state = SPEED_CTRL;
          mode = JOG_MODE;
        }
        else if (customKey == '2')
        {
          state = SPEED_CTRL;
          mode = INCR_MODE;
        }
        else if (customKey == '3')
        {
          state = WELCOME_SCRN;
        }
        /*else if (customKey == '4')
          {
          state = SPEED_CTRL;
          }*/
      }
      else if (ControlMode == PC_MODE)
      {
        if ((pc_RxData[0] == PC_JOG_MODE) && (pc_data == true))
        {
          state = SPEED_CTRL;
          mode = JOG_MODE;
          Serial.write(PC_ACK);
        }
        else if ((pc_RxData[0] == PC_INCR_MODE) && (pc_data == true))
        {
          state = SPEED_CTRL;
          mode = INCR_MODE;
          Serial.write(PC_ACK);
        }
        else if ((pc_RxData[0] == PC_BACK) && (pc_data == true))
        {
          state = WELCOME_SCRN;
          Serial.write(PC_ACK);
        }
        pc_data = false;       
      }
      break;

    case SPEED_CTRL:              // SPEED_CONTROL
      if (ControlMode == KEYPAD_MODE)
      {
        currentState = SPEED_CTRL;
        if (currentState != lastState)
        {
          ScrnDisplay_Cnt = 0;
        }
        if (ScrnDisplay_Cnt < 1)
        {
          SelectSpeed_Screen();
          data[0] = SPEED;
          ScrnDisplay_Cnt++;
        }
        if ((customKey == '1') || ((customKeypad.getState() == HOLD) && (holdKey == '1')))
        {
          if ((millis() - holdTime) > minHoldTime)
          {
            spd = (spd > 49) ? 0 : (spd + 1);
            data_spd = (data_spd > 4.9) ? 0 : (data_spd + 0.1);
            Serial.println(spd);
            Serial.println(data_spd);
            holdTime = millis();
            LCD_SpdDisplay();
          }

        }
        else if ((customKey == '2') || ((customKeypad.getState() == HOLD) && (holdKey == '2')))
        {
          if ((millis() - holdTime) > minHoldTime)
          {
            spd = (spd <= 0) ? 50 : (spd -= 1);
            data_spd = (data_spd < 0.1) ? (5.0) : (data_spd -= 0.1);
            Serial.println(spd);
            Serial.println(data_spd);
            holdTime = millis();
            LCD_SpdDisplay();
          }
        }
        else if (customKey == '3')
        {
          data[1] = spd;
          Serial.println(data_spd);
          Serial1.write(data, 3);
          state = HOME_POSITION;
        }
        else if (customKey == '4')
        {
          state = MODE_SELECT;
        }
      }
      else if (ControlMode == PC_MODE)
      {
        if (pc_RxData[0] == PC_BACK && pc_data == true)
        {
          state = MODE_SELECT;
          Serial.write(PC_ACK);
        }
        else if(pc_RxData[0] == PC_SPEED && pc_data == true)
        {
          pc_Spddata = atoi(&pc_RxData[1]);
          data[0] = SPEED;
          data[1] = pc_Spddata;
          Serial2.print(pc_Spddata);
          Serial1.write(data, 3);
          state = HOME_POSITION;
          Serial.write(PC_ACK);
        }
        pc_data = false;
      }
      break;

    case HOME_POSITION:             // HOME POSITION
      if (ControlMode == KEYPAD_MODE)
      {
        currentState = HOME_POSITION;
        if (currentState != lastState)
        {
          ScrnDisplay_Cnt = 0;
        }
        if (ScrnDisplay_Cnt < 1)
        {
          SelectHome_Screen();
          ScrnDisplay_Cnt++;
          holdTime = 0;
        }
        if (mode == INCR_MODE)
        {
          data[0] = INCREMENT;
          if (customKey == '1')
          {
            data[1] = CW;
            /*pos = (pos < 359) ? (pos + 1) : 0;*/
            Serial1.write(data, 3);
            /*LCD_PosDisplay();*/
          }
          else if (customKey == '2')
          {
            data[1] = CCW;
            /*pos = (pos < 1) ? (359) : (pos - 1);*/
            Serial1.write(data, 3);
            /*LCD_PosDisplay();*/
          }
          else if (customKey == '3')
          {
            data[0] = SET_HOME;
            Serial1.write(data, 3);
            pos = 0;
            LCD_PosDisplay_Home();
          }
          else if (customKey == '4')
          {
            state = SPEED_CTRL;
          }
        }
        else if (mode == JOG_MODE)
        {
          if ((customKey == '1') || (holdKey == '1'))
          {
            if ((customKeypad.getState() == RELEASED) /*&& (lastKeyState == PRESSED)*/)
            {
              Serial.println("Key released");
              lastKeyState = customKeypad.getState();
              data[0] = STOP;
              Serial1.write(data, 3);
              pos_update = false;
            }
            else if ((customKeypad.getState() == PRESSED) && (lastKeyState == RELEASED))
            {
              lastKeyState = customKeypad.getState();
              Serial.println("Key pressed or held");
              data[0] = MOVE;
              data[1] = CW;
              Serial1.write(data, 3);
              pos_update = true;
            }
#if 0
            else if (( pos_update = true) && (lastKeyState == PRESSED))
            {

              if (spd > 0)
              {
                delayMicroseconds(42120 - 13052);
                pos = (pos < 359) ? (pos + 1) : 0;
                LCD_PosDisplay();
              }

            }
#endif
            else
            {
              /* Do Nothing*/
            }
          }
          else if ((customKey == '2') || (holdKey == '2'))
          {
            if ((customKeypad.getState() == RELEASED) && (lastKeyState == PRESSED))
            {
              Serial.println("Key released");
              lastKeyState = customKeypad.getState();
              data[0] = STOP;
              Serial1.write(data, 3);
              pos_update = false;
            }
            else if ((customKeypad.getState() == PRESSED) && (lastKeyState == RELEASED))
            {
              lastKeyState = customKeypad.getState();
              Serial.println("Key pressed or held");
              data[1] = CCW;
              data[0] = MOVE;
              Serial1.write(data, 3);
              pos_update = true;
            }
#if 0
            else if (( pos_update = true) && (lastKeyState == PRESSED))
            {
              if (spd > 0)
              {
                delayMicroseconds(MtrSpd_Delay[spd] - 13052);
                pos = (pos < 1) ? (359) : (pos - 1);
                LCD_PosDisplay();
              }
            }
#endif
            else
            {
              /* Do Nothing*/
            }
          }
          else if (customKey == '3')
          {
            data[0] = SET_HOME;
            Serial1.write(data, 3);
            pos = 0;
            LCD_PosDisplay_Home();
          }
          else if (customKey == '4')
          {
            state = SPEED_CTRL;
          }
        }
      }
      else if (ControlMode == PC_MODE)
      {
        if (mode == INCR_MODE)
        {
          data[0] = INCREMENT;
          if (pc_RxData[0] == PC_INCRMOVE_CW && pc_data == true)
          {
            data[1] = CW;
            /*pos = (pos < 359) ? (pos + 1) : 0;*/
            holdKey = '1';
            Serial1.write(data, 3);
          }
          else if (pc_RxData[0] == PC_INCRMOVE_CCW && pc_data == true)
          {
            data[1] = CCW;
            /*pos = (pos < 1) ? (359) : (pos - 1);*/
            holdKey = '2';
            Serial1.write(data, 3);
          }
          else if (pc_RxData[0] == PC_SETHOME && pc_data == true)
          {
            data[0] = SET_HOME;
            Serial1.write(data, 3);
            pos = 0;
            Serial.write(PC_ACK);           
          }
          else if (pc_RxData[0] == PC_BACK && pc_data == true)
          {
            state = SPEED_CTRL;
            Serial.write(PC_ACK);
          }
          else if ((pc_RxData[0] == PC_GOHOME) && (pc_data == true))
          {
            goToHome();
            Serial.write(PC_ACK);
          }
          pc_data = false;
        }
        else if (mode == JOG_MODE)
        {
          if (pc_RxData[0] == PC_JOGMOVE_CW && pc_MoveCmd == false)
          {
            data[0] = MOVE;
            data[1] = CW;
            Serial1.write(data, 3);
            pc_MoveCmd = true;
            holdKey = '1';
          }
          else if (pc_RxData[0] == PC_JOGMOVE_CCW && pc_MoveCmd == false)
          {
            data[0] = MOVE;
            data[1] = CCW;
            Serial1.write(data, 3);
            pc_MoveCmd = true;
            holdKey = '2';
          }
          else if (pc_RxData[0] == PC_JOG_STOP && pc_data == true)
          {
            data[0] = STOP;
            Serial1.write(data, 3);
            holdKey = '0';
            pc_MoveCmd = false;
          }
          else if ((pc_RxData[0] == PC_SETHOME) && (pc_data == true))
          {
            data[0] = SET_HOME;
            Serial1.write(data, 3);
            pos = 0;
            Serial.write(PC_ACK);
            holdKey = '0';            
          }
          else if ((pc_RxData[0] == PC_BACK) && (pc_data == true))
          {
            state = SPEED_CTRL;
            Serial.write(PC_ACK); 
          }          
          else if ((pc_RxData[0] == PC_GOHOME) && (pc_data == true))
          {
             goToHome();
             Serial.write(PC_ACK);
          }
          pc_data = false;
        }       
      }
      break;

    default: break;
  }
  lastState = currentState;
}

void goToHome(void)
{
  if (state == HOME_POSITION)
  {
    data[0] = GO_HOME;
    data[1] = pos;
    pos = 0;
    Serial1.write(data, 3);
    if (ControlMode == KEYPAD_MODE)
    {
      LCD_PosDisplay();
    }
    else if (ControlMode == PC_MODE)
    {
      Serial.write(pos);
    }
    else
    {
      /*Do Nothing*/
    }
  }
  else
  {
    /* Do Nothing*/
  }
}

void updatePos(void)
{
  if ((mode == JOG_MODE || mode==INCR_MODE) && (state == HOME_POSITION))
  {
    if (holdKey == '1')
    {
      pos = (pos < 359) ? (pos + 1) : 0;
    }
    else if (holdKey == '2')
    {
      pos = (pos < 1) ? (359) : (pos - 1);
    }
    else
    {
      /*Do Nothing*/
    }
    if (ControlMode == KEYPAD_MODE)
    {
      LCD_PosDisplay();
    }
    else if (ControlMode == PC_MODE)
    {
      Serial.write(pos);
    }
    else
    {
      /*Do Nothing*/
    }
  }
  else
  {
    /*Do Nothing*/
  }
}

void PC_Read()
{
  static byte i = 0;
  char pc_strng = 0;  
  while (Serial.available() > 0 && pc_data == false)
  {
    pc_strng = 0;
    pc_strng = Serial.read();
    /*Serial.write(pc_strng);
    Serial.println();
    Serial.println(i);*/
    if (pc_strng != PC_TERMINATE)
    {
      if ((pc_strng != '\n') &&  (pc_strng != '\r'))
      {
      pc_RxData[i] = pc_strng;
      Serial2.write(pc_RxData[0]);
      i++;
      }
    }
    else
    {
      i = 0;
      pc_data = true;
    }
  }
}

#endif /* define HANDHELD_DEVICE_C_*/
