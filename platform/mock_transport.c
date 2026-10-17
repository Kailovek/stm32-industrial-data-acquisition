#include "mock_transport.h"

#include <string.h>

static int mock_write(void *context, const uint8_t *data, size_t length) {
    daq_mock_transport_t *mock = (daq_mock_transport_t *)context;
    if (mock == NULL || data == NULL || length > sizeof(mock->last_write)) {
        return -1;
    }
    memcpy(mock->last_write, data, length);
    mock->last_write_length = length;
    mock->write_count++;
    return (int)length;
}

static int mock_read(void *context, uint8_t *data, size_t capacity) {
    daq_mock_transport_t *mock = (daq_mock_transport_t *)context;
    size_t length;
    if (mock == NULL || data == NULL) {
        return -1;
    }
    length = mock->scripted_read_length < capacity
                 ? mock->scripted_read_length
                 : capacity;
    memcpy(data, mock->scripted_read, length);
    mock->scripted_read_length = 0u;
    return (int)length;
}

void daq_mock_transport_init(daq_mock_transport_t *mock) {
    if (mock != NULL) {
        memset(mock, 0, sizeof(*mock));
    }
}

void daq_mock_transport_script_read(daq_mock_transport_t *mock,
                                    const uint8_t *data,
                                    size_t length) {
    if (mock == NULL || data == NULL || length > sizeof(mock->scripted_read)) {
        return;
    }
    memcpy(mock->scripted_read, data, length);
    mock->scripted_read_length = length;
}

daq_transport_t daq_mock_transport_interface(daq_mock_transport_t *mock) {
    daq_transport_t transport;
    transport.context = mock;
    transport.write = mock_write;
    transport.read = mock_read;
    return transport;
}
