
int SensorFirstBottle = A0;
int SensorLastBottle = A1;

int SensorCounterBottle = 2;

int BottlesInFiller = 3;

//const byte SensorCounterPin = A3;
byte pinConveyor = 3;

static unsigned long start_millis_filler_down = 0;
static unsigned long last_interrupt_time = 0;
static unsigned long buch_timer_head1 = 0;

byte pinactuator = 13 ;

byte pinc02_1 = 8 ;
byte pinbuch_1 = 9 ;
byte pinpurge_1 = 5 ;

byte pinc02_2 = 6 ;
byte pinbuch_2 = 7 ;
byte pinpurge_2 = 10 ;

int FillerVertical = 12; 

int BottleCounter = 0;
int lastBottleCounter =0;
int SensorFillerValue = 0;
int timer_bottle = 0;

/* class definition */
class Head{
  private:
    byte pinc02;
    byte pinbuch;
    byte pinpurge;
  public:
    Head(byte pinc02, byte pinbuch, byte pinpurge) {
      this->pinc02 = pinc02;
      this->pinbuch = pinbuch;
      this->pinpurge = pinpurge;
     
      pinMode(pinc02, OUTPUT);
      pinMode(pinbuch, OUTPUT);
      pinMode(pinpurge, OUTPUT);
     
     }
    void c02on(){
      digitalWrite(pinc02, HIGH); 
     }
     void c02off(){
      digitalWrite(pinc02, LOW);
     }
     void buchon(){
      digitalWrite(pinbuch, HIGH);
     }
     void buchoff(){
        digitalWrite(pinbuch, LOW);
     }
     void purgeon(){
      digitalWrite(pinpurge, HIGH);
     }
     void purgeoff(){
      digitalWrite(pinpurge, LOW);
     }

};

/*Filler class definition */
class Filler {

  private:
    byte pinfiller;
   
  public: 
    Filler(Head head1, Head head2, byte pinfiller) {
      
      this->pinfiller = pinfiller;
     // this->head1 = head1;
      pinMode(pinfiller, OUTPUT);
    }
      
     void up(){
      digitalWrite(pinfiller, HIGH);      
     }
     void down(){
      digitalWrite(pinfiller, LOW);      
     } 
};

/*Conveyor class definition */
class Conveyor {

  private:
    byte pinConveyor;
   
  public: 
    Conveyor( byte pinConveyor) {
      
      this->pinConveyor = pinConveyor;
      pinMode(pinConveyor, OUTPUT);
    }
      
     void on(){
      digitalWrite(pinConveyor, HIGH);  
      Serial.println("conveyor on");        
     }
     void off(){
      digitalWrite(pinConveyor, LOW);            
      Serial.println("conveyor off");     
     } 

};

/*Bottle_detector class definition */
class Bottle_detector {

  private:
    byte bot_det = 0 ;
    byte pinsensor;
    int FillerValue_1 = 700;
    int FillerValue = 0;    

  public: 
    Bottle_detector(byte pinsensor) {
      this->pinsensor = pinsensor;   
      pinMode(pinsensor, INPUT);
      }
      
     byte is_bottle() {
      return bot_det;       
     }
     void reset_bottle(){
        bot_det=0;       
      } 
      
     byte on(){
      FillerValue = analogRead(pinsensor);
      
      if (FillerValue_1 > 200 && FillerValue >200) {   
          bot_det = 0 ;
          FillerValue_1 = FillerValue;
      }
      if (FillerValue_1 < 200 && FillerValue <200) {   
          bot_det = 1 ;
          FillerValue_1 = FillerValue;
      }
      if (FillerValue_1 > 200 && FillerValue < 200) {   
          FillerValue_1 = FillerValue;
      }
      if (FillerValue_1 < 200 && FillerValue >200) {   
          FillerValue_1 = FillerValue;
      }
    
     }

};

class Actuator {

  private:
    byte pinactuator ;

  public: 
    Actuator(byte pinactuator) {
      this->pinactuator = pinactuator;   
      pinMode(pinactuator, OUTPUT);
      }
      
     void open() {
      digitalWrite(pinactuator, LOW);  
      Serial.println("actuator open");     
     }
      
     void close() {
      digitalWrite(pinactuator, HIGH);
      Serial.println("actuator close");      
     }
};


void increase_count(){
    BottleCounter ++;
}


void bottle_interrupt_handler(){
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    // If interrupts come faster than 1000ms, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time > 1000)
    {
      increase_count();
    }
    last_interrupt_time = interrupt_time;
} 
 
 void close_Actuator(){

 }


