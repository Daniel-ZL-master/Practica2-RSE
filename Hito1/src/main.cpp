#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#define PERIOD_LED_MS 200

// put function declarations here:

void TareaUART(void *parameter)
{
  static const char *TAG = "TAREA UART";
  for (;;)
  {
    ESP_LOGI(TAG, "hola mundo");
    vTaskDelay(1000 / portTICK_RATE_MS);
  }
  ESP_LOGE(TAG, "SALIENDO DE TAREA");
  vTaskDelete(NULL);
}

void TareaLED(void *parameter)
{
  static const char *TAG = "TAREA LED";
  for (;;)
  {
    ESP_LOGI(TAG, "enciendo led");
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay(PERIOD_LED_MS / portTICK_RATE_MS);
    ESP_LOGI(TAG, "apago led");
    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay(PERIOD_LED_MS / portTICK_RATE_MS);
  }
  ESP_LOGE(TAG, "SALIENDO DE TAREA");
  vTaskDelete(NULL);
}

void setup()
{
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  xTaskCreate(TareaUART, "TareaUART", 10000, NULL, 1, NULL);
  xTaskCreate(TareaLED, "TareaLED", 10000, NULL, 2, NULL);
}

void loop()
{
  // put your main code here, to run repeatedly:
}