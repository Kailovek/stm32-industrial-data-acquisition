#ifndef DAQ_UPLINK_STATE_H
#define DAQ_UPLINK_STATE_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    DAQ_UPLINK_DISCONNECTED,
    DAQ_UPLINK_CONNECTING,
    DAQ_UPLINK_READY,
    DAQ_UPLINK_BACKOFF
} daq_uplink_status_t;

typedef struct {
    daq_uplink_status_t status;
    uint32_t base_backoff_ms;
    uint32_t max_backoff_ms;
    uint32_t retry_at_ms;
    uint8_t failure_count;
} daq_uplink_t;

bool daq_uplink_init(daq_uplink_t *uplink,
                     uint32_t base_backoff_ms,
                     uint32_t max_backoff_ms);
bool daq_uplink_should_connect(const daq_uplink_t *uplink, uint32_t now_ms);
void daq_uplink_mark_connecting(daq_uplink_t *uplink);
void daq_uplink_mark_connected(daq_uplink_t *uplink);
void daq_uplink_mark_failure(daq_uplink_t *uplink, uint32_t now_ms);

#endif
