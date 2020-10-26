#include "Arduino.h"
#include "door.h"

Door::Door(int lock_open, int lock_close, int time_power_lock)
{
    pinMode (lock_open, OUTPUT);    // задаем контакт подключения светодиода как выходной
    pinMode (lock_close, OUTPUT);   // задаем контакт подключения светодиода как выходной
    digitalWrite(lock_open, HIGH);  // выставляем высокий уровень
    digitalWrite(lock_close, HIGH); // выставляем высокий уровень
	
	_lock_open = lock_open;
	_lock_close = lock_close;
	_time_power_lock = time_power_lock;
}

//Функция открытия двери
void Door::doorOpen()
{
  digitalWrite(_lock_open, LOW);
  delay(10); 
  digitalWrite(_lock_open, HIGH);
  delay(10); 
  digitalWrite(_lock_open, LOW);
  delay(10); 
  digitalWrite(_lock_open, HIGH);
  delay(10); 
  digitalWrite(_lock_open, LOW);
  delay(10); 
  digitalWrite(_lock_open, HIGH);
  delay(10); 
  digitalWrite(_lock_open, LOW);
	_returnRele();
}

//Функция закрытия двери
void Door::doorClose()
{
	digitalWrite(_lock_close, LOW);
  delay(10); 
  digitalWrite(_lock_close, HIGH);
  delay(10); 
  digitalWrite(_lock_close, LOW);
  delay(10); 
  digitalWrite(_lock_close, HIGH);
  delay(10); 
  digitalWrite(_lock_close, LOW);
  delay(10); 
  digitalWrite(_lock_close, HIGH);
  delay(10); 
  digitalWrite(_lock_close, LOW);
	_returnRele();
}

//Функция сброса реле
void Door::_returnRele()
{
	delay(_time_power_lock); 
	digitalWrite(_lock_close, HIGH);
	digitalWrite(_lock_open, HIGH);
}
