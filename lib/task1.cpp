#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#include <esp_timer.h>

const uint8_t ledPin = 25;
const uint8_t btnPin = 18;

QueueHandle_t blinkQueue;

void buttonTask(void *parameter)
{
  uint32_t event;
  int64_t lastAcceptedUs = 0;
  int64_t firstPressUs = 0;

  bool waitingSecondPress = false;

  bool lastButtonState = digitalRead(btnPin);

  event = 250;

  while (true)
  {
    int64_t taskTimeUs = esp_timer_get_time();
    int currentButtonState = digitalRead(btnPin);
    bool pressed = currentButtonState == LOW && lastButtonState == HIGH;

    int64_t pressDiff = taskTimeUs - firstPressUs;

    if (waitingSecondPress && pressDiff > 300000)
    {
      waitingSecondPress = false;
      Serial.println("Single");

      event *= 2;
      if (event > 2000)
      {
        event = 250;
      }
      xQueueSend(blinkQueue, &event, 0);
    }

    if (pressed && waitingSecondPress)
    {
      int64_t pressDiff = taskTimeUs - firstPressUs;

      if (pressDiff < 300000)
      {

        waitingSecondPress = false;
        Serial.println("Double");

        event /= 2;
        if (event <= 125)
        {
          event = 2000;
        }
        xQueueSend(blinkQueue, &event, 0);
      }
    }
    else if (pressed && !waitingSecondPress)
    {
      firstPressUs = esp_timer_get_time();
      waitingSecondPress = true;
      Serial.println("Waiting");
    }

    lastButtonState = currentButtonState;
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

void ledTask(void *parameter)
{
  TickType_t lastWakeTime = xTaskGetTickCount();
  int lastLedState = LOW;
  uint32_t event = 250;

  while (true)
  {
    bool received = xQueueReceive(blinkQueue, &event, 0) == pdTRUE;
    Serial.print("LED task. Core: ");
    Serial.println(xPortGetCoreID());
    if (received)
    {

      Serial.print("event: ");
      Serial.println(event);
    }
    digitalWrite(ledPin, lastLedState);

    lastLedState = !lastLedState;
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(event));
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(btnPin, INPUT_PULLUP);

  blinkQueue = xQueueCreate(5, sizeof(uint32_t));

  xTaskCreatePinnedToCore(
      buttonTask,
      "buttonTask",
      2048,
      nullptr,
      1,
      nullptr,
      0);

  xTaskCreatePinnedToCore(
      ledTask,
      "ledTask",
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
