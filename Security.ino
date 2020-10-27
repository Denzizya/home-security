#include "BluetoothSerial.h"  // Заголовочный файл для последовательного Bluetooth будет добавлен по умолчанию в Arduino
BluetoothSerial ESP_BT;       // Объект для Bluetooth
#include "call.h"             // Библиотека звонка
#include "door.h"             // Библиотека электрозамка
#include "alert.h"            // Библиотека работы сирены

//Замок
  Door door(33, 32, 50);    // подключаем библиотеку электро-замка контакты (открыть, закрыть, время работы)

//Звонок
  Call Call(5, 34, 4, 300); // подключаем библиотеку звонка контакты (светодиод, кнопка, оптопара, задержка нажатия)

// Сирена
  Alert Alert(26, 35);      // оптопара сработки тревоги и реле сирены

// Охрана
  int SECURITY = 25;             // оптопара поставки/снятия охраны
  bool STATUS_SECURITY = false;  // флаг сработки тревоги

//Идентифицируем ядро 0.
  TaskHandle_t Task1;
//Идентифицируем ядро 1.  
  TaskHandle_t Task2;
  
void setup()
{
    Serial.begin(115200); // Запускаем последовательный монитор со скоростью 9600
    //Настройка блютуз
    ESP_BT.begin("Control"); // Задаем имя вашего устройства Bluetooth    

    pinMode(SECURITY, INPUT_PULLUP);

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

//Запускаем выполнение задачи звонка и блютуз на ядре 0.
void Task1code( void * pvParameters )
{
    for(;;)
    {
        Call.callBlik();  // оправшиваем кнопку звонка
        bluetooth();      // опрашиваем блютуз
    }
}

void Task2code( void * pvParameters )
{
    for(;;){
        if (digitalRead(SECURITY) && !STATUS_SECURITY)
        {
            door.doorOpen();
            STATUS_SECURITY = true;
            Serial.println("OPEN DOOR");
        }
        if (!digitalRead(SECURITY) && STATUS_SECURITY)
        {
            door.doorClose();
            STATUS_SECURITY = false;
            Serial.println("CLOSE DOOR");
        }
        delay(10);
        
        Alert.statusAlert();
        Alert.status_alert;
    }
}

void loop()
{
}

//Переменная принятых данных по ВТ
  int incoming;
  
//Функция Bluetooth
void bluetooth()
{
    if (ESP_BT.available()) // Проверяем, не получили ли мы что-либо от Bluetooth модуля
    {
        incoming = ESP_BT.read(); // Читаем, что получили
        if (incoming == 49)  // Если значение равно единице, открывем замок
        {
            door.doorOpenBT();
            ESP_BT.println("Door Open!");
        }
        
        if (incoming == 48)  // Если значение равно нулю, закрываем замок
        {
            door.doorCloseBT();
            ESP_BT.println("Door Close!");
        }     
    }  
}
