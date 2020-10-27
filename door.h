/*
	Открытие закрытие электро-замка 
	Телеграм: @denzizya
	1. Пин открвтия
	2. Пин закрытия
	3. Время работы
*/
class Door
{
	public:
		Door(int lock_open, int lock_close, int time_power_lock);
		void doorOpen();
		void doorClose();
    void doorOpenBT();
    void doorCloseBT();
	private:
		void _returnRele();
		int _lock_open;
		int _lock_close;
		int _time_power_lock;
};
