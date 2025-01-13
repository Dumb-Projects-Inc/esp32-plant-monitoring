#pragma once
#include <stdalign.h>

// Help alignment to prevent false sharing.
// This will be useful as multiple threads *can* write to the same area: https://en.wikipedia.org/wiki/False_sharing
//this is only important for writing on multiple threads
#define ALIGN alignas(32)


typedef struct {
    ALIGN struct {
        float temperature;
        float humidity;
    } soil_sensor;
    ALIGN float temperature;
    ALIGN int light;
} sensor_data_t;