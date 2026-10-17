#include "daq/acquisition.h"

#include <stddef.h>

void daq_sample_reset(daq_sample_t *sample, uint32_t timestamp_ms) {
    if (sample == NULL) {
        return;
    }
    sample->wind_speed_mps = 0.0f;
    sample->wind_direction_deg = 0.0f;
    sample->temperature_c = 0.0f;
    sample->humidity_percent = 0.0f;
    sample->pressure_hpa = 0.0f;
    sample->rainfall_mm = 0.0f;
    sample->valid_mask = 0u;
    sample->timestamp_ms = timestamp_ms;
}

void daq_sample_set(daq_sample_t *sample, daq_field_t field, float value) {
    if (sample == NULL) {
        return;
    }
    switch (field) {
        case DAQ_FIELD_WIND_SPEED: sample->wind_speed_mps = value; break;
        case DAQ_FIELD_WIND_DIRECTION: sample->wind_direction_deg = value; break;
        case DAQ_FIELD_TEMPERATURE: sample->temperature_c = value; break;
        case DAQ_FIELD_HUMIDITY: sample->humidity_percent = value; break;
        case DAQ_FIELD_PRESSURE: sample->pressure_hpa = value; break;
        case DAQ_FIELD_RAINFALL: sample->rainfall_mm = value; break;
        default: return;
    }
    sample->valid_mask |= (uint32_t)field;
}

bool daq_sample_has(const daq_sample_t *sample, daq_field_t field) {
    return sample != NULL && (sample->valid_mask & (uint32_t)field) != 0u;
}
