#include "fctfprm.h"
#include "keycore.h"
#include <Arduino.h>

// Création d'une instance globale de ScreenManager
ScreenManager screen;
KeyCoreDll keyCore;

// Task handles
TaskHandle_t TaskCore0Handle;
TaskHandle_t TaskCore1Handle;

// Task for Core 0
void TaskCore0(void *pvParameters) {
  screen.initstart();
  // Main loop for the task
  while (1) {
    // Add a delay to avoid WDT timeout
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1-second delay
  }
}

// Task for Core 1
void TaskCore1(void *pvParameters) {
  keyCore.begin();
  while (1) {
    keyCore.ChecksendKey();
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Setup Start");
  // Create tasks and pin them to specific cores
  xTaskCreatePinnedToCore(TaskCore0, "TaskCore0", 40000, NULL, 1, &TaskCore0Handle, 0); // Core 0
  xTaskCreatePinnedToCore(TaskCore1, "TaskCore1", 20000, NULL, 1, &TaskCore1Handle, 1); // Core 1
}

void loop() {
  // Loop is empty as tasks are managed by FreeRTOS
}
