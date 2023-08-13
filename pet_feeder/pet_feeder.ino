#include <Wire.h>
#include <DS3231.h>

DS3231 set;
RTClib myRTC;

void setup () {
    Serial.begin(9600);
    Wire.begin();
    delay(500);
    set.setYear(2023 - 48);
    set.setMonth(8);
    set.setDate(14);
    set.setHour(11);
    set.setMinute(58);
    set.setSecond(00);
    pinMode(D6, OUTPUT);
}

void loop () {
    pinMode(D6, LOW);

    delay(500);
    
    DateTime now = myRTC.now();
   
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    Serial.print("Temp: ");
    Serial.println(set.getTemperature());

    pinMode(D6, HIGH);

    delay(500);
}