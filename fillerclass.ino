int SensorFiller = A0; 

byte pinc02 = 9;
byte pinbuch = 8;
byte pinpurge = 10;
byte FillerVertical = 12;
byte pinactuator = 13 ;
 

int BottleCounter = 0; 
int SensorFillerValue = 0; 
int timer_bottle = 0;

/*Filler class definition */ 

class Filler {

private: 
    byte pinc02; 
    byte pinbuch; 
    byte pinpurge; 
    byte pinvertical;

public: 
    Filler(byte pinc02, byte pinbuch, byte pinpurge, byte pinvertical) { 
    this->pinc02 = pinc02; 
    this->pinbuch = pinbuch; 
    this->pinpurge = pinpurge; 
    this->pinvertical = pinvertical;

    pinMode(pinc02, OUTPUT);
    pinMode(pinbuch, OUTPUT);
    pinMode(pinpurge, OUTPUT);
    pinMode(pinvertical, OUTPUT);
}
  
 void up(){
    digitalWrite(pinvertical, HIGH);   
     
 }
 void down(){
    digitalWrite(pinvertical, LOW);      
 }
 void c02on(){
    digitalWrite(pinc02, LOW); 
 }
 void c02off(){
    digitalWrite(pinc02, HIGH);
 }
 void buchon(){
    digitalWrite(pinbuch, LOW);
 }
 void buchoff(){
    digitalWrite(pinbuch, HIGH);
 }
 void purgeon(){
    digitalWrite(pinpurge, HIGH);
 }
 void purgeoff(){
    digitalWrite(pinpurge, LOW);
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
    pinMode(pinsensor, INPUT); }

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
    byte pinactuator; 
    int FillerValue_1 = 700; 
    int FillerValue = 0;

  public: 
    Actuator(byte pinactuator) { 
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

Filler filler=Filler(pinc02, pinbuch, pinpurge, FillerVertical); 
Bottle_detector bottle_detector=Bottle_detector(SensorFiller); 
Actuator actuator=Actuator(13);

void setup() { // put your setup code here, to run once: Serial.begin(9600);
  filler.c02off();
  filler.buchoff();
  actuator.close(); 
  filler.up(); }

void loop() {

  // put your main code here, to run repeatedly:

  bottle_detector.on();
  int tmp = bottle_detector.is_bottle();

  if(tmp ==0){ 
    Serial.println("waiting for bottle"); 
    }
  else{ 
    Serial.println("bottle"); 
   
    
    delay(1000);
    filler.down();
    delay(500);
    filler.c02on();
    delay(2000);
    filler.c02off(); 
    filler.buchon(); 
    delay(55000);
    filler.buchoff();
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
