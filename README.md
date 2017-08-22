# Demo for 1-Wire Temperature, Humidity and Light Sensor https://taaralabs.eu/ol1

[1-WireHumidityLight Website](https://taaralabs.eu/ol1)

Arduino demo sketch that queries and shows the following data from the DS2438 1-Wire chip:
* VAD voltage between pin 1 and 4 (GND and VAD) connected to the analog humidity sensor;
* Internal temperature;
* IAD voltage between pin 2 and 3 (vsens+ and vsens-) in millivolts connected to the shunt resistor and the photodiode;
* VDD voltage between 1 and 5 (GND and VDD) supply voltage.
* Also calculates and shows the relative humidity using VDD, VAD and Temperature.
