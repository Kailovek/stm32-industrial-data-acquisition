# STM32 Industrial Environmental Data Acquisition

This portfolio repository presents a privacy-safe reconstruction of selected modules from an STM32F103-based industrial environmental data collector.

The original system collected wind, temperature, humidity, pressure and rainfall data over UART/RS485, maintained local and cellular network links, and handled timeouts and recovery in an RTOS-based firmware architecture.

The public code focuses on:

- Modbus RTU CRC16 and generic request validation
- deterministic sensor polling with timeout and retry handling
- fixed-window moving-average filtering without dynamic allocation
- an explicit acquisition-validity model
- bounded TCP reconnect backoff
- transport interfaces that separate application logic from MCU drivers

This is **not** the production firmware. It contains no credentials, real endpoints, device identities, proprietary protocol fields, vendor SDKs, firmware images or company-owned source code. See [README.md](README.md) for the full Chinese documentation and [PUBLICATION_SCOPE.md](PUBLICATION_SCOPE.md) for the publication boundary.

## Layout

```text
include/daq/   Public C interfaces
src/           Protocol, filtering and state-machine modules
platform/      Host-side mock transport
examples/      Synthetic demonstration entry point
docs/          Publication notes and image checklist
```

## License

The reconstruction code included in this repository is available under the [MIT License](LICENSE). The license does not apply to any excluded production firmware, proprietary material, or third-party dependency.
