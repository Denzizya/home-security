#include "Arduino.h"
#include "call.h"

Call::Call(int call_led, int call_buton, int call_optopara, int timer_down_buton)
{
	ledcSetup(_ledChannel, _freq, _resolution); // настраиваем ШИМ
	ledcAttachPin(call_led, _ledChannel);      	// привязываем канал к портам светодиодов
	ledcWrite(_ledChannel, 255);                // устанавлваем первоначальную яркость
	pinMode(call_buton, INPUT_PULLUP);			// устанавливаем пин кнопки на вход с подтяжкой
    pinMode(call_optopara, OUTPUT);
    digitalWrite(call_optopara, LOW);
	
	_call_led = call_led;
	_call_buton = call_buton;
	_call_optopara = call_optopara;
	_timer_down_buton = timer_down_buton;
}

//Функция мигания звонка
void Call::callBlik()
{

	if(analogRead(_call_buton) > 5)
	{ 
		_buton_down_time = 0;
		// уменьшаем яркость светодиода
		for(int dutyCycle = 255; dutyCycle >= 10; dutyCycle--)
		{
			ledcWrite(_ledChannel, dutyCycle);   
			delay(5);
			if(analogRead(_call_buton)<30){break;}
		}

		// увеличиваем яркость светодиода
		for(int dutyCycle = 10; dutyCycle <= 255; dutyCycle++)
		{   
			ledcWrite(_ledChannel, dutyCycle);   
			delay(5);
			if(analogRead(_call_buton)<30){break;}
		}
	}
	else{
			ledcWrite(_ledChannel, 10);
			_buton_down_time++;
	}
	
	if(_buton_down_time > _timer_down_buton)
	{
		digitalWrite(_call_optopara, HIGH);
		delay(1000);
		digitalWrite(_call_optopara, LOW);
	}
}