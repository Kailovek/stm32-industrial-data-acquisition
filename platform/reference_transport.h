#ifndef DAQ_REFERENCE_TRANSPORT_H
#define DAQ_REFERENCE_TRANSPORT_H

#include "daq/transport.h"

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t last_write[256];
    size_t last_write_length;
    uint8_t scripted_read[256];
    size_t scripted_read_length;
    size_t write_count;
} daq_reference_transport_t;

void daq_reference_transport_init(daq_reference_transport_t *adapter);
void daq_reference_transport_load_response(daq_reference_transport_t *adapter,
                                           const uint8_t *data,
                                           size_t length);
daq_transport_t daq_reference_transport_interface(daq_reference_transport_t *adapter);

#endif
