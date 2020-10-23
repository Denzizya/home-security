/*
	Мигалка LED с отслеживанием нажатия
	Телеграм: @denzizya
	1. Пин светодиода
	2. Пин кнопки
	3. Пин оптопары
	4. Время задержки на дребезг кнопки
*/
class Call
{
	public:
		Call(int call_led, int call_buton, int call_optopara, int timer_down_buton);
		void callBlik();
	
	private:
		int _call_led;
		int _call_buton;
		int _call_optopara;
		int _buton_down_time = 0;
		int _timer_down_buton;
		
		// задаём свойства ШИМ-сигнала для мигания светодиода
		int _freq = 5000;
		int _ledChannel = 0;
		int _resolution = 8;
};