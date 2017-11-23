/* 
 * Arduino demo sketch that queries and shows the following data from the DS2438 1-Wire chip:
 * - VAD voltage between pin 1 and 4 (GND and VAD) connected to the analog humidity sensor;
 * - Internal temperature;
 * - IAD voltage between pin 2 and 3 (vsens+ and vsens-) in millivolts connected to the shunt resistor and the photodiode;
 * - VDD voltage between 1 and 5 (GND and VDD) supply voltage.
 * 
 * Calculates and shows the relative humidity using VDD, VAD and Temperature
 * 
 * Demo for 1-Wire Temperature, Humidity and Light Sensor https://taaralabs.eu/ol1
 * 
 * by Märt Maiste, TaaraLabs
 * https://github.com/TaaraLabs/1-WireHumidityLight-Demo
 * 2017-08-22
 *
*/

#include <OneWire.h>

OneWire ds(14);

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {
  byte i;
  byte data[12];
  byte addr[8];

  while (!ds.search(addr)) {
    ds.reset_search();
    Serial.println("No more sensors");
    delay(1000);
  }

  for ( i = 0; i < 8; i++) { // we need 9 bytes, last one for CRC
    Serial.print(addr[i], HEX);
    Serial.print(":");
  }
  Serial.print(" ");


  // Configure chip to read VAD and not VDD
  ds.reset();
  ds.select(addr);
  ds.write(0x4E);
  ds.write(0x00);
  ds.write(0x00);

  // Convert voltage
  ds.reset();
  ds.select(addr);
  ds.write(0xB4);
  delay(20);

  // Start temperature conversion
  ds.reset();
  ds.select(addr);
  ds.write(0x44);
  delay(20);
  
  // Current readings are updated automatically
  
  // Recall
  ds.reset();
  ds.select(addr);
  ds.write(0xB8);
  ds.write(0x00);

  // Read Scratchpad
  ds.reset();
  ds.select(addr);
  ds.write(0xBE);
  ds.write(0x00);
  
  for ( i = 0; i < 9; i++) { // we need 9 bytes, last one for CRC
    data[i] = ds.read();
    // Serial.print(data[i], HEX);
    // Serial.print(" ");
  }

  // CRC
  if ( OneWire::crc8(data, 8) != data[8]) {
    Serial.println("CRC is not valid!");
    return;
  }
  
  Serial.print("Temp=");
  float temp = (((((int16_t) data[2]) << 8) | data[1]) >> 3) * 0.03125;
  Serial.print(temp);

  Serial.print(" VAD=");
  float vad = ((((int16_t)data[4]) << 8) | data[3]) * 0.01;
  Serial.print(vad);

  Serial.print(" IAD=");
  float iad = ((((int16_t)data[6]) << 8) | data[5] << 2) * 0.2441;
  Serial.print(iad);

  // Configure chip to read VDD and not VAD
  ds.reset();
  ds.select(addr);
  ds.write(0x4E);
  ds.write(0x00);
  ds.write(0x0F);

  // Convert voltage
  ds.reset();
  ds.select(addr);
  ds.write(0xB4);
  delay(20);

  // Recall
  ds.reset();
  ds.select(addr);
  ds.write(0xB8);
  ds.write(0x00);

  // Read Scratchpad
  ds.reset();
  ds.select(addr);
  ds.write(0xBE);
  ds.write(0x00);
  
  Serial.print(" ");
    for ( i = 0; i < 9; i++) { // we need 9 bytes, last one for CRC
    data[i] = ds.read();
  }

  // CRC
  if ( OneWire::crc8(data, 8) != data[8]) {
    Serial.println("CRC is not valid!");
    return;
  }
  
  Serial.print(" VDD=");
  float vdd = ((((int16_t)data[4]) << 8) | data[3]) * 0.01;
  Serial.print(vdd);

  Serial.print(" Hum=");
  float stdHum = ((vad / vdd) - 0.16) / 0.0062;
  float trueHum = stdHum / (1.0546 - (0.00216 * temp));
  Serial.print(trueHum);
  
  Serial.println();
}

