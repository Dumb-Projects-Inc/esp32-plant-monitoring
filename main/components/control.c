#include "control.h"

void controlLoop(void *pvParameters ) {
    //Init stuff

    while(1) {
        //Process Stuff
        vTaskDelay(pdMS_TO_TICKS(5000)); // wait 5 seconds
    }
}