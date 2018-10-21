// v1.1.0

#include "DHT.h"
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

#define DHTPIN 4 // what digital pin we're connected to
//#define ID "THOR" // ok 1809191730 v1.1.0, was ODIN => THOR
//#define ID "AMUN" // ok 1809190100 v1.1.0, was INKE => AMUN
#define ID "ZEUS" // ok 1809191730 v1.1.0, was PURL => ZEUS
#define MSGFORMAT "%s0HU%03dTE%+04d"
#define DELAY_DHT22 2750
#define DELAY_YL69 10000

#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321

RH_ASK driver;

float prevHumidity = 0;
float prevTemperature = 0;
int humidities[60] = {};
int temperatures[60] = {};
int valueIterator = 0;
int valuesNum = sizeof(humidities) / sizeof(int);
bool initialized = false;
bool reinitialized = false;
long timerDebug = 0;
long timerDHT22 = 0;
long timerYL69 = 0;

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

void printValues(char *msgBuffer, int h, int t)
{
  Serial.print((millis() - timerDebug) / 1000);
  Serial.print("s -> ");
  Serial.print("HU ");
  Serial.print(h);
  Serial.print(" TE ");
  Serial.print(t);
  Serial.print(" Message: ");
  Serial.print(msgBuffer);
  Serial.println();

  timerDebug = millis();
}

void handleDHT22()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  int h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  int t = dht.readTemperature();
  int hSum = 0;
  int tSum = 0;
  int hAvg;
  int tAvg;
  float hAvgFloat;
  float tAvgFloat;

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");

    return;
  }

  humidities[valueIterator] = h;
  temperatures[valueIterator] = t;
  valueIterator++;

  // reset counter
  if (valueIterator == valuesNum)
  {
    // set initialized after first iteration loop
    if (initialized == false)
    {
      Serial.println("First iteration loop completed. Begin to send data.");

      initialized = true;
    }

    if (initialized == true && reinitialized == false)
    {
      Serial.println("Update full set of measurements.");

      reinitialized = true;
    }

    valueIterator = 0;
  }

  // calc average values
  for (int i = 0; i < valuesNum; i++)
  {
    hSum += humidities[i];
    tSum += temperatures[i];
  }

  hAvgFloat = hSum / valuesNum;
  tAvgFloat = tSum / valuesNum;
  hAvg = ceil(hAvgFloat);
  tAvg = ceil(tAvgFloat);

  // send value when changed (>= 1.00°C / >= 1.00%) and first set of measurements is complete
  //  if (reinitialized && (abs(hAvgFloat - prevHumidity) >= 1 || abs(tAvgFloat - prevTemperature) >= 1)) {
  // send value every full set (~ once every 3 minutes)
  if (reinitialized)
  {
    char msgBuffer[16];
    int n = sprintf(msgBuffer, MSGFORMAT, ID, hAvg, tAvg);

    driver.send((uint8_t *)msgBuffer, n);
    driver.waitPacketSent();

    // update storage
    prevHumidity = hAvgFloat;
    prevTemperature = tAvgFloat;

    // get full set of measurements before sending again
    reinitialized = false;

    printValues(msgBuffer, hAvg, tAvg);
  }
}

void handleYL69()
{
}

void setup()
{
  Serial.begin(9600);
  Serial.print("DHT22 Humidity and Temperature: ");
  Serial.println(ID);
  Serial.print("Send average of ");
  Serial.print(valuesNum);
  Serial.println(" measurements.");

  dht.begin();

  if (!driver.init())
  {
    Serial.println("Failed to initialize RadioHead!");
  }
}

void loop()
{
  // every loop may take 3s => 3s * 60 values = send value every 3 min => ~480 values per day
  if (millis() - timerDHT22 > DELAY_DHT22)
  {
    handleDHT22();

    timerDHT22 = millis();
  }

  // every loop may take 3s => 3s * 60 values = send value every 3 min => ~480 values per day
  if (millis() - timerYL69 > DELAY_YL69)
  {
    handleYL69();

    timerYL69 = millis();
  }
}
