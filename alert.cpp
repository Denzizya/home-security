#include "Arduino.h"
#include "alert.h"

Alert::Alert(int alert_optopara, int alert_rele)
{
	pinMode(alert_optopara, INPUT_PULLUP);  // контакт отслеживания сработки серены.
  pinMode(alert_rele, OUTPUT);            // выставляем контакт реле на выход
  digitalWrite(alert_rele, HIGH);         // выставляем высокий уровень
	
	_alert_optopara = alert_optopara;
	_alert_rele = alert_rele;
}

// Функция сработки серены
void Alert::statusAlert()
{
    if (digitalRead(_alert_optopara) && !status_alert)
    {
        status_alert = true;
        Serial.println("ALERT ON");
    }
    if (!digitalRead(_alert_optopara) && status_alert)
    {
        status_alert = false;
        Serial.println("ALERT OFF");
    }
    delay(10);
}
