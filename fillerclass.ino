int SensorFiller = A0;
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
    byte pinvertical;
    
  public: 
    Filler(Head thehead, byte pinvertical) {
      
      this->pinvertical = pinvertical;
      this->thehead.pin
   

      pinMode(pinvertical, OUTPUT);
    }
      
     void up(){
      digitalWrite(pinvertical, HIGH);      
     }
     void down(){
      digitalWrite(pinvertical, LOW);      
     } 
     void start_heads_cycle(){
      //head1.c02on();
      
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
    byte bot_det = 0 ;
    byte pinactuator =13;
    int FillerValue_1 = 700;
    int FillerValue = 0;    

  public: 
    actuator() {
      this->pinactuator = pinactuator;   
      pinMode(pinactuator, OUTPUT);
      }
      
     void open() {
      digitalWrite(pinactuator, LOW);       
     }
      
     void close() {
      digitalWrite(pinactuator, HIGH);
       
     }
};



 Head head1=Head(pinc02_1, pinbuch_1, pinpurge_1); 
 Head head2=Head(pinc02_2, pinbuch_2, pinpurge_2); 

 Filler filler=Filler(head1, FillerVertical);
 Bottle_detector bottle_detector=Bottle_detector(SensorFiller);
 Actuator actuator=Actuator();


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
 
 actuator.close();
 filler.up();

}

void loop() {
  
  // put your main code here, to run repeatedly:
  
  bottle_detector.on(); 
  int tmp = bottle_detector.is_bottle();
  if(tmp ==0){
    Serial.println("waiting for bottle");
  }
  else{
    Serial.println("bottle");
    delay(2000);
    filler.down();
    delay(5000);
    filler.up();
    delay(1000);   
    actuator.open();
    Serial.println("actuator open");
    delay(2000);
    bottle_detector.reset_bottle();
    actuator.close();
    Serial.println("actuator closed");
  }


  
}
