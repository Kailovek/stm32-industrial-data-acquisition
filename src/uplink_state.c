#include "daq/uplink_state.h"

static bool time_reached(uint32_t now_ms, uint32_t target_ms) {
    return (int32_t)(now_ms - target_ms) >= 0;
}

bool daq_uplink_init(daq_uplink_t *uplink,
                     uint32_t base_backoff_ms,
                     uint32_t max_backoff_ms) {
    if (uplink == 0 || base_backoff_ms == 0u ||
        max_backoff_ms < base_backoff_ms) {
        return false;
    }
    uplink->status = DAQ_UPLINK_DISCONNECTED;
    uplink->base_backoff_ms = base_backoff_ms;
    uplink->max_backoff_ms = max_backoff_ms;
    uplink->retry_at_ms = 0u;
    uplink->failure_count = 0u;
    return true;
}

bool daq_uplink_should_connect(const daq_uplink_t *uplink, uint32_t now_ms) {
    return uplink != 0 &&
           (uplink->status == DAQ_UPLINK_DISCONNECTED ||
            (uplink->status == DAQ_UPLINK_BACKOFF &&
             time_reached(now_ms, uplink->retry_at_ms)));
}

void daq_uplink_mark_connecting(daq_uplink_t *uplink) {
    if (uplink != 0) {
        uplink->status = DAQ_UPLINK_CONNECTING;
    }
}

void daq_uplink_mark_connected(daq_uplink_t *uplink) {
    if (uplink != 0) {
        uplink->status = DAQ_UPLINK_READY;
        uplink->failure_count = 0u;
        uplink->retry_at_ms = 0u;
    }
}

void daq_uplink_mark_failure(daq_uplink_t *uplink, uint32_t now_ms) {
    uint32_t delay;
    uint8_t shifts;
    if (uplink == 0) {
        return;
    }
    if (uplink->failure_count < 31u) {
        uplink->failure_count++;
    }
    delay = uplink->base_backoff_ms;
    shifts = (uint8_t)(uplink->failure_count - 1u);
    while (shifts-- > 0u && delay < uplink->max_backoff_ms) {
        delay = delay > uplink->max_backoff_ms / 2u
                    ? uplink->max_backoff_ms
                    : delay * 2u;
    }
    if (delay > uplink->max_backoff_ms) {
        delay = uplink->max_backoff_ms;
    }
    uplink->retry_at_ms = now_ms + delay;
    uplink->status = DAQ_UPLINK_BACKOFF;
}