enum States
{
  StateStart, StopConveyor, FillerDown, Openc02_head1, Openc02_head2, Openc02_head3, Closec02_head1, Closec02_head2, Closec02_head3, Opencbuch_head1, Opencbuch_head2, Openbuch_head3, Closebuch_head1, Closebuch_head2, Closebuch_head3,   FillerUp, ActuatorOpen, ActuatorClose, StartConveyor
};

 //at power up, we will start out in this machine state (mState)
/*byte mState = 1;

void StateMachine(){
   Serial.println("state");
   Serial.println(mState);

  switch (mState)
  {

     unsigned long start_millis_filler_down;

    case 1:
      int FirstCycle = 1;
      Serial.println("State machine 1");
      mState = 2; //now switching to State1
      break;      
      
    case 2:
      Serial.println("State machine 1");
      Serial.println("conveyor off");  
         start_millis_filler_down = millis();
         mState = 3; 
        break;
      
    case 3:  
      unsigned long current_millis_filler_down = millis(); 

         Serial.println(current_millis_filler_down);
         Serial.println(start_millis_filler_down);
      if (current_millis_filler_down - start_millis_filler_down > 2000)
      {
//        filler.down();
           Serial.println("filler down");  
           mState = Openc02_head1;
      }     

      break;
    case Openc02_head1:

     if (FirstCycle)
      {   Serial.println("open c02");  
//        head1.Closec02();
        static unsigned long last_interrupt_time = millis();
        mState = Closec02_head1;
      } 
            
    case Closec02_head1:

      unsigned long interrupt_time = millis();

      if (interrupt_time - last_interrupt_time > 1000)
      {
//        head1.closec02();
        last_interrupt_time = interrupt_time; 
        Serial.println("close c02");
        mState = Opencbuch_head1;
     
      }

      break;
     
     
    case Opencbuch_head1:
      if (FirstCycle)
      {
//      head1.openbuch();
        static unsigned long buch_timer_head1 = millis();
        Serial.println("open buch");
        mState = Closebuch_head1;  
      }
      
     
    case Closebuch_head1:
      
      unsigned long buch_filling = millis();

      if (buch_filling - buch_timer_head1 > 10000)
      {
//        head1.closec02();
          Serial.println("close buch");
          mState = FillerUp;  
      }
       
      
     
    case FillerUp:
      static unsigned long start_millis = 0;
      unsigned long current_millis = millis();    
      if (current_millis - start_millis > 2000)
      {
         Serial.println("filler up");  
         mState = ActuatorOpen;
      }                   
     
     
    case ActuatorOpen:
      if (1)
      {
           Serial.println("actuator open");  
//        actuator.open();
      }                   
      mState = StartConveyor;
     
    case StartConveyor:
      if (1)
      {
      // conveyor.stop();
      }                   
      mState = ActuatorClose;
      break;  

    case ActuatorClose:
      if (1)
      {   
           Serial.println("actuator close");  
//        actuator.close();
      }                   
      mState = StateStart;
      break;  

    default:
      // default code goes here
      mState = 1;
      break;

  } 
} 
*/



 
 Actuator actuator=Actuator(pinactuator);

 Head head1=Head(pinc02_1, pinbuch_1, pinpurge_1); 
 Head head2=Head(pinc02_2, pinbuch_2, pinpurge_2); 

 Filler filler=Filler(head1,head2, FillerVertical);
 
 Bottle_detector first_bottle_detector=Bottle_detector(SensorFirstBottle);
 Bottle_detector last_bottle_detector=Bottle_detector(SensorLastBottle);
 Conveyor conveyor=Conveyor(pinConveyor);

 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(pinactuator, OUTPUT);

  pinMode(SensorCounterBottle, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SensorCounterBottle), bottle_interrupt_handler, CHANGE);
  
  conveyor.on();
  actuator.close();
  filler.up();
  lastBottleCounter = 0;
  Serial.println("set up finished");
  
}

void loop() {  
  // put your main code here, to run repeatedly:

//  Serial.println("BOTTLE #");
  Serial.println(BottleCounter);
 
//  if(BottleCounter == BottlesInFiller && first_bottle_detector.is_bottle() == 1 && last_bottle_detector.is_bottle() == 1){

//  if(BottleCounter == BottlesInFiller){

     StateMachine(); 
//     Serial.println("inside the loop");
  
 // }
}
