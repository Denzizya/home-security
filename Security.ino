#include "BluetoothSerial.h"  // Заголовочный файл для последовательного Bluetooth будет добавлен по умолчанию в Arduino
BluetoothSerial ESP_BT;       // Объект для Bluetooth
#include "Call.h"             // Библиотека звонка
#include "Door.h"             // Библиотека электрозамка

//Замок
  Door door(33, 32, 50); // подключаем библиотеку электро-замка контакты (открыть, закрыть, время работы)

//Звонок
  Call Call(5, 34, 4, 300); // подключаем библиотеку звонка контакты (светодиод, кнопка, оптопара, задержка нажатия)

//Контакты охраны
  const int SECURITY = 25;
  unsigned char SECURITY_FLAG = false;
  const int ALERT = 26;
  unsigned char ALERT_FLAG = false;
//  const int ALERT_RELE = 35;

//Идентифицируем ядро 0.
  TaskHandle_t Task1;
  TaskHandle_t Task2;
  
void setup()
{
    Serial.begin(115200); // Запускаем последовательный монитор со скоростью 9600
    //Настройка блютуз
    ESP_BT.begin("Control"); // Задаем имя вашего устройства Bluetooth    
    
    pinMode(SECURITY, INPUT_PULLUP);  
    
//    Serial.println("Bluetooth Device is Ready to Pair");  // По готовности сообщаем, что устройство готово к сопряжению
    
    // Создаем задачу с кодом из функции Task1code(),
    // с приоритетом 1 и выполняемую на ядре 0:
    xTaskCreatePinnedToCore(
                        Task1code,   /* Функция задачи */
                        "Task1",     /* Название задачи */
                        10000,       /* Размер стека задачи */
                        NULL,        /* Параметр задачи */
                        1,           /* Приоритет задачи */
                        &Task1,      /* Идентификатор задачи,
                                        чтобы ее можно было отслеживать */
                        0);          /* Ядро для выполнения задачи (0) */
    delay(500);                  

    // Создаем задачу с кодом из функции Task2code(),
    // с приоритетом 1 и выполняемую на ядре 1:
    xTaskCreatePinnedToCore(
                      Task2code,   /* Функция задачи */
                      "Task2",     /* Название задачи */
                      10000,       /* Размер стека задачи */
                      NULL,        /* Параметр задачи */
                      1,           /* Приоритет задачи */
                      &Task2,      /* Идентификатор задачи,
                                      чтобы ее можно было отслеживать */
                      1);          /* Ядро для выполнения задачи (1) */
    delay(500);
}

//Запускаем выполнение задачи снятия показания и нагрузка сети на ядре 0.
void Task1code( void * pvParameters )
{

    
    Serial.print("Task1 running on core ");
    //  "Задача Task2 выполняется на ядре "
    Serial.println(xPortGetCoreID());

  
    for(;;)
    {
        Call.callBlik(); // оправшиваем кнопку звонка
        bluetooth();
    }
}

void Task2code( void * pvParameters ){
  
  Serial.print("Task2 running on core ");
  //  "Задача Task2 выполняется на ядре "
  Serial.println(xPortGetCoreID());
  
  for(;;){
      security();
  }
}

void loop()
{
}

//Функция Bluetooth
void security()
{
    if (digitalRead(SECURITY) && !SECURITY_FLAG)
    {
        door.doorOpen();
        SECURITY_FLAG = true;
        Serial.println("OPEN DOOR");
        
    }
    if (!digitalRead(SECURITY) && SECURITY_FLAG)
    {
        door.doorClose();
        SECURITY_FLAG = false;
        Serial.println("CLOSE DOOR");
    }
    delay(100);
}


//Переменная принятых данных по ВТ
  int incoming;
  
//Функция Bluetooth
void bluetooth()
{
  if (ESP_BT.available()) // Проверяем, не получили ли мы что-либо от Bluetooth модуля
  {
    incoming = ESP_BT.read(); // Читаем, что получили
//    Serial.print("Received:"); 
//    Serial.println(incoming);

    if (incoming == 49)  // Если значение равно единице, открывем замок
    {
      door.doorOpen();
      ESP_BT.println("Door Open!");
    }
  
    if (incoming == 48)  // Если значение равно нулю, закрываем замок
    {
      door.doorClose();
      ESP_BT.println("Door Close!");
    }     
  }  
}
