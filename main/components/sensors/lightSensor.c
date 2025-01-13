#include "lightSensor.h"

int init_light_sensor(int ADC_CHANNEL) {
    adc1_config_width(ADC_WIDTH_BIT_12);
    //TODO: fix deprecation
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_11); //ADC1_CHANNEL_0 is on GPIO0 (GPIOzero)
    return 0;
}

int get_light_value() {
    int val = adc1_get_raw(ADC1_CHANNEL_0); //TODO: Update to be reusable for multiple channels
    return val;
}