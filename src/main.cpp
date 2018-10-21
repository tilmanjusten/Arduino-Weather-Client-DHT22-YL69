// v2.0.0

#include <DHT.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile
#include <Log.h>

#ifndef MODE_PRODUCTION
#define MODE_PRODUCTION 0x0
#endif
#ifndef MODE_DEVELOPMENT
#define MODE_DEVELOPMENT 0x1
#endif
#ifndef MODE
#define MODE MODE_PRODUCTION
#endif

#define DHTPIN 4 // what digital pin we're connected to
//#define ID "THOR" // ok 1810211235 v2.0.0, was ODIN => THOR
//#define ID "AMUN" // ok 1810211245 v2.1.0, was INKE => AMUN
//#define ID "ZEUS" // ok 1810211120 v2.0.0, was PURL => ZEUS
#define MSGFORMAT_DHT "%s0HU%03dTE%+04d"
#define MSGFORMAT_YL69 "%sMO%04dMV%+04d"
#define DELAY_DHT 2750
#define DELAY_YL69 900000 // 15 Minutes
#define YL69PIN PIN_A1
#define YL69PIN_VCC 2

#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321

RH_ASK driver;
Log l(LOG_MODE_SILENT);

float prevHumidity = 0;
float prevTemperature = 0;
int humidities[60] = {};
int temperatures[60] = {};
int valueIterator = 0;
int valuesNum = sizeof(humidities) / sizeof(int);
bool initialized_dht = false;
bool reinitialized_dht = false;
long timerDebugDHT = 0;
long timerDHT = 0;
long timerYL69 = 0;
bool initialized_yl69 = false;

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

void printValuesDHT(char *msgBuffer, int h, int t)
{
  l.l((millis() - timerDebugDHT) / 1000);
  l.l("s -> ");
  l.l("HU ");
  l.l(h);
  l.l(" TE ");
  l.l(t);
  l.l(" Message: ");
  l.l(msgBuffer);
  l.ln();

  timerDebugDHT = millis();
}

void handleDHT()
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
    l.ln("Failed to read from DHT sensor!");

    return;
  }

  humidities[valueIterator] = h;
  temperatures[valueIterator] = t;
  valueIterator++;

  // reset counter
  if (valueIterator == valuesNum)
  {
    // set initialized after first iteration loop
    if (initialized_dht == false)
    {
      l.ln("First iteration loop completed. Begin to send data.");

      initialized_dht = true;
    }

    if (initialized_dht == true && reinitialized_dht == false)
    {
      l.ln("Update full set of measurements.");

      reinitialized_dht = true;
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
  if (reinitialized_dht)
  {
    char msgBuffer[16];
    int n = sprintf(msgBuffer, MSGFORMAT_DHT, ID, hAvg, tAvg);

#if MODE == MODE_PRODUCTION
    driver.send((uint8_t *)msgBuffer, n);
    driver.waitPacketSent();
#endif

    // update storage
    prevHumidity = hAvgFloat;
    prevTemperature = tAvgFloat;

    // get full set of measurements before sending again
    reinitialized_dht = false;

    printValuesDHT(msgBuffer, hAvg, tAvg);
  }
}

void handleYL69()
{
  l.l("Enable sensor... ");
  digitalWrite(YL69PIN_VCC, HIGH);

  delay(50);

  l.l("Read data... ");
  int analog = analogRead(YL69PIN);
  int val = map(analog, 0, 1023, 100, 0);

  l.l("Disable sensor... ");
  digitalWrite(YL69PIN_VCC, LOW);

  // Send via RadioHead
  char msgBuffer[16];
  int n = sprintf(msgBuffer, MSGFORMAT_YL69, ID, analog, val);

#if MODE == MODE_PRODUCTION
  driver.send((uint8_t *)msgBuffer, n);
  driver.waitPacketSent();
#endif

  // Debug output
  l.l("Analog: ");
  l.l(analog);
  l.l(", Value: ");
  l.l(val);
  l.l("%");
  l.l(", Message: ");
  l.l(msgBuffer);
  l.ln();
}

void setup()
{
  Serial.begin(9600);
  l.l("DHT22 Humidity and Temperature: ");
  l.ln(ID);
  l.l("Send average of ");
  l.l(valuesNum);
  l.ln(" measurements.");

  // DHT22
  dht.begin();

  // YL69
  pinMode(YL69PIN, INPUT);
  pinMode(YL69PIN_VCC, OUTPUT);

  digitalWrite(YL69PIN_VCC, LOW);

  delay(100);

  if (!driver.init())
  {
    l.ln("Failed to initialize RadioHead!");
  }
}

void loop()
{
  // every loop may take 3s => 3s * 60 values = send value every 3 min => ~480 values per day
  if (millis() - timerDHT > DELAY_DHT)
  {
    handleDHT();

    timerDHT = millis();
  }

  // every loop may take 3s => 3s * 60 values = send value every 3 min => ~480 values per day
  if (initialized_yl69 == false || millis() - timerYL69 > DELAY_YL69)
  {
    handleYL69();

    timerYL69 = millis();
    initialized_yl69 = true;
  }
}
