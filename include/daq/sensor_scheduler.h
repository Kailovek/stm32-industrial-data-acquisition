#ifndef DAQ_SENSOR_SCHEDULER_H
#define DAQ_SENSOR_SCHEDULER_H

#include "daq/transport.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t address;
    uint16_t start_register;
    uint16_t register_count;
    uint32_t period_ms;
    uint32_t timeout_ms;
    uint8_t max_retries;
} daq_sensor_descriptor_t;

typedef enum {
    DAQ_POLL_IDLE,
    DAQ_POLL_WAITING
} daq_poll_state_t;

typedef struct {
    const daq_sensor_descriptor_t *sensors;
    size_t sensor_count;
    size_t current;
    uint32_t next_poll_ms;
    uint32_t deadline_ms;
    uint8_t retries;
    uint32_t timeout_count;
    daq_poll_state_t state;
} daq_sensor_scheduler_t;

bool daq_sensor_scheduler_init(daq_sensor_scheduler_t *scheduler,
                               const daq_sensor_descriptor_t *sensors,
                               size_t sensor_count,
                               uint32_t now_ms);
bool daq_sensor_scheduler_step(daq_sensor_scheduler_t *scheduler,
                               daq_transport_t *transport,
                               uint32_t now_ms);
bool daq_sensor_scheduler_accept(daq_sensor_scheduler_t *scheduler,
                                 const uint8_t *frame,
                                 size_t length,
                                 uint32_t now_ms);

#endif
