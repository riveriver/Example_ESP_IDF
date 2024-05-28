#include "HeartbeatTask.hpp"
TaskHandle_t xHeartbeatTaskHandle = NULL;
static uint8_t ucParameterToPass;
void app_main() {
    // xTaskCreate(HeartbeatTask, "HeartbeatTask", 8192, NULL, 3, &xHeartbeatTaskHandle);
    xTaskCreatePinnedToCore(HeartbeatTask, "HeartbeatTask", 8192, NULL, 3, xHeartbeatTaskHandle, 1);
    configASSERT( xHandle );
    while(1){};
}