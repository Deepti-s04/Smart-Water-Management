int Level = 0 , OldLevel = 0, OldLevelOF = 0;
int  FreshLevel = 0, TempLevel = 0;
int StabilityCount = 0 ;
bool R = 0, G = 0, B = 0;

bool LPC = 0 , LPC1 = 0;
unsigned long PM = 0 , NM = 0, SM = 0;

int BuzzerCounter = 0, SwitchCounter = 0;

///////////////// LED pins////////////////
const int Rpin = 2;
const int GND = 3;
const int Gpin = 4;
const int Bpin = 5;
/////////////////////////////////////////

const int Switch = 8; //bluewhite
const int Buzzer = 9; // Blue

void setup()
{
  Serial.begin(9600);

  pinMode(Rpin, OUTPUT);
  pinMode(Gpin, OUTPUT);
  pinMode(Bpin, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(GND, OUTPUT);

  digitalWrite(Rpin, LOW);
  digitalWrite(Gpin, LOW);
  digitalWrite(Bpin, LOW);
  digitalWrite(Buzzer, LOW);
  digitalWrite(GND, LOW);

  pinMode(Switch, INPUT);

  pinMode(A1, OUTPUT);  // Sensor Power

  for (short i = 0; i < 4; i++)    // Indicates Device StartUp
  {
    digitalWrite(Gpin, HIGH);
    delay(100);
    digitalWrite(Gpin, LOW);
    delay(50);
  }
  for (short i = 0; i < 4; i++)    // Indicates Device StartUp
  {
    digitalWrite(Rpin, HIGH);
    delay(100);
    digitalWrite(Rpin, LOW);
    delay(50);
  }
  for (short i = 0; i < 4; i++)    // Indicates Device StartUp
  {
    digitalWrite(Bpin, HIGH);
    delay(100);
    digitalWrite(Bpin, LOW);
    delay(50);
  }
}

void loop()
{
  delay(250);

  CheckLevel();

  LEDUpdate();

  OverFlowCheck();
  RserveLevelCheck();

  if (digitalRead(Switch))
    ManualCheck();
}

void CheckLevel()
{
  digitalWrite(A1, HIGH);
  delayMicroseconds(50);
  int raw = analogRead(A0);          // Sensor output
  Serial.println(raw);
  digitalWrite(A1, LOW);
  delayMicroseconds(50);

  if (raw < 300)  // Wire breakage
    FreshLevel  = 0;
  else if (raw >= 300 && raw < 400) // Empty Tank
    FreshLevel  = 1;
  else if (raw >= 400 && raw < 500)
    FreshLevel  = 2;
  else if (raw >= 800 && raw < 900)
    FreshLevel  = 3;
  else
    FreshLevel  = 4;


  if (FreshLevel != Level)     // Fresh reading is not equal to Stable Level but equals to intermediate level then stability count is raised.
  {
    StabilityCount++;
  }
  else                                                          // Fresh reading is equal to Stable Level than Intermediate is set to Stable level and Stability counter is reset.
  {
    StabilityCount = 0;
  }


  if (StabilityCount == 10)     //It will take 10 Cycles for Level Finalization
  {
    Level = FreshLevel;
    StabilityCount = 0;
  }
}

void LEDUpdate()
{
  if (Level == 0  )
  {
    R = 1; G = 0; B = 1;
  }
  else if (Level == 1  )
  {
    R = 1; G = 0; B = 0;
  }
  else if (Level == 2  )
  {
    R = 0; G = 0 ; B = 1;
  }
  else if (Level == 3  )
  {
    R = 0; G = 1; B = 0;
  }
  else if (Level == 4  )
  {
    R = 1; G = 1; B = 1;
  }
  digitalWrite(Rpin, R);
  digitalWrite(Gpin, G);
  digitalWrite(Bpin, B);

}


void OverFlowCheck()
{
  if (Level != OldLevelOF && Level == 3 && OldLevelOF != 0 )
  {
    OldLevelOF = Level;
    digitalWrite(Buzzer, HIGH);
    digitalWrite(Gpin, HIGH);

    while (1)
    {
      if (digitalRead(Switch))
      {
        digitalWrite(Buzzer, LOW);
        break;
      }
    }
    delay(2000);
  }
  else
  {
    OldLevelOF = Level;
  }
}

void RserveLevelCheck()
{
  if (Level != OldLevel && Level == 1 && OldLevel != 0)
  {
    OldLevel = Level;
    for (int i = 0; i <= 20 ; i++)
    {
      digitalWrite(Buzzer, HIGH);
      digitalWrite(Rpin, HIGH);
      delay(200);
      if (digitalRead(Switch))
        break;
      delay(200);
      if (digitalRead(Switch))
        break;

      digitalWrite(Buzzer, LOW);
      digitalWrite(Rpin, LOW);
      delay(200);
      if (digitalRead(Switch))
        break;
      delay(200);
      if (digitalRead(Switch))
        break;
      delay(100);
      if (digitalRead(Switch))
        break;
    }
    digitalWrite(Buzzer, LOW);
    digitalWrite(Rpin, LOW);
    delay(2000);
  }
  else
  {
    OldLevel = Level;
  }
}

void ManualCheck()
{
  while (digitalRead(Switch))
  {
    delay(100);
    SwitchCounter++;
    if (SwitchCounter >= 20)
    {
      SwitchCounter = 0;
      SwitchOff();
    }
  }
  SwitchCounter = 0;
}


void SwitchOff()
{
  digitalWrite(Rpin, LOW);
  digitalWrite(Gpin, LOW);
  digitalWrite(Bpin, LOW);

  digitalWrite(Buzzer, HIGH);
  delay(500);
  digitalWrite(Buzzer, LOW);

  for (short i = 0; i < 4; i++)    // Indicates Device ShutOff
  {
    digitalWrite(Rpin, HIGH);
    delay(100);
    digitalWrite(Rpin, LOW);
    delay(50);
  }

  Level = 0 ; OldLevel = 0; OldLevelOF = 0;
  R = 0; G = 0; B = 0;
  BuzzerCounter = 0 ; SwitchCounter = 0;
  bool isSleepMode = true;
  while (isSleepMode)
  {
    delay(250);

    while (digitalRead(Switch))
    {
      delay(100);
      SwitchCounter++;
      if (SwitchCounter >= 20)
      {
        SwitchCounter = 0;
        isSleepMode = false;

        digitalWrite(Buzzer, HIGH);
        delay(500);
        digitalWrite(Buzzer, LOW);
        for (short i = 0; i < 4; i++)    // Indicates Device StartUp
        {
          digitalWrite(Gpin, HIGH);
          delay(100);
          digitalWrite(Gpin, LOW);
          delay(50);
        }
        for (short i = 0; i < 4; i++)    // Indicates Device StartUp
        {
          digitalWrite(Rpin, HIGH);
          delay(100);
          digitalWrite(Rpin, LOW);
          delay(50);
        }
        for (short i = 0; i < 4; i++)    // Indicates Device StartUp
        {
          digitalWrite(Bpin, HIGH);
          delay(100);
          digitalWrite(Bpin, LOW);
          delay(50);
        }
        break;
      }
    }
    SwitchCounter = 0;
  } 
}
