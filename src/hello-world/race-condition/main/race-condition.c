/**
 * @file    race-condition-demo.c
 * @brief   Race condition demonstration using ESP-IDF v6.0.
 */

#include <stdio.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TASK_ITERATIONS    (100000U)
#define TASK_DELAY_MS      (1U)

/* Shared resource */
static volatile uint32_t g_sharedCounter = 0U;

/**
 * @brief Task that increments the shared counter.
 */
static void CounterTask(void *pvParameters)
{
    const char *taskName = (const char *)pvParameters;

    for (uint32_t i = 0U; i < TASK_ITERATIONS; i++)
    {
        /*
         * Non-atomic operation:
         * Read -> Modify -> Write
         */
        g_sharedCounter++;

        if ((i % 1000U) == 0U)
        {
            vTaskDelay(pdMS_TO_TICKS(TASK_DELAY_MS));
        }
    }

    printf("%s finished\n", taskName);

    vTaskDelete(NULL);
}

/**
 * @brief Main application entry point.
 */
void app_main(void)
{
    printf("\n");
    printf("=====================================\n");
    printf("     ESP32 Race Condition Demo\n");
    printf("=====================================\n");

    printf("Expected final value: %u\n",
           (TASK_ITERATIONS * 2U));

    xTaskCreate(
        CounterTask,
        "Task_A",
        2048,
        (void *)"Task_A",
        5,
        NULL);

    xTaskCreate(
        CounterTask,
        "Task_B",
        2048,
        (void *)"Task_B",
        5,
        NULL);

    /* Allow tasks to finish */
    vTaskDelay(pdMS_TO_TICKS(5000));

    printf("Actual final value: %lu\n",
           (unsigned long)g_sharedCounter);
}