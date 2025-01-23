#pragma once
#include <stdalign.h>
#include "semaphore.h"

// Help alignment to prevent false sharing.
// This will be useful as multiple threads *can* write to the same area: https://en.wikipedia.org/wiki/False_sharing
//this is only important for writing on multiple threads
#define ALIGN alignas(32)


typedef struct {
    ALIGN struct {
        float temperature;
        uint16_t humidity;
    } soil;
    ALIGN float temperature;
    ALIGN float humidity;
    ALIGN int light;
    ALIGN SemaphoreHandle_t mutex;
} sensor_data_t;

typedef struct { 
    int light;
    float temperature;
    float humidity;
    float soil_temperature;
    int soil_humidity;
} rule_limits_t;