//using library DHT sensor Library by Adafruit Version 1.4.3

#include <DHT.h>
#define DHTPIN 2  //GPIO2 atau D4
// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT11     // DHT 22 (AM2302)
#define DHTTYPE    DHT21     // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;

//pin output
int red_pin = 3;
int yellow_pin = 4;
int green_pin = 5;

//Incoming Serial Data Array
const byte channel = 3;
char* arr[channel];

//millis================================
//Set every 5 sec read DHT
unsigned long previousMillis = 0;  // variable to store the last time the task was run
const long interval = 5000;        // time interval in milliseconds (eg 1000ms = 1 second)
//======================================

void setup() {
  Serial.begin(9600);
  delay(500);
  pinMode(red_pin, OUTPUT);
  pinMode(yellow_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);

  Serial.println("Sensor Suhu dan Kelembaban Udara DHT11, dan kirim data ke Serial");
  dht.begin();
  delay(1000);
}

void loop() {
  unsigned long currentMillis = millis();  // mendapatkan waktu sekarang
  // Checks whether it is time to run the task
  if (currentMillis - previousMillis >= interval) {
    // Save the last time the task was run
    previousMillis = currentMillis;

    t = dht.readTemperature();
    h = dht.readHumidity();
    Serial.print(t);
    Serial.print(",");
    Serial.print(h);
    Serial.println();
  }
  // Read Excel variables from serial port (Data Streamer)
  processIncomingSerial();
  delay(50);
}

// INCOMING SERIAL DATA PROCESSING CODE----------------------------------------
void processIncomingSerial()
{
  if (Serial.available()) {
    parseData(GetSerialData());
    //Get data from serial, format: red_value,yellow_value, green_value. ex:0,0,1
    int red = atoi(arr[0]);
    int yellow = atoi(arr[1]);
    int green = atoi(arr[2]);

    //turn ON/OFF lamp
    digitalWrite(red_pin, red);
    digitalWrite(yellow_pin, yellow);
    digitalWrite(green_pin, green);
  }
}

// Gathers bytes from serial port to build inputString
//char* GetSerialData()
char* GetSerialData()
{
  static char inputString[64]; // Create a char array to store incoming data
  memset(inputString, 0, sizeof(inputString)); // Clear the memory from a pervious reading
  while (Serial.available()) {
    Serial.readBytesUntil('\n', inputString, 64); //Read every byte in Serial buffer until line end or 64 bytes
  }
  return inputString;
}

// Seperate the data at each delimeter
void parseData(char data[])
{
  char *token = strtok(data, ","); // Find the first delimeter and return the token before it
  int index = 0; // Index to track storage in the array
  while (token != NULL) { // Char* strings terminate w/ a Null character. We'll keep running the command until we hit it
    arr[index] = token; // Assign the token to an array
    token = strtok(NULL, ","); // Conintue to the next delimeter
    index++; // incremenet index to store next value
  }
}
