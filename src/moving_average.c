#include "daq/moving_average.h"

bool daq_moving_average_init(daq_moving_average_t *filter,
                             float *storage,
                             size_t capacity) {
    if (filter == NULL || storage == NULL || capacity == 0u) {
        return false;
    }
    filter->samples = storage;
    filter->capacity = capacity;
    filter->count = 0u;
    filter->next = 0u;
    filter->sum = 0.0f;
    return true;
}

bool daq_moving_average_push(daq_moving_average_t *filter,
                             float sample,
                             float *average) {
    if (filter == NULL || average == NULL || filter->samples == NULL ||
        filter->capacity == 0u) {
        return false;
    }

    if (filter->count == filter->capacity) {
        filter->sum -= filter->samples[filter->next];
    } else {
        filter->count++;
    }
    filter->samples[filter->next] = sample;
    filter->sum += sample;
    filter->next = (filter->next + 1u) % filter->capacity;
    *average = filter->sum / (float)filter->count;
    return true;
}
