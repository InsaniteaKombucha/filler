int SensorFirstBottle = A0;
int SensorLastBottle = 2;

int BottlesInFiller = 3;

//const byte SensorCounterPin = A3;
byte pinConveyor = 3;

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


void start_cycle_heads(Head head1, Head head2){

    head1.c02on();
    head2.c02on();
    Serial.println("c02 ofn");
    delay(2000);
    head1.c02off();
    head2.c02off();
    Serial.println("c02 off");
    head1.buchon();
    head2.buchon();
    Serial.println("buch on");
    delay(15000);
    head1.buchoff();
    head2.buchoff();
    Serial.println("buch off");
  
}

void increase_count(){
   // Serial.println("one more bottle");
    BottleCounter ++;
   // Serial.println(BottleCounter);

}

void increase_last_count(){
    lastBottleCounter ++;
    Serial.println(BottleCounter);

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


void last_bottle_handler(){
     static unsigned long last_interrupt_time = 0;
     unsigned long interrupt_time = millis();
     Serial.println("I am inside the handler");
 
     // If interrupts come faster than 500ms, assume it's a bounce and ignore
     if (interrupt_time - last_interrupt_time > 500)
     {
       increase_last_count();
     }
     last_interrupt_time = interrupt_time;
} 

 
 Actuator actuator=Actuator(pinactuator);

 Head head1=Head(pinc02_1, pinbuch_1, pinpurge_1); 
 Head head2=Head(pinc02_2, pinbuch_2, pinpurge_2); 

 Filler filler=Filler(head1,head2, FillerVertical);
 
 Bottle_detector first_bottle_detector=Bottle_detector(SensorFirstBottle);
 Conveyor conveyor=Conveyor(pinConveyor);

 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(pinactuator, OUTPUT);

  pinMode(SensorLastBottle, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SensorLastBottle), bottle_interrupt_handler, CHANGE);

  //pinMode(SensorLastBottle, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(SensorLastBottle), last_bottle_handler, CHANGE);
  conveyor.on();

  actuator.close();
  delay(3000); 
 
  filler.up();
  lastBottleCounter = 0;

  Serial.println("set up finished");
  
}

void loop() {  
  // put your main code here, to run repeatedly:

  Serial.println("BOTTLE #");
  Serial.println(BottleCounter);
 
  delay (1000);
  if(BottleCounter == BottlesInFiller ){
    Serial.println(BottleCounter);
   
    delay(2000);
    conveyor.off();
    filler.down();
    start_cycle_heads(head1, head2);
    filler.up();
    actuator.open();
    delay(500);
    conveyor.on(); 
    delay(2000);
    actuator.close();
    delay(2000);
    BottleCounter = 0;  
    lastBottleCounter=0;

  }

  Serial.println(BottleCounter);
}
