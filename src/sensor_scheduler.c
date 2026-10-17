#include "daq/sensor_scheduler.h"

#include "daq/modbus_rtu.h"

static bool time_reached(uint32_t now_ms, uint32_t target_ms) {
    return (int32_t)(now_ms - target_ms) >= 0;
}

static bool send_current(daq_sensor_scheduler_t *scheduler,
                         daq_transport_t *transport,
                         uint32_t now_ms) {
    uint8_t request[8];
    const daq_sensor_descriptor_t *sensor = &scheduler->sensors[scheduler->current];
    const size_t length = daq_modbus_build_read_holding_request(
        sensor->address, sensor->start_register, sensor->register_count, request);
    if (length == 0u || transport == NULL || transport->write == NULL ||
        transport->write(transport->context, request, length) != (int)length) {
        return false;
    }
    scheduler->state = DAQ_POLL_WAITING;
    scheduler->deadline_ms = now_ms + sensor->timeout_ms;
    return true;
}

bool daq_sensor_scheduler_init(daq_sensor_scheduler_t *scheduler,
                               const daq_sensor_descriptor_t *sensors,
                               size_t sensor_count,
                               uint32_t now_ms) {
    if (scheduler == NULL || sensors == NULL || sensor_count == 0u) {
        return false;
    }
    scheduler->sensors = sensors;
    scheduler->sensor_count = sensor_count;
    scheduler->current = 0u;
    scheduler->next_poll_ms = now_ms;
    scheduler->deadline_ms = now_ms;
    scheduler->retries = 0u;
    scheduler->timeout_count = 0u;
    scheduler->state = DAQ_POLL_IDLE;
    return true;
}

bool daq_sensor_scheduler_step(daq_sensor_scheduler_t *scheduler,
                               daq_transport_t *transport,
                               uint32_t now_ms) {
    const daq_sensor_descriptor_t *sensor;
    if (scheduler == NULL || scheduler->sensor_count == 0u) {
        return false;
    }

    sensor = &scheduler->sensors[scheduler->current];
    if (scheduler->state == DAQ_POLL_IDLE) {
        return time_reached(now_ms, scheduler->next_poll_ms)
                   ? send_current(scheduler, transport, now_ms)
                   : false;
    }

    if (!time_reached(now_ms, scheduler->deadline_ms)) {
        return false;
    }
    if (scheduler->retries < sensor->max_retries) {
        scheduler->retries++;
        return send_current(scheduler, transport, now_ms);
    }

    scheduler->timeout_count++;
    scheduler->current = (scheduler->current + 1u) % scheduler->sensor_count;
    scheduler->retries = 0u;
    scheduler->state = DAQ_POLL_IDLE;
    scheduler->next_poll_ms = now_ms;
    return true;
}

bool daq_sensor_scheduler_accept(daq_sensor_scheduler_t *scheduler,
                                 const uint8_t *frame,
                                 size_t length,
                                 uint32_t now_ms) {
    const daq_sensor_descriptor_t *sensor;
    if (scheduler == NULL || scheduler->state != DAQ_POLL_WAITING ||
        !daq_modbus_frame_is_valid(frame, length)) {
        return false;
    }
    sensor = &scheduler->sensors[scheduler->current];
    if (frame[0] != sensor->address || (frame[1] & 0x7Fu) != 0x03u ||
        (frame[1] & 0x80u) != 0u) {
        return false;
    }
    scheduler->current = (scheduler->current + 1u) % scheduler->sensor_count;
    scheduler->retries = 0u;
    scheduler->state = DAQ_POLL_IDLE;
    scheduler->next_poll_ms = now_ms + sensor->period_ms;
    return true;
}
