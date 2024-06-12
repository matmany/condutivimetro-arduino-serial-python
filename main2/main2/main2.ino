#include <ArduinoJson.h>
#include <max6675.h>

#define GPIO_SO 8
#define GPIO_CS 9
#define GPIO_CLK 10
#define BAUDRATE_SERIAL_MONITOR 9600
#define TEMPO_ENTRE_LEITURAS 1000

MAX6675 termopar(GPIO_CLK, GPIO_CS, GPIO_SO);

//CONDUTIMETRO
#define TdsSensorPin A1
#define VREF 5.0          // analog reference voltage(Volt) of the ADC
#define SCOUNT 30         // sum of sample point
int analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0, tdsValue = 0;


void setup()
{
    Serial.begin(BAUDRATE_SERIAL_MONITOR);
    while (!Serial) continue;
}

void loop()
{
    //float temperature = getTemperature();
    float temperature = getTemperatureMedian();
    tdsValue = getppm(tdsValue, temperature);
    enviaJsonSerial(temperature, tdsValue);
    delay(TEMPO_ENTRE_LEITURAS);
}

void enviaJsonSerial(float temperatura, float condutividade)
{
    StaticJsonDocument<100> doc;
    doc["temp"] = temperatura;
    doc["condu"] = condutividade;
    serializeJson(doc, Serial);
    Serial.println();
}

float getTemperature()
{
  float readMean = 0;
  int meanSize = 64;
  for (int i = 0; i<meanSize; i++) {
    readMean = termopar.readCelsius() + readMean;
  }

  return readMean/meanSize;
}

float getTemperatureMedian()
{
  int readSize = 63;
  float readMean[readSize];
  for (int i = 0; i<readSize; i++) {
    readMean[i] = termopar.readCelsius();
  }

   for (int i = 0; i < readSize - 1; i++) {
        for (int j = i + 1; j < readSize; j++) {
            if (readMean[i] > readMean[j]) {
                float temp = readMean[i];
                readMean[i] = readMean[j];
                readMean[j] = temp;
            }
        }
    }

  return readMean[31];
}

float getppm(float ppm, float temperatura)
{
    static unsigned long analogSampleTimepoint = millis();
    if (millis() - analogSampleTimepoint > 40U) // every 40 milliseconds,read the analog value from the ADC
    {
        analogSampleTimepoint = millis();
        analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin); // read the analog value and store into the buffer
        analogBufferIndex++;
        if (analogBufferIndex == SCOUNT)
            analogBufferIndex = 0;
    }
    static unsigned long printTimepoint = millis();
    if (millis() - printTimepoint > 800U)
    {
        printTimepoint = millis();
        for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++)
            analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
        averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0;                                                                                                  // read the analog value more stable by the median filtering algorithm, and convert to voltage value
        float compensationCoefficient = 1.0 + 0.02 * (temperatura - 25.0);                                                                                                               // temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
        float compensationVolatge = averageVoltage / compensationCoefficient;                                                                                                            // temperature compensation
        ppm = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5; // convert voltage value to tds value
    }
    return ppm;
}

int getMedianNum(int bArray[], int iFilterLen)
{
    int bTab[iFilterLen];
    for (byte i = 0; i < iFilterLen; i++)
        bTab[i] = bArray[i];
    int i, j, bTemp;
    for (j = 0; j < iFilterLen - 1; j++)
    {
        for (i = 0; i < iFilterLen - j - 1; i++)
        {
            if (bTab[i] > bTab[i + 1])
            {
                bTemp = bTab[i];
                bTab[i] = bTab[i + 1];
                bTab[i + 1] = bTemp;
            }
        }
    }
    if ((iFilterLen & 1) > 0)
        bTemp = bTab[(iFilterLen - 1) / 2];
    else
        bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
    return bTemp;
}
