#include "daq/modbus_rtu.h"

uint16_t daq_modbus_crc16(const uint8_t *data, size_t length) {
    uint16_t crc = 0xFFFFu;
    size_t index;
    unsigned bit;

    if (data == NULL && length != 0u) {
        return 0u;
    }

    for (index = 0; index < length; ++index) {
        crc ^= data[index];
        for (bit = 0; bit < 8u; ++bit) {
            crc = (crc & 1u) != 0u
                      ? (uint16_t)((crc >> 1u) ^ 0xA001u)
                      : (uint16_t)(crc >> 1u);
        }
    }
    return crc;
}

size_t daq_modbus_build_read_holding_request(uint8_t address,
                                             uint16_t start_register,
                                             uint16_t register_count,
                                             uint8_t output[8]) {
    uint16_t crc;
    if (output == NULL || address == 0u || register_count == 0u) {
        return 0u;
    }

    output[0] = address;
    output[1] = 0x03u;
    output[2] = (uint8_t)(start_register >> 8u);
    output[3] = (uint8_t)start_register;
    output[4] = (uint8_t)(register_count >> 8u);
    output[5] = (uint8_t)register_count;
    crc = daq_modbus_crc16(output, 6u);
    output[6] = (uint8_t)crc;
    output[7] = (uint8_t)(crc >> 8u);
    return 8u;
}

bool daq_modbus_frame_is_valid(const uint8_t *frame, size_t length) {
    uint16_t expected;
    uint16_t received;
    if (frame == NULL || length < 4u) {
        return false;
    }
    expected = daq_modbus_crc16(frame, length - 2u);
    received = (uint16_t)frame[length - 2u] |
               (uint16_t)((uint16_t)frame[length - 1u] << 8u);
    return expected == received;
}
