#ifndef DAQ_TRANSPORT_H
#define DAQ_TRANSPORT_H

#include <stddef.h>
#include <stdint.h>

typedef int (*daq_transport_write_fn)(void *context,
                                      const uint8_t *data,
                                      size_t length);
typedef int (*daq_transport_read_fn)(void *context,
                                     uint8_t *data,
                                     size_t capacity);

typedef struct {
    void *context;
    daq_transport_write_fn write;
    daq_transport_read_fn read;
} daq_transport_t;

#endif
