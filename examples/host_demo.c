#include "daq/acquisition.h"
#include "daq/moving_average.h"
#include "daq/sensor_scheduler.h"
#include "daq/uplink_state.h"
#include "mock_transport.h"

#include <stdio.h>

int main(void) {
    const daq_sensor_descriptor_t sensors[] = {
        {1u, 0u, 2u, 1000u, 200u, 2u},
        {2u, 0u, 1u, 1000u, 200u, 2u}
    };
    float filter_storage[4];
    float filtered_temperature;
    daq_moving_average_t filter;
    daq_sample_t sample;
    daq_sensor_scheduler_t scheduler;
    daq_mock_transport_t mock;
    daq_uplink_t uplink;

    daq_mock_transport_init(&mock);
    daq_sensor_scheduler_init(&scheduler, sensors, 2u, 0u);
    daq_transport_t transport = daq_mock_transport_interface(&mock);
    daq_sensor_scheduler_step(&scheduler, &transport, 0u);

    daq_moving_average_init(&filter, filter_storage, 4u);
    daq_moving_average_push(&filter, 23.4f, &filtered_temperature);
    daq_moving_average_push(&filter, 23.8f, &filtered_temperature);

    daq_sample_reset(&sample, 1000u);
    daq_sample_set(&sample, DAQ_FIELD_TEMPERATURE, filtered_temperature);
    daq_sample_set(&sample, DAQ_FIELD_HUMIDITY, 56.0f);

    daq_uplink_init(&uplink, 1000u, 30000u);
    daq_uplink_mark_connecting(&uplink);
    daq_uplink_mark_connected(&uplink);

    printf("Synthetic sample: temperature=%.1f C, humidity=%.1f %%\n",
           sample.temperature_c,
           sample.humidity_percent);
    printf("First generic Modbus request: address=%u, bytes=%u\n",
           (unsigned)mock.last_write[0],
           (unsigned)mock.last_write_length);
    printf("Uplink state: %s\n",
           uplink.status == DAQ_UPLINK_READY ? "ready" : "not ready");
    return 0;
}
