#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

SemaphoreHandle_t testMutex;

void taskForceOne(void *param)
{
  while (true)
  {
    if (xSemaphoreTake(testMutex, portMAX_DELAY) == pdTRUE)
    {
      Serial.println("Task one has mutex");

      while (true)
      {
      }

      xSemaphoreGive(testMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void taskForceTwo(void *param)
{
  while (true)
  {
    if (xSemaphoreTake(testMutex, portMAX_DELAY) == pdTRUE)
    {
      Serial.println("Task two has mutex");

      xSemaphoreGive(testMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void setup()
{
  Serial.begin(115200);

  testMutex = xSemaphoreCreateMutex();

  if (testMutex == nullptr)
  {
    Serial.println("Mutex error");
  }
  else
  {
    Serial.println("Mutex created");
  }

  xTaskCreatePinnedToCore(
      taskForceOne,
      "taskForceOne",
      2048,
      nullptr,
      1,
      nullptr,
      0);

  xTaskCreatePinnedToCore(
      taskForceTwo,
      "taskForceTwo",
      2048,
      nullptr,
      1,
      nullptr,
      1);
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
}
