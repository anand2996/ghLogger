#include <RTClib.h>
#include <DHT.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>

// Declare Objects 

File logFile;           // Create file to log to
RTC_DS1307 rtc;         // Create a RealTimeClock object

// Integers and Pin Numbers 
 
#define DHTTYPE DHT22   // model DHT 22 
#define DHTPIN 2

const int delaytime = 5000;
const int chipSelect = 10;

// Declare Variables

long temperature;
DHT dht(DHTPIN, DHTTYPE);

void setup()   
{
Serial.begin(9600);
Serial.print("Initializing SD Card... ");
  if (!SD.begin(chipSelect)) {
   Serial.println("Card Load Failed, or Not Detected"); // Terminate
    return; 
  }
  Serial.println("Card Successfully Initialized.");
  dht.begin ();                   // Initialize DHT library
  SD.begin (chipSelect);
  rtc.begin();                    // Initialize RTC library 
  pinMode(10, OUTPUT);

  #ifdef AVR
    Wire.begin();
  #else
    Wire1.begin();    // Shield I2C pins connect to alt I2C bus on Arduino Due
  #endif
  
// Set Date and Time
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));       
  rtc.adjust(DateTime(2016, 11, 16, 18, 0, 0));
  char filename[] = "LOGGER00.CSV";                       
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logFile = SD.open(filename, FILE_WRITE); 
      break;  
    }
  }
  if (!logFile) {
    Serial.println("Could not create File");
  }
  Serial.print("Logging to: ");
  Serial.println(filename);
  logFile.print ("Format date:dd/mm/yy,hh:mm:ss,temperature,humidity");
  logFile.println();
}

void loop ()   
{
  delay (delaytime);
  long humidity = dht.readHumidity();
  long temperature = dht.readTemperature();
  DateTime now = rtc.now();
  logFile.print(now.day(), DEC);
  logFile.print('/');
  logFile.print(now.month(), DEC);
  logFile.print('/');
  logFile.print(now.year(), DEC);
  logFile.print(',');
  logFile.print(' ');
  logFile.print(now.hour(), DEC);
  logFile.print(':');
  logFile.print(now.minute(), DEC);
  logFile.print(':');
  logFile.print(now.second(), DEC);
  logFile.print (",");
  logFile.print (" ");
  logFile.print (temperature);
  logFile.print (",");
  logFile.print (" ");
  logFile.print (humidity);
  logFile.println();
  logFile.flush();
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  Serial.print(',');
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print (",");
  Serial.print (" ");
  Serial.print (temperature);
  Serial.print (" *C");
  Serial.print (",");
  Serial.print (" ");
  Serial.print (humidity);
  Serial.print (" %");
  Serial.println(); 
}
