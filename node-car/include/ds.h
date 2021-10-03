// #include <DallasTemperature.h>
#include <DS18B20.h>
#include <OneWire.h>

#define ONE_WIRE_DS D7

/**DS18B2018B20 ADDRES **/
// byte DS_ADDRESS[8] = {0x28, 0xAB, 0x0E, 0x9A, 0x0A, 0x00, 0x00, 0x88};

OneWire DS_WIRE(ONE_WIRE_DS);
DS18B20 sensor(&DS_WIRE);

void readTemperature() {
  
	if(sensor.isConversionComplete()){
		Serial.print("inside:\t");
		Serial.println(sensor.getTempC(),1);
		sensor.requestTemperatures();
	}
}