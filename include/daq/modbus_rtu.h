#ifndef DAQ_MODBUS_RTU_H
#define DAQ_MODBUS_RTU_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

uint16_t daq_modbus_crc16(const uint8_t *data, size_t length);
size_t daq_modbus_build_read_holding_request(uint8_t address,
                                             uint16_t start_register,
                                             uint16_t register_count,
                                             uint8_t output[8]);
bool daq_modbus_frame_is_valid(const uint8_t *frame, size_t length);

#endif
