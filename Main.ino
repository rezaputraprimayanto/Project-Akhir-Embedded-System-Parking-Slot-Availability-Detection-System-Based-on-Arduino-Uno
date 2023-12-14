#include <Servo.h> 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const int buttonPin = 1;   
const int buzzerPin = 3;   
int ultmasuk = 12;
int ultkeluar = 8;
int err = 0;

LiquidCrystal_I2C lcd(0x3f, 16, 2); //LCD
Servo Servomasuk; // servo masuk
Servo Servokeluar; // servo keluar

#define ult_enter 11 // Echo
#define ult_out 10  // Echo

#define ir_car1 5
#define ir_car2 6
#define ir_car3 7
#define opbut 0

int S1 = 0, S2 = 0, S3 = 0; 
int slot = 3;
int pos1 = 0;    
int pos2 = 0;     
int total = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ir_car1, INPUT); // slot parkir 1
  pinMode(ir_car2, INPUT); // slot parkir 2
  pinMode(ir_car3, INPUT); // slot parkir 3
  pinMode(ult_enter, INPUT);
  pinMode(ult_out, INPUT);
  pinMode(ultmasuk, OUTPUT);
  pinMode(ultkeluar, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(opbut, INPUT_PULLUP);
  
  Servomasuk.attach(2); 
  Servokeluar.attach(4);

  lcd.backlight();
  lcd.begin();
  lcd.setCursor(0, 0);
  lcd.print("Pendeteksi Slot Parkir");
  lcd.setCursor(0, 1);
  lcd.print("   KELOMPOK 2  ");
  for (int i = 1; i <= 23; i++) {
    lcd.scrollDisplayLeft();
    delay(500);
  }
  delay(1000);
  lcd.clear();
  Read_Sensor();
}

void loop() {
  Read_Sensor();
  digitalWrite(ultmasuk,HIGH);
  delay(50);
  digitalWrite(ultmasuk,LOW);
  float waktu = pulseIn(ult_enter, HIGH);
  int jarak = waktu*0.032/2;
  //Serial.print(jarak);
  //Serial.println("  cm");
  if(jarak <= 6 && slot > 0){
    delay(1000);
    open_servomasuk();
  }else{
    close_servomasuk();
  }

  int buttonState = digitalRead(buttonPin);

  // Memeriksa jika tombol ditekan
  if (buttonState == HIGH) {
    // Memutar bunyi buzzer
    digitalWrite(buzzerPin, 1000);
  } else {
    // Mematikan bunyi buzzer
    digitalWrite(buzzerPin, 0);
  }

  digitalWrite(ultkeluar,HIGH);
  delay(50);
  digitalWrite(ultkeluar,LOW);
  float waktu2 = pulseIn(ult_out, HIGH);
  int jarak2 = waktu2*0.032/2;
  //Serial.print(jarak2);
  //Serial.println("  cm");
  if(jarak2 <= 6){
    open_servokeluar();
    delay(1000);
  }else{
    close_servokeluar();
  }

  err = digitalRead(opbut);
  if (err == LOW){
    open_servokeluar();
  }

  hitung_slot();

  lcd.setCursor(0, 0);
  lcd.print("Slot Tersedia: ");
  lcd.print(slot);

  lcd.setCursor(0, 1);
  if (S1 == 1) {
    lcd.print("S1:I ");
  } else {
    lcd.print("S1:X ");
  }
  lcd.setCursor(5, 1);
  if (S2 == 1) {
    lcd.print("S2:I ");
  } else {
    lcd.print("S2:X ");
  }
  lcd.setCursor(10, 1);
  if (S3 == 1) {
    lcd.print("S3:I ");
  } else {
    lcd.print("S3:X ");
  }
   Serial.println(slot);
}

void hitung_slot(){
  if (S1 == 1 && S2 == 1 && S3 == 1){
    slot = 0;
  }
  if (S1 == 0 && S2 == 0 && S3 == 0){
    slot = 3;
  }
  if ((S1 == 1 && S2 == 1 && S3 == 0)||(S1 == 1 && S2 == 0 && S3 == 1)||(S1 == 0 && S2 == 1 && S3 == 1)){
    slot = 1;
  }
  if ((S1 == 1 && S2 == 0 && S3 == 0)||(S1 == 0 && S2 == 0 && S3 == 1)||(S1 == 0 && S2 == 1 && S3 == 0)){
    slot = 2;
  }
}

void Read_Sensor() {
  S1 = 0, S2 = 0, S3 = 0;
  if (digitalRead(ir_car1) == 0){
    S1 = 1;
  
  }
  if (digitalRead(ir_car2) == 0){
    S2 = 1;
  
  }
  if (digitalRead(ir_car3) == 0){
    S3 = 1;
  }
  
}

void open_servomasuk(){
    Servomasuk.write(90);
}
void open_servokeluar(){
    Servokeluar.write(90);
}
void close_servokeluar(){
    Servokeluar.write(0);
}
void close_servomasuk(){
    Servomasuk.write(180);
}
