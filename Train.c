// Antonio Felipe de Melo Neto 

#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define NUM_TRAINS 5
#define NUM_CARS 5

SemaphoreHandle_t trainSemaphore; // criação do semáforo de interação entre trens e carros
SemaphoreHandle_t carSemaphore; 
SemaphoreHandle_t crossingMutex; // garante que apenas um veículo passe por vez.

typedef struct {
    int id;
    int direction; // 0 para leste-oeste, 1 para norte-sul
    TaskHandle_t taskHandle; // identificador de tarefa
} Train;

typedef struct {
    int id;
    int direction; // 0 para norte-sul, 1 para leste-oeste
    TaskHandle_t taskHandle;
} Car;

int trainCount = 0;

void trainTask(void *arg) {   // função de controle do comportamento do trem
    Train *train = (Train *)arg;
    while (1) {
        if (xSemaphoreTake(trainSemaphore, portMAX_DELAY) == pdTRUE) {
            xSemaphoreTake(crossingMutex, portMAX_DELAY);
            if (trainCount == 0 || trainCount == 1 && train->direction != 1) {
                trainCount++; // incremento da contagem de trens
                xSemaphoreGive(trainSemaphore);
                xSemaphoreGive(crossingMutex);
                printf("Trem %d se aproximando.\n", train->id);
                vTaskDelay(pdMS_TO_TICKS(2000)); // Tempo para atravessar o cruzamento
                xSemaphoreTake(crossingMutex, portMAX_DELAY);
                trainCount--; // decremento da contagem de trens
                xSemaphoreGive(crossingMutex); 
                printf("Trem %d cruzou.\n", train->id);
            }
            else { // garante que apenas um trem ou um númeor seguro de trens atravesse o cruzamento
                xSemaphoreGive(trainSemaphore);
                xSemaphoreGive(crossingMutex); // garantia do cumprimento de uma tarefa
                vTaskDelay(pdMS_TO_TICKS(1000)); // Espera e tenta novamente
            }
        }
    }
}

void carTask(void *arg) {
    Car *car = (Car *)arg;
    while (1) {
        if (xSemaphoreTake(carSemaphore, portMAX_DELAY) == pdTRUE) {
            printf("Carro %d se aproximando.\n", car->id);
            xSemaphoreTake(crossingMutex, portMAX_DELAY);
            if (trainCount == 0) {
                xSemaphoreGive(carSemaphore);
                xSemaphoreGive(crossingMutex);
                printf("Carro %d cruzou.\n", car->id);
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
        vTaskDelay(pdMS_TO_TICKS(3000)); // função responsável por realiza a contagem de trens 
                                        // que devem passar no semáforo.
    }
}

int main(void) { // configura e inicializa as tarefas para trens e carros
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

