#ifndef DAQ_MOCK_TRANSPORT_H
#define DAQ_MOCK_TRANSPORT_H

#include "daq/transport.h"

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t last_write[256];
    size_t last_write_length;
    uint8_t scripted_read[256];
    size_t scripted_read_length;
    size_t write_count;
} daq_mock_transport_t;

void daq_mock_transport_init(daq_mock_transport_t *mock);
void daq_mock_transport_script_read(daq_mock_transport_t *mock,
                                    const uint8_t *data,
                                    size_t length);
daq_transport_t daq_mock_transport_interface(daq_mock_transport_t *mock);

#endif
