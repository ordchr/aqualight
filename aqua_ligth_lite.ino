/*
   Проект: Aqua Light
   Версия: 0.1
   Разработчик: ordchr
*/

// Модуль работы со временем
#include <Wire.h>
#include "RTClib.h"
// Инициализируем объект для работы со временем
RTC_DS1307 rtc;

// О программе
const float AQUALIGHT_VERSION = 0.1;

// Время включения и выключения света
unsigned long LightOnAfter = 32400; // 9 * 60 * 60 + 60 * 0; // 9 часов утра
unsigned long LightOffAfter = 72000; // 20 * 60 * 60 + 60 * 0; // 8 часов вечера

// Время включения и выключения кнопки покормить вручную
/* unsigned long FeedKeyOnAfter = 19 * 60 * 60 + 60 * 50 + 0; // 19-50 0 сек */
/* unsigned long FeedKeyOffAfter = 19 * 60 * 60 + 60 * 50 + 1; // 19-50 1 сек */

// Контакты управления реле
const int LightRelay = 2;
const int Feed = 9;

// Текущее дата и время
char* str_char_now;

unsigned long timer_cycl = 0;

// the setup function runs once when you press reset or power the board
void setup() {

    if (! rtc.begin()) {
        //Serial.println("Couldn't find RTC");
        while (1);
    }

    if (! rtc.isrunning()) {
        Serial.println("RTC is NOT running!");
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }

    // Инициализация выходов реле
    pinMode(LightRelay, OUTPUT);
    pinMode(Feed, OUTPUT);
    digitalWrite( LightRelay, HIGH );
    digitalWrite( Feed, HIGH );

    Serial.begin(9600);

}

// the loop function runs over and over again forever
void loop() {

    if ( millis() - timer_cycl > 3000 ) {
        // Serial.println("*************");
        timer_cycl = millis();
        DateTime now = rtc.now();

        char current_date_str[10];
        char current_time_str[8];
        sprintf(current_date_str, "%02d.%02d ",  now.day(), now.month());
        sprintf(current_time_str, "%02d:%02d:%02d",  now.hour(), now.minute(), now.second());
        str_char_now = strcat( current_date_str, current_time_str );
        // Serial.println(str_char_now);

        // Serial.println(LightOnAfter);
        // Serial.println(LightOffAfter);
        unsigned long now_seconds; 
        unsigned long hour = now.hour();
        unsigned long minute = now.minute();
        unsigned long second = now.second();

        now_seconds =  hour * 60 * 60 + minute * 60 + second;
        
        // Serial.println( "now_seconds: " );
        // Serial.println( now_seconds );

        if ( (LightOnAfter < now_seconds) && (LightOffAfter > now_seconds) ) {
            // В этом периоде свет должен быть включен
            digitalWrite( LightRelay, LOW );
            // Serial.println("Set LOW");

        } else {
            // А тут свет должен быть выключен
            digitalWrite( LightRelay, HIGH );
            // Serial.println("Set HIGH");

        }

    }

}


