volatile     long t =0;
volatile     long t0=0xffff;
volatile     long t1=0;
volatile     long tw=0;
volatile       long tp=0;

volatile   unsigned long risingEdges=0;
volatile   unsigned long fallingEdges=0;

const volatile int PIN_INPUT = 2;
const volatile int PIN_START_TRIGGER = 3;
const volatile int PIN_STOP_TRIGGER = 4;
const volatile int PIN_PSO_INVERSE = 5;
 
const volatile int RXLED = 17;
 
const volatile int ACTIVE = LOW;
int HoldTime = 1000; //µs


void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
   
   pinMode(RXLED, OUTPUT);  // Set RX LED as an output
   pinMode(PIN_START_TRIGGER, OUTPUT);
   pinMode(PIN_STOP_TRIGGER, OUTPUT);
   pinMode(PIN_PSO_INVERSE,OUTPUT);
   pinMode(PIN_INPUT, INPUT);//INPUT_PULLUP
   
   attachInterrupt(digitalPinToInterrupt(PIN_INPUT), ISR_onInput, CHANGE);

   HoldTime = 1000;
   t = 0;
   t0 = 0;
   t1 = 0;
   risingEdges =0;
   tw =0 ;
   TXLED0;
}

void ISR_onInput()
{
t = micros();
if( ACTIVE ==  digitalRead(PIN_INPUT))
{

  tp = t-t0; // frequency
  t0 = t;  
  
  digitalWrite(PIN_START_TRIGGER,HIGH);
  digitalWrite(PIN_STOP_TRIGGER,LOW);
  digitalWrite(PIN_PSO_INVERSE,!ACTIVE);
  digitalWrite(RXLED, LOW); // turn LED on
  
  risingEdges++;
}
else
{
   tw = t - t0;  // pulse width
   fallingEdges++;
   digitalWrite(PIN_PSO_INVERSE,ACTIVE);

} 

}

void loop() {
  // put your main code here, to run repeatedly:
  //noInterrupts();  
  t1 =  micros();
  //interrupts();  

  if (risingEdges > 1 &&  fallingEdges > 1 && (t1-t0) > 0 &&   (t1 - t0) > 1.5*tp   )
  { 
    noInterrupts();
    digitalWrite(PIN_START_TRIGGER,LOW);    
    digitalWrite(PIN_STOP_TRIGGER,HIGH);
    digitalWrite(RXLED, HIGH);
    
      if(risingEdges !=0)
      {
          
        Serial.print(t1-t0);
        Serial.print(" ");
        Serial.print(tp);
        Serial.print(" ");
        Serial.print(tw); 
        Serial.print(" "); 
        Serial.print(risingEdges);  
        Serial.print(" ");
        Serial.println(fallingEdges);  
        
      }
    risingEdges = 0;
    fallingEdges = 0;
    t =0;
    t0=0xffff;
    t1=0;
    tw=0;
    tp=0;
    interrupts();
  }
  else
  {

  }
}
