/*
	Сработка сирены и управление электрозамком
	Телеграм: @denzizya
	1. Пин оптопары
	4. Пин реле сирены
*/
class Alert
{
	public:
		Alert(int alert_optopara, int alert_rele);
		void statusAlert();
	  bool status_alert = false;  // флаг сработки тревоги
    
	private:
		int _alert_optopara;       // оптопара сработки тревоги 
    int _alert_rele;           // реле сирены
};
