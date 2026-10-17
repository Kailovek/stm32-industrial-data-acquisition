#include "reference_transport.h"

#include <string.h>

static int adapter_write(void *context, const uint8_t *data, size_t length) {
    daq_reference_transport_t *adapter = (daq_reference_transport_t *)context;
    if (adapter == NULL || data == NULL || length > sizeof(adapter->last_write)) {
        return -1;
    }
    memcpy(adapter->last_write, data, length);
    adapter->last_write_length = length;
    adapter->write_count++;
    return (int)length;
}

static int adapter_read(void *context, uint8_t *data, size_t capacity) {
    daq_reference_transport_t *adapter = (daq_reference_transport_t *)context;
    size_t length;
    if (adapter == NULL || data == NULL) {
        return -1;
    }
    length = adapter->scripted_read_length < capacity
                 ? adapter->scripted_read_length
                 : capacity;
    memcpy(data, adapter->scripted_read, length);
    adapter->scripted_read_length = 0u;
    return (int)length;
}

void daq_reference_transport_init(daq_reference_transport_t *adapter) {
    if (adapter != NULL) {
        memset(adapter, 0, sizeof(*adapter));
    }
}

void daq_reference_transport_load_response(daq_reference_transport_t *adapter,
                                           const uint8_t *data,
                                           size_t length) {
    if (adapter == NULL || data == NULL ||
        length > sizeof(adapter->scripted_read)) {
        return;
    }
    memcpy(adapter->scripted_read, data, length);
    adapter->scripted_read_length = length;
}

daq_transport_t daq_reference_transport_interface(daq_reference_transport_t *adapter) {
    daq_transport_t transport;
    transport.context = adapter;
    transport.write = adapter_write;
    transport.read = adapter_read;
    return transport;
}
