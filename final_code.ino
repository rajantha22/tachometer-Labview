int timer1_counter;
volatile int IRQcount;//counts of the interrupt.
int pin = 2; //digital read pin of the TCRT 5000
int pin_irq = 7; //motor analog write pin
int count_old=0; //previous position
int count=0; //present count of the interuupt
int rpm=0;//RPM value
char string;
int data=0;
int sensorPin = A1; // or A0, A2, A3... all of them
int sensorValue=0; //initializing the variables
int volt=0;
double volt_out = 0;
double devided_d = 0;
//starting voltage
void setup() {
 // put your setup code here, to run once:
 Serial.begin (9600);
 pinMode(7,OUTPUT);
 attachInterrupt(digitalPinToInterrupt(3), IRQcounter, RISING); //attaach interrupt to code
//pins
 pinMode(12,OUTPUT);
 pinMode(13,OUTPUT);l
 pinMode(4,OUTPUT);
 //pins
 //pinMode(A3,INPUT);
 //Timer conigurations
 TCCR1A = 0;
 TCCR1B = 0;
 // Set timer1_counter to the correct value for our interrupt interval
 //timer1_counter = 64911; // preload timer 65536-16MHz/256/100Hz
 //timer1_counter = 64286; // preload timer 65536-16MHz/256/50Hz
 timer1_counter = 34286; // preload timer 65536-16MHz/256/2Hz
 //timer1_counter = 59286;
 TCNT1 = timer1_counter; // preload timer
 TCCR1B |= (1 << CS12); // 256 prescaler
 TIMSK1 |= (1 << TOIE1); // enable timer overflow interrupt
 interrupts(); // enable all interrupts
}
//this is the interrupt terigger
void IRQcounter() {
 IRQcount++;//
}
void loop() {
 sensorValue = analogRead (sensorPin);
 volt =map(sensorValue,0,1023,0,255);
 analogWrite(4,volt);

 volt_out=(double)volt/255*5.0;
 devided_d = rpm/volt_out;

 Serial.print(abs(rpm));
 Serial.print(",");
 Serial.print(volt_out);
 Serial.print(",");
 Serial.println(devided_d);

if(abs(rpm)>15000){
 digitalWrite(12,HIGH);
 digitalWrite(13,LOW);
 }else{
 digitalWrite(13,HIGH);
 digitalWrite(12,LOW);
 }


 }
//timmer interupt
ISR(TIMER1_OVF_vect) // interrupt service routine
//every 500ms this will be triggered and the rpm will be counted
{
 TCNT1 = timer1_counter; // preload timer
 int count=IRQcount;
 rpm=(count-count_old)*60;
 count=count_old;
 count=0;
 count_old=0;
//Serial.println(IRQcount);
 IRQcount=0;
}
