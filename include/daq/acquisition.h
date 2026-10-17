#ifndef DAQ_ACQUISITION_H
#define DAQ_ACQUISITION_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    DAQ_FIELD_WIND_SPEED = 1u << 0,
    DAQ_FIELD_WIND_DIRECTION = 1u << 1,
    DAQ_FIELD_TEMPERATURE = 1u << 2,
    DAQ_FIELD_HUMIDITY = 1u << 3,
    DAQ_FIELD_PRESSURE = 1u << 4,
    DAQ_FIELD_RAINFALL = 1u << 5
} daq_field_t;

typedef struct {
    float wind_speed_mps;
    float wind_direction_deg;
    float temperature_c;
    float humidity_percent;
    float pressure_hpa;
    float rainfall_mm;
    uint32_t valid_mask;
    uint32_t timestamp_ms;
} daq_sample_t;

void daq_sample_reset(daq_sample_t *sample, uint32_t timestamp_ms);
void daq_sample_set(daq_sample_t *sample, daq_field_t field, float value);
bool daq_sample_has(const daq_sample_t *sample, daq_field_t field);

#endif
