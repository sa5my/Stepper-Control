#define dirPin 22
#define stepPin 23
#define RS485_RXEN 24

#define DIR 0   /*Payload Data - Direction*/
#define SPD 1 /*Payload Data - Speed*/

/* Commands*/
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

extern const unsigned long MtrSpd_Table[];

unsigned long time_degree_init, time_degree;

signed long pos;
unsigned int rx_data[3], lastDirState;
unsigned int speed_temp, speed_index, move_flag;
boolean newData = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  /*pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);*/
  DDRA |= 0x07;   /* 22- PORTA0, 23-PORTA1*/
  /*pinMode(RS485_RXEN, OUTPUT);
  digitalWrite(RS485_RXEN, LOW);*/
  while (!Serial1);
}

int i = 0;

void loop() {
  /*time_degree_init = micros();*/
  static byte i = 0;
  int endMarker = TERMINATE;
  String rc = "";
  int indata = 0;
  while ((Serial1.available() > 0) && (newData == false))
  {
    rc = "";
    rc = Serial1.read();
    /*Serial.print("Received:");
    Serial.println(rc.toInt());*/
    indata = rc.toInt();
    if (indata != endMarker)
    {
      rx_data[i] = indata;
      i++;
    }
    else
    {
      i = 0;
      newData = true;
    }
  }
  switch (rx_data[0])
  {
    case MOVE:
      if (newData == true)
      {
        if ((rx_data[1] == CW))
        {
          /*Serial.println("Motor move");*/
          Move_Motor(CW, 1);
          if (pos < 359)
          {
            pos += 1;
          }
          else
          {
            pos = 0;
          }
          move_flag = 1;
        }
        else if ((rx_data[1] == CCW))
        {
          Move_Motor(CCW, 1);
          move_flag = 1;
          if (pos < 0)
          {
            pos = 359;
          }
          else
          {
            pos-=1;
          }
        }
      }
      else
      {
        if ((move_flag == 1) && (rx_data[1] == CW))
        {
          /*Serial.println("Motor move");*/
          Move_Motor(CW, 1);
          if (pos < 359)
          {
            pos += 1;
          }
          else
          {
            pos = 0;
          }
          /*time_degree = micros() - time_degree_init;
          Serial.println(time_degree);*/
        }
        else if ((move_flag == 1) && (rx_data[1] == CCW))
        {
          Move_Motor(CCW, 1);
          if (pos < 0)
          {
            pos = 359;
          }
          else
          {
            pos-=1;
          }
         
        }
      }
      break;

    case STOP:
      if (newData == true) {
        Serial.println("Motor stop");
        move_flag = 0;
      }
      else
      {
        /*Serial.println("Reaching here");*/
        /* Do Nothing*/
      }
      newData = false;
      break;

    case SPEED:
      if (newData == true) {
        speed_index = rx_data[SPD];
        Serial.println(speed_index);
        newData = false;
      }
      break;

    case INCREMENT:
      if ((newData == true) && (rx_data[1] == CW))
      {
        Serial.println("Motor move");
        Move_Motor(CW, 1);
        if (pos < 359)
        {
          pos += 1;
        }
        else
        {
          pos = 0;
        }
      }
      else if ((newData == true) && (rx_data[1] == CCW))
      {
        Move_Motor(CCW, 1);
        if (pos < 0)
        {
          pos = 359;
        }
        else
        {
          pos-=1;
        }
      }
      break;

    case GO_HOME:
      if ((newData == true))
      {
        if ((pos > 0U) && (pos < 180U))
        {
          Serial.println(pos);
          Serial.println("Home <180");
          Move_Motor(CCW, pos);
          pos=0;
        }
        else if ((pos > 179U) && (pos < 360U))
        {
          Serial.println(pos);
          Serial.println("Home >180");
          Move_Motor(CW, (int)(360U - pos));
          pos = 0;
        }
      }
      newData = false;
      break;

    case SET_HOME:
      if (newData == true)
      {
        pos = 0U;
      }
      newData = false;
      break;

    default:
      break;
  }
}

void Move_Motor(const int mtr_dir, int degree)
{
  int steps;
  steps =0;
  newData = false;
  if (lastDirState != mtr_dir)
  {
    if (mtr_dir == CW)
    {
      PORTA |= 0x01;  /* Dir Pin clockwise */
    }
    else if (mtr_dir == CCW)
    {
      PORTA &= ~0x01;  /* Dir Pin CounterClockwise */
    }
  }
  lastDirState = mtr_dir;

  while ((steps < degree) && (speed_index > 0))
  {
    for (int i = 0; i < 18; i++)  /* 18 steps to move 1 degree */
    {
      PORTA |= 0x02;      /* stepPin HIGH*/
      delayMicroseconds(MtrSpd_Table[speed_index]);
      PORTA &= ~0x02;    /* stepPin LOW*/
      delayMicroseconds(MtrSpd_Table[speed_index]);
    }
    steps++;
    if((rx_data[0] == MOVE) || (rx_data[0] == INCREMENT))
    {
    PORTA |= 0x04;    /* Feedback ack to increment position at HHD*/
    PORTA &= ~0x04;   
    }
    else
    {
      /* Do Nothing */
    }
  }
  /*Serial.println("Motor Spun");*/
}
