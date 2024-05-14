// Antonio Felipe de Melo Neto 

#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define NUM_TRAINS 5
#define NUM_CARS 5

SemaphoreHandle_t trainSemaphore;
SemaphoreHandle_t carSemaphore;
SemaphoreHandle_t crossingMutex;

typedef struct {
    int id;
    int direction; // 0 para leste-oeste, 1 para norte-sul
    TaskHandle_t taskHandle;
} Train;

typedef struct {
    int id;
    int direction; // 0 para norte-sul, 1 para leste-oeste
    TaskHandle_t taskHandle;
} Car;

int trainCount = 0;

void trainTask(void *arg) {
    Train *train = (Train *)arg;
    while (1) {
        if (xSemaphoreTake(trainSemaphore, portMAX_DELAY) == pdTRUE) {
            xSemaphoreTake(crossingMutex, portMAX_DELAY);
            if (trainCount == 0 || trainCount == 1 && train->direction != 1) {
                trainCount++;
                xSemaphoreGive(trainSemaphore);
                xSemaphoreGive(crossingMutex);
                printf("Train %d approaching the intersection.\n", train->id);
                vTaskDelay(pdMS_TO_TICKS(2000)); // Tempo para atravessar o cruzamento
                xSemaphoreTake(crossingMutex, portMAX_DELAY);
                trainCount--;
                xSemaphoreGive(crossingMutex);
                printf("Train %d crossed the intersection.\n", train->id);
            }
            else {
                xSemaphoreGive(trainSemaphore);
                xSemaphoreGive(crossingMutex);
                vTaskDelay(pdMS_TO_TICKS(1000)); // Espera e tenta novamente
            }
        }
    }
}

void carTask(void *arg) {
    Car *car = (Car *)arg;
    while (1) {
        if (xSemaphoreTake(carSemaphore, portMAX_DELAY) == pdTRUE) {
            printf("Car %d approaching the intersection.\n", car->id);
            xSemaphoreTake(crossingMutex, portMAX_DELAY);
            if (trainCount == 0) {
                xSemaphoreGive(carSemaphore);
                xSemaphoreGive(crossingMutex);
                printf("Car %d crossed the intersection.\n", car->id);
            }
            else {
                xSemaphoreGive(crossingMutex);
                vTaskDelay(pdMS_TO_TICKS(1000)); // Espera e tenta novamente
            }
        }
    }
}

void vMonitorTask(void *pvParameters) {
    while (1) {
        xSemaphoreTake(crossingMutex, portMAX_DELAY);
        printf("Train count: %d\n", trainCount);
        xSemaphoreGive(crossingMutex);
        vTaskDelay(pdMS_TO_TICKS(3000)); // Atualiza a cada 3 segundos
    }
}

int main(void) {
    trainSemaphore = xSemaphoreCreateMutex();
    carSemaphore = xSemaphoreCreateMutex();
    crossingMutex = xSemaphoreCreateMutex();

    Train trains[NUM_TRAINS];
    Car cars[NUM_CARS];

    for (int i = 0; i < NUM_TRAINS; i++) {
        trains[i].id = i + 1;
        trains[i].direction = i % 2;
        xTaskCreate(trainTask, "Train Task", configMINIMAL_STACK_SIZE, &trains[i], 2, &trains[i].taskHandle);
    }

    for (int i = 0; i < NUM_CARS; i++) {
        cars[i].id = i + 1;
        cars[i].direction = i % 2;
        xTaskCreate(carTask, "Car Task", configMINIMAL_STACK_SIZE, &cars[i], 1, &cars[i].taskHandle);
    }

    xTaskCreate(vMonitorTask, "Monitor Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1);

    return 0;
}

