#include <Arduino.h>

#define LED_PIN       5
#define QUEUE_SIZE    20
#define GEN_PERIOD    100    // ms
#define DISP_PERIOD   1000   // ms
#define LED_ON_TIME   200    // ms
static QueueHandle_t xDataQueue = NULL;

void vDataGeneratorTask(void *pvParameters)
{
    int32_t dato;

    while (1)
    {
        dato = (int32_t)random(0, 1001);  // 0–1000

        if (xQueueSend(xDataQueue, &dato, 0) != pdTRUE)
        {
            Serial.println("[WARN] Queue llena – dato descartado");
        }

        vTaskDelay(pdMS_TO_TICKS(GEN_PERIOD));
    }
}
void vUartDisplayTask(void *pvParameters)
{
    int32_t dato;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1)
    {
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(DISP_PERIOD));

        /* Encender LED */
        digitalWrite(LED_PIN, HIGH);

        /* Vaciar queue e imprimir */
        int count = 0;
        Serial.println("──── Muestras del último segundo ────");

        while (xQueueReceive(xDataQueue, &dato, 0) == pdTRUE)
        {
            Serial.printf("  [%2d] %d\n", count + 1, (int)dato);
            count++;
        }

        Serial.printf("  Total: %d muestras\n\n", count);

        /* Apagar LED tras 200 ms */
        vTaskDelay(pdMS_TO_TICKS(LED_ON_TIME));
        digitalWrite(LED_PIN, LOW);
    }
}

void setup()
{
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    /* Crear queue */
    xDataQueue = xQueueCreate(QUEUE_SIZE, sizeof(int32_t));
    if (xDataQueue == NULL)
    {
        Serial.println("[ERROR] No se pudo crear la queue");
        while (1); // detener ejecución
    }

    /* Crear tareas, una por núcleo */
    xTaskCreatePinnedToCore(
        vDataGeneratorTask,
        "DataGen",
        2048,
        NULL,
        1,      // prioridad
        NULL,
        0       // Core 0
    );

    xTaskCreatePinnedToCore(
        vUartDisplayTask,
        "UartDisp",
        4096,
        NULL,
        4,      // prioridad
        NULL,
        1       // Core 1
    );
}

void loop()
{
}