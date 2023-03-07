/**  Arduino Binary Clock & Stopwatch Project           **/
/**  Part of Embedded Lab Project                       **/
/**  Full project:                                      **/

/* Variable declartion and Pin configrations*/
struct Time {  
  uint8_t Hour;
  uint8_t Minute;
  uint8_t Second;
};

Time Clock     = { 0, 0, 0 };  // initialize clock instance
Time StopWatch = { 0, 0, 0 };  // initialize stopwatch instance

int LedSeconds[] = {7,8,9,10,11,12}; // array for seconds leds
int LedMinutes[] = {0,2,3,4,5,6};     // array for Mins leds 
int LedHours[]   = {A1,A2,A3,A4,A5};    // array for hours leds

const int ModeSelect = A0;  // the button to select mode between clock & stopwatch 
const int FunSelect  = 13;   // the button to pause, resume and reset the stopwatch 
int count1, newcount1 = 0;  // for simulating switch counts
int count2, newcount2 = 0;  // for simulating switch counts


bool Modeclock  = false;     // run selected mode clock 
bool ModeSW  = false; // run selected mode   stopwatch
bool TimeRun   = true; // display time in binary on leds

int nBitsSeconds;   // for bit conversion of seconds value
int nBitsMinutes;   // for bit conversion of mins value
int nBitsHours;     // for bit conversion of hours value


// Pin cofigration
void setup(void)
{

   nBitsSeconds = sizeof(LedSeconds)/sizeof(LedSeconds[0]) ;
   nBitsMinutes = sizeof(LedMinutes)/sizeof(LedMinutes[0]);
   nBitsHours = sizeof(LedHours)/sizeof(LedHours[0]);

  for (byte i = 0; i < nBitsSeconds; i++) {
    pinMode(LedSeconds[i], OUTPUT);
  }

  for (byte i = 0; i < nBitsMinutes; i++) {
    pinMode(LedMinutes[i], OUTPUT);
  }

  for (byte i = 0; i < nBitsHours; i++) {
    pinMode(LedHours[i], OUTPUT);
  }
  
  pinMode(ModeSelect, INPUT);
 
  
  pinMode(FunSelect, INPUT);
 

  Serial.begin(9600);
 
  Serial.println("/***  Arduino based Binary clock and Stop watch!  ***/");
  Serial.println("--> Please select the mode to start!");
}

/*  the main routine*/
void loop(void)
{

  /* mode selection clock, stopwatch and idle mode */
  if(digitalRead(A0) == HIGH){
    newcount1 = count1 + 1;
    if(newcount1!= count1){

      switch(newcount1){
        case 1: Modeclock = true;  // for clock mode to operate
          ModeSW = false;
          TimeRun = true;
          Serial.println("Mode: Clock");
          Clock = {0,0,0};
          count2 = 0;
          newcount2 = 0;
        break;
        case 2: ModeSW = true;     // for stopwatch mode to operate
          TimeRun = true;
          Modeclock = false;
          Serial.println("Mode: Stopwatch");
          StopWatch = { 0, 0, 0 };
        break;
        default:                  // do nothing
          Modeclock = false;
          ModeSW = false;
          StopWatch = { 0, 0, 0 };
          TimeRun = true;
          newcount1 = 0;
          count2 = 0;
          newcount2 = 0;
        Serial.println("Mode: Idle!, Please select mode to start again");
        break;
      }
       count1 = newcount1;      
    }
  }

  /*  function selection in stopwatch i.e., pause,resume and reset */
  if(digitalRead(13)== HIGH){
    newcount2 = count2 + 1;
    if(newcount2!= count2){
     if (ModeSW){ 
        switch(newcount2){
          case 1: TimeRun = false;   //
           Serial.println("Stopwatch paused!!!");
          break;
          case 2: TimeRun = true;
          Serial.println("Stopwatch resumed!!!");
          break;
          default:
          StopWatch = { 0, 0, 0 };
          TimeRun = true;
          newcount2 = 0;
          Serial.println("Stopwatch resetted!!!");
          break;
      }
      
      }
      count2 = newcount2;
    }
  }

  /* main routine works based on mode and function selections*/
  if (Modeclock ){                                                  //clock mode
    Tick(Clock);                                           
    Clock.Hour = (Clock.Hour + StopWatch.Hour) % 24;               
    Clock.Minute = (Clock.Minute + StopWatch.Minute ) % 60;          
    Clock.Second = (Clock.Second + StopWatch.Second) % 60;             
    if (TimeRun) {                                         
      displayBinarySeconds(Clock.Second);
      displayBinaryMinutes(Clock.Minute);
      displayBinaryHours(Clock.Hour);
    }
  } else if(ModeSW) {                                        //stopwatch mode
    Tick(StopWatch); 
    StopWatch.Hour = (StopWatch.Hour) % 24;               
    StopWatch.Minute = (StopWatch.Minute) % 60;          
    StopWatch.Second = ( StopWatch.Second) % 60;                                       
     if (TimeRun) {                                         //display time
      displayBinarySeconds(StopWatch.Second);
      displayBinaryMinutes(StopWatch.Minute);
      displayBinaryHours(StopWatch.Hour);
    }
  }else{

    
  }
  delay(1000);   // 1000 milliseconds = approx. 1 second delay
}

// displays time seconds, minutes and hours
void displayBinarySeconds(byte nSec)
{
  for (byte i = 0; i < nBitsSeconds; i++) {
    digitalWrite(LedSeconds[i], nSec & 1);
    nSec /= 2;
  }
}

void displayBinaryMinutes(byte nMin)
{
  for (byte i = 0; i < nBitsMinutes; i++) {
    digitalWrite(LedMinutes[i], nMin & 1);
    nMin /= 2;
  }
}

void displayBinaryHours(byte nHr)
{
  for (byte i = 0; i < nBitsHours; i++) {
    digitalWrite(LedHours[i], nHr & 1);
    nHr /= 2;
  }
}

// Calculates time, increments secends,minutes and hours counters
void Tick(struct Time &Temp){
  Temp.Second = (Temp.Second + 1);
  if (Temp.Second > 59)
  {
    Temp.Second = 0;
    Temp.Minute = (Temp.Minute + 1);
    if (Temp.Minute > 59)
    {
      Temp.Minute = 0;
      Temp.Hour = (Temp.Hour + 1);
      if (Temp.Hour > 23)
      {
        Temp.Hour = 0;
        Temp.Minute = 0;
        Temp.Second = 0;
      }
    }
  }
}

