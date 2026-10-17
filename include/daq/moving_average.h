#ifndef DAQ_MOVING_AVERAGE_H
#define DAQ_MOVING_AVERAGE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    float *samples;
    size_t capacity;
    size_t count;
    size_t next;
    float sum;
} daq_moving_average_t;

bool daq_moving_average_init(daq_moving_average_t *filter,
                             float *storage,
                             size_t capacity);
bool daq_moving_average_push(daq_moving_average_t *filter,
                             float sample,
                             float *average);

#endif
