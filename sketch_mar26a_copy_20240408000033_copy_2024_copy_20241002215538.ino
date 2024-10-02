#include <DHT.h>
#include <Servo.h>

#define DHTPIN 11          
#define DHTTYPE DHT11      

#define SERVO_PIN 12       

#define DISP_A_PIN 4       
#define DISP_B_PIN 5
#define DISP_C_PIN 6
#define DISP_D_PIN 7
#define DISP_E_PIN 8
#define DISP_F_PIN 9
#define DISP_G_PIN 10
#define DISP_DP_PIN 3
#define DISP_DIGIT_PIN 2  

#define DHT_BUTTON_PIN A0  
#define DISP_BUTTON_PIN A1 

DHT dht(DHTPIN, DHTTYPE); 
Servo servo;              

int dhtDisplayMode = 0;    
int dispDisplayMode = 0;   
int lastDhtButtonState = HIGH;  
int lastDispButtonState = HIGH; 
unsigned long lastDhtDebounceTime = 0; 
unsigned long lastDispDebounceTime = 0; 
unsigned long lastPressTime = 0;        
int pressCount = 0;                      
bool servoState = false;                
int servoAngle = 0;                     
float pastTemperature[24]; 
float pastHumidity[24];    
unsigned long lastLogTime = 0;  // Last time data was logged
const unsigned long logInterval = 60000; // Logging interval: 1 minute

void displayLast24HoursData();
void displayLast24HoursByHours();
void displayHour(int hour);

void setup() {
  Serial.begin(9600);
  dht.begin();
  servo.attach(SERVO_PIN);
  pinMode(DHT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(DISP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(DISP_A_PIN, OUTPUT);
  pinMode(DISP_B_PIN, OUTPUT);
  pinMode(DISP_C_PIN, OUTPUT);
  pinMode(DISP_D_PIN, OUTPUT);
  pinMode(DISP_E_PIN, OUTPUT);
  pinMode(DISP_F_PIN, OUTPUT);
  pinMode(DISP_G_PIN, OUTPUT);
  pinMode(DISP_DP_PIN, OUTPUT);
  pinMode(DISP_DIGIT_PIN, OUTPUT);

  for (int i = 0; i < 24; i++) {
    pastTemperature[i] = 20.0;
    pastHumidity[i] = 40.0;
  }
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (temperature > 30 || humidity < 20) {
    servoAngle = 180;
    servo.write(servoAngle);
    servoState = true;
  } else {
    servoAngle = 0;
    servo.write(servoAngle);
    servoState = false;
  }

  int dhtButtonState = digitalRead(DHT_BUTTON_PIN);
  if (dhtButtonState == LOW && millis() - lastPressTime > 1000) {
    lastPressTime = millis();
    dhtDisplayMode = (dhtDisplayMode + 1) % 3; 
  }

  int dispButtonState = digitalRead(DISP_BUTTON_PIN);
  if (dispButtonState != lastDispButtonState) {
    if (dispButtonState == LOW) {
      pressCount++;
    }
    lastDispButtonState = dispButtonState;
  }

  updateDisplay(temperature, humidity, pressCount);

  // Log temperature and humidity data every minute
  unsigned long currentMillis = millis();
  if (currentMillis - lastLogTime >= logInterval) {
    lastLogTime = currentMillis;
    logData(temperature, humidity);
  }
}


void updateDisplay(float temperature, float humidity, int pressCount) {
  switch (pressCount) {
    case 1: 
      displayDigit((dhtDisplayMode + 1) % 3);
      break;
    case 2: 
      displayTemperature(temperature);
      displayHumidity(humidity);
      break;
    case 3: 
      displayLast24HoursData();
      break;
    case 4: 
      displayLast24HoursByHours();
      break;
  }
}

void displayDigit(int digit) {
  digitalWrite(DISP_DIGIT_PIN, LOW); 
  
  switch (digit) {
    case 0:
      digitalWrite(DISP_A_PIN, HIGH);
      digitalWrite(DISP_B_PIN, HIGH);
      digitalWrite(DISP_C_PIN, HIGH);
      digitalWrite(DISP_D_PIN, HIGH);
      digitalWrite(DISP_E_PIN, HIGH);
      digitalWrite(DISP_F_PIN, HIGH);
      digitalWrite(DISP_G_PIN, LOW);
      break;
    case 1:
      digitalWrite(DISP_A_PIN, LOW);
      digitalWrite(DISP_B_PIN, HIGH);
      digitalWrite(DISP_C_PIN, HIGH);
      digitalWrite(DISP_D_PIN, LOW);
      digitalWrite(DISP_E_PIN, LOW);
      digitalWrite(DISP_F_PIN, LOW);
      digitalWrite(DISP_G_PIN, LOW);
      break;
    case 2:
      digitalWrite(DISP_A_PIN, HIGH);
      digitalWrite(DISP_B_PIN, HIGH);
      digitalWrite(DISP_C_PIN, LOW);
      digitalWrite(DISP_D_PIN, HIGH);
      digitalWrite(DISP_E_PIN, HIGH);
      digitalWrite(DISP_F_PIN, LOW);
      digitalWrite(DISP_G_PIN, HIGH);
      break;
    case 3:
      digitalWrite(DISP_A_PIN, HIGH);
      digitalWrite(DISP_B_PIN, HIGH);
      digitalWrite(DISP_C_PIN, HIGH);
      digitalWrite(DISP_D_PIN, HIGH);
      digitalWrite(DISP_E_PIN, LOW);
      digitalWrite(DISP_F_PIN, LOW);
      digitalWrite(DISP_G_PIN, HIGH);
      break;
    case 4:
      digitalWrite(DISP_A_PIN, LOW);
      digitalWrite(DISP_B_PIN, HIGH);
      digitalWrite(DISP_C_PIN, HIGH);
      digitalWrite(DISP_D_PIN, LOW);
      digitalWrite(DISP_E_PIN, LOW);
      digitalWrite(DISP_F_PIN, HIGH);
      digitalWrite(DISP_G_PIN, HIGH);
      break;
    case 5:
      digitalWrite(DISP_A_PIN, HIGH);
      digitalWrite(DISP_B_PIN, LOW);
      digitalWrite(DISP_C_PIN, HIGH);
      digitalWrite(DISP_D_PIN, HIGH);
      digitalWrite(DISP_E_PIN, LOW);
      digitalWrite(DISP_F_PIN, HIGH);
      digitalWrite(DISP_G_PIN, HIGH);
      break;
    case 6:
      digitalWrite(DISP_A_PIN, HIGH);
      digitalWrite(DISP_B_PIN, LOW);
      digitalWrite(DISP_C_PIN, HIGH);
      digitalWrite(DISP_D_PIN, HIGH);
      digitalWrite(DISP_E_PIN, HIGH);
      digitalWrite(DISP_F_PIN, HIGH);
      digitalWrite(DISP_G_PIN, HIGH);
      break;
    case 7:
      digitalWrite(DISP_A_PIN, HIGH);
      digitalWrite(DISP_B_PIN, HIGH);
      digitalWrite(DISP_C_PIN, HIGH);
      digitalWrite(DISP_D_PIN, LOW);
      digitalWrite(DISP_E_PIN, LOW);
      digitalWrite(DISP_F_PIN, LOW);
      digitalWrite(DISP_G_PIN, LOW);
      break;
    case 8:
      digitalWrite(DISP_A_PIN, HIGH);
      digitalWrite(DISP_B_PIN, HIGH);
      digitalWrite(DISP_C_PIN, HIGH);
      digitalWrite(DISP_D_PIN, HIGH);
      digitalWrite(DISP_E_PIN, HIGH);
      digitalWrite(DISP_F_PIN, HIGH);
      digitalWrite(DISP_G_PIN, HIGH);
      break;
    case 9:
      digitalWrite(DISP_A_PIN, HIGH);
      digitalWrite(DISP_B_PIN, HIGH);
      digitalWrite(DISP_C_PIN, HIGH);
      digitalWrite(DISP_D_PIN, HIGH);
      digitalWrite(DISP_E_PIN, LOW);
      digitalWrite(DISP_F_PIN, HIGH);
      digitalWrite(DISP_G_PIN, HIGH);
      break;
    case 10: 
      digitalWrite(DISP_DP_PIN, HIGH);
      break;
    default:
      break;
  }
  delay(1000);
  digitalWrite(DISP_DIGIT_PIN, HIGH); 
}

void clearDisplay(){
    digitalWrite(DISP_A_PIN, LOW);
  digitalWrite(DISP_B_PIN, LOW);
  digitalWrite(DISP_C_PIN, LOW);
  digitalWrite(DISP_D_PIN, LOW);
  digitalWrite(DISP_E_PIN, LOW);
  digitalWrite(DISP_F_PIN, LOW);
  digitalWrite(DISP_G_PIN, LOW);
  
  digitalWrite(DISP_DP_PIN, LOW);
}
void displayTemperature(float temperature) {
  int temp = temperature;
  if (dhtDisplayMode == 2) { 
    temp = (temperature * 9 / 5) + 32;
  } else if (dhtDisplayMode == 1) { 
    temp += 273.15;
  }
  
  int tempHundreds = temp / 100;
  int tempTens = (temp / 10) % 10;
  int tempOnes = temp % 10;
  
  displayDigit(tempHundreds);
  delay(5);

  displayDigit(tempTens);
  delay(5);

  displayDigit(tempOnes);
  delay(5);

clearDisplay();

  displayDigit(10);
clearDisplay();

  delay(1000);
}

void displayHumidity(float humidity) {
  int hum = (int)(humidity * 100); 
  int humHundreds = (hum / 100) % 10; 
  int humTens = (hum / 10) % 10;      
  int humOnes = hum % 10;             

  displayDigit(humHundreds);
  delay(5);

  displayDigit(humTens);
  delay(5);

  digitalWrite(DISP_DP_PIN, HIGH);
  delay(5);

  displayDigit(humOnes);
  delay(5);

  displayDigit(0);
  delay(5);
}

void displayLast24HoursData() {
  float avgTemperature = 0.0;
  float avgHumidity = 0.0;

  for (int i = 0; i < 24; i++) {
    avgTemperature += pastTemperature[i];
    avgHumidity += pastHumidity[i];
  }
  avgTemperature /= 24;
  avgHumidity /= 24;

  displayTemperature(avgTemperature);
  delay(5);
  displayHumidity(avgHumidity);
}

void displayLast24HoursByHours() {
  for (int i = 0; i < 24; i++) {
    displayHour(i);
    delay(1000); 
    displayTemperature(pastTemperature[i]);
    delay(1000);
    displayHumidity(pastHumidity[i]);
    delay(2000); 
  }
}

void displayHour(int hour) {
  int hourTens = hour / 10;
  int hourOnes = hour % 10;

  displayDigit(hourTens);
  delay(5);

  displayDigit(hourOnes);
  delay(5);
}

void logData(float temperature, float humidity) {
  static int index = 0; // Index to track where to store data in arrays
  pastTemperature[index] = temperature;
  pastHumidity[index] = humidity;
  index = (index + 1) % 24; // Circular buffer, overwrite oldest data
}
