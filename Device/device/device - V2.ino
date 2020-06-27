// Phuc Le
// 456061
// Exhbition - Watering system
// Hardware part (device code)


//"ALWAYS use unsigned long for timers, not int"
//unsigned long lastWateringTime = millis() - (waterEveryMinutes * 60 * 1000); // Time of last watering. Milliseconds since Arduino program has started.
//unsigned long wateringStartedTime = millis() - (wateringTimeInMinutes * 60 * 1000); //"ALWAYS use unsigned long for timers, not int"
//const double waterEveryMinutes = 0.5; // Time Interval pump starts watering process. In minutes.
//const double wateringTimeInMinutes = 0.2; // Time Watering Process takes. In minutes.
#include <Wire.h>
#include <HCSR04.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SoftwareSerial.h>
// OLED display TWI address
#define OLED_ADDR   0x3C

Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int motorPinOut = 10; //Pin starts water pump motor
const int buttonPin = 2;     // the number of the pushbutton pin
const int wateringLedPin =  13;      // the number of the LED pin

boolean isWatering = false; //


boolean buttonState = false; //Button state, FALSE=unpressed, TRUE=pressed
int lastButtonState = buttonState; // Previous button state. Used to reduce state related signals spamming.

const int sensor_pin_1 = A1;  /* Soil moisture sensor O/P pin */
int motorSpeed = 1000;
int trigPin = 11;    // Trigger
int echoPin = 12;    // Echo
long duration, cm;
int greenLed = 8;
int yellowLed = 9;
int redLed = 10;
byte bluetoothData;
SoftwareSerial Bluetooth(2, 3);
UltraSonicDistanceSensor distanceSensor(trigPin, echoPin);
void setup() {
  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  //Serial Port begin
  Serial.begin (9600);
  Bluetooth.begin(9600);
  //Define inputs and outputs
  //  pinMode(trigPin, OUTPUT);
  //  pinMode(echoPin, INPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);

  pinMode(motorPinOut, OUTPUT);
  // initialize the LED pin as an output:
  pinMode(wateringLedPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {
  //=====================================soil's moisture================================//

  float moisture_percentage_A1;
  float sensor_analog_A1;
  sensor_analog_A1 = analogRead(sensor_pin_1);
  moisture_percentage_A1 = ( 100 - ((sensor_analog_A1 / 1023.00) * 100 ));
  //=====================================water level detector===============================//
  float distance = distanceSensor.measureDistanceCm();
  if (distance < 0) {
    distance = 0;
  }
  //  Serial.println(distance);
  //
  //  Bluetooth.println(distance);
  //Bluetooth print

  if (Bluetooth.available() > 0) {                    // wait until there is data to receive
    bluetoothData = Bluetooth.read();
    Bluetooth.print(moisture_percentage_A1);
    Bluetooth.print("%");
    Bluetooth.print("|");
    Bluetooth.print(distance);
    Bluetooth.print(" cm");
    Bluetooth.println();
    delay(700);// Einlesen der Daten
  }
  //  Serial.print(moisture_percentage_A1);
  //  Serial.print("|");
  //  Serial.print(distance);




  display.clearDisplay();

  // display a line of text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(15, 20);
  display.print("Moisture: ");
  display.print(moisture_percentage_A1);
  display.print("%");
  display.setCursor(10, 40);
  display.print("Water Level: ");
  display.print((float)distance);
  display.print("cm");
  display.display();
  delay(250);
  display.clearDisplay();





  //If water tank is about 12cm height
  if (distance >= 8 && distance <= 12) {
    Serial.print("High");
    digitalWrite(greenLed, HIGH);
    digitalWrite(yellowLed, LOW);
    digitalWrite(redLed, LOW);
  }
  if (distance >= 4 && distance <= 8) {
    Serial.print("Moderate");
    digitalWrite(greenLed, LOW);
    digitalWrite(yellowLed, HIGH);
    digitalWrite(redLed, LOW);
  }
  if (distance >= 1 && distance <= 4) {
    Serial.print("Low");
    digitalWrite(greenLed, LOW);
    digitalWrite(yellowLed, LOW);
    digitalWrite(redLed, HIGH);
  }
  delay(700);

}
//======================================Water Pumper=================================//
//  if (moisture_percentage_A1 < 55) {
//    if (isWatering) {
//      digitalWrite(motorPinOut, LOW);
//      if (timer <= 0) {
//        stopWatering();
//      } else {
//        printOuput("Watering ended!");
//      }
//    }
//  }
//  else {
//    if (moisture_percentage_A1 >= 55) {
//      digitalWrite(motorPinOut, HIGH);
//      if (timer <= 0) {
//        startWatering();
//      } else {
//        //printOuput("Watering ends in " + millisToTime(timer));
//        printOuput("Watering started!");
//      }
//    }
//  }
//
//  //button state allow manually turn on/off motor ~ interfering the watering procedure
//  buttonState = digitalRead(buttonPin);
//  if (buttonState == HIGH) {
//    // turn LED on:
//    digitalWrite(wateringLedPin, HIGH);
//  } else {
//    // turn LED off:
//    digitalWrite(wateringLedPin, LOW);
//  }
//  // check if the pushbutton is pressed (buttonState is HIGH) AND if previous pushbutton state was different start or stop watering
//  if (buttonState == HIGH && lastButtonState == LOW && !isWatering) {
//    startWatering();
//  } else if (buttonState == HIGH && lastButtonState == LOW && isWatering) {
//    stopWatering();
//  }
//  lastButtonState = buttonState;
//}
//
//
//void startWatering() {
//  isWatering = true;
//  printOuput("Watering starts");
//  // start motor
//  digitalWrite(motorPinOut, LOW);
//
//}
//void stopWatering() {
//  isWatering = false;
//  printOuput("Watering ends");
//  // stop motor
//  digitalWrite(motorPinOut, HIGH);
//}
//
//String millisToTime(unsigned long milliseconds) {
//  int seconds = (int) (milliseconds / 1000) % 60 ;
//  int minutes = (int) ((milliseconds / (1000 * 60)) % 60);
//  int hours   = (int) ((milliseconds / (1000 * 60 * 60)) % 24);
//  String timeString = "";
//  timeString = timeString + hours + ":" + minutes + ":" + seconds;
//  return timeString;
//}
//void printOuput(String msg) {
//  Serial.print(msg + "\n");
//}
