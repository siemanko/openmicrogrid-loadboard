#include "ReadingModel.h"


void on_output_current_reading(double reading) {
    output_current = ADC_FORGET * reading +
                     (1.0 - ADC_FORGET) * output_current;
}

void on_network_voltage_reading(double reading) {
    network_voltage = ADC_FORGET * reading +
                     (1.0 - ADC_FORGET) * network_voltage;
}

void on_output_voltage_reading(double reading) {
    output_voltage = ADC_FORGET * reading +
                     (1.0 - ADC_FORGET) * output_voltage;
}

void on_phone_voltage_reading(double reading) {
    phone_voltage = ADC_FORGET * reading +
                     (1.0 - ADC_FORGET) * phone_voltage;
}