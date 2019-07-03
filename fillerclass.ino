int SensorFirstBottle = A0;
int SensorLastBottle = A3;
const byte SensorCounterPin = 2;

byte pinactuator = 13 ;

byte pinc02_1 = 7 ;
byte pinbuch_1 = 8 ;
byte pinpurge_1 = 9 ;

byte pinc02_2 = 4 ;
byte pinbuch_2 = 5 ;
byte pinpurge_2 = 6 ;

int FillerVertical = 12; 

int BottleCounter = 0;
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
     void start_heads_cycle(){
      //head1.c02on();
     }

};

/*Bottle_counter class definition */
class Bottle_counter {

  private:
    byte bot_count = 0 ;
    byte pinsensor;
    long duration;
 
  public: 
    Bottle_counter(byte pinsensor) {
      this->pinsensor = pinsensor;   
      pinMode(pinsensor, INPUT);
      }
      
     byte num_bottle() {
      return bot_count;       
     }
     void reset_bottle(){
        bot_count=0;       
      } 

     void increase_count(){
        bot_count ++;
     }
     /*byte on(){
     while(bot_count<4){
      duration = pulseIn(pinsensor, HIGH);
      if (duration >200 && duration < 2000){
        bot_count ++; 
        Serial.println("bottle");
      }
     } */   
   
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
    byte bot_det = 0 ;
    byte pinactuator ;
    int FillerValue_1 = 700;
    int FillerValue = 0;    

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
    Serial.println("c02 on");
    delay(2000);
    head1.c02off();
    head2.c02off();
    Serial.println("c02 off");
    head1.buchon();
    head2.buchon();
    Serial.println("buch on");
    delay(30000);
    head1.buchoff();
    head2.buchoff();
    Serial.println("buch off");

  
}

void increase_count(){
    Serial.println("one more bottle");
    BottleCounter ++;
    Serial.println(BottleCounter);

}

void bottle_interrupt_handler(){
 static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time = millis();
 // If interrupts come faster than 200ms, assume it's a bounce and ignore
 if (interrupt_time - last_interrupt_time > 1000)
 {
  increase_count();
 }
 last_interrupt_time = interrupt_time;
} 

 
 Actuator actuator=Actuator(pinactuator);
 Bottle_counter bottle_counter=Bottle_counter(SensorCounterPin);

 Head head1=Head(pinc02_1, pinbuch_1, pinpurge_1); 
 Head head2=Head(pinc02_2, pinbuch_2, pinpurge_2); 

 Filler filler=Filler(head1,head2, FillerVertical);
 
 Bottle_detector first_bottle_detector=Bottle_detector(SensorFirstBottle);
 Bottle_detector last_bottle_detector=Bottle_detector(SensorLastBottle);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SensorCounterPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SensorCounterPin), bottle_interrupt_handler, CHANGE);

  actuator.close();
  filler.up();
  //bottle_counter.on();
  Serial.println("set up finished");
}

void loop() {
  
  // put your main code here, to run repeatedly:

  Serial.println("iteration on loop");
 
  delay (1000);
  if(BottleCounter == 4){
    Serial.println("Start filling cycle");
    delay(2000);
    filler.down();
    start_cycle_heads(head1, head2);
    filler.up();
    delay(1000);   
    actuator.open();
    delay(2000);
    first_bottle_detector.reset_bottle();
    last_bottle_detector.reset_bottle();
    actuator.close();
    delay(2000);
    BottleCounter = 0;  

  }/*  
      first_bottle_detector.on();
      last_bottle_detector.on();

      if(first_bottle_detector.is_bottle() && last_bottle_detector.is_bottle()){
        Serial.println("bottle");
        delay(2000);
        filler.down();
        start_cycle_heads(head1, head2);
        filler.up();
        delay(1000);   
        actuator.open();
        Serial.println("actuator open");
        delay(2000);
        first_bottle_detector.reset_bottle();
        last_bottle_detector.reset_bottle();
        actuator.close();
        Serial.println("actuator closed");
      }
      else{
        Serial.println("waiting for bottle");
      }  
     
  }
  */
}
