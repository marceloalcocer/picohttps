# Pico HTTPS request example

An HTTPS client example for the Raspberry Pi Pico W.

Implemented in C, leveraging the [Raspberry Pi Pico C/C++ SDK][pico-sdk].

[pico-sdk]: https://www.raspberrypi.com/documentation/pico-sdk/

## Description

There are many excellent examples of HTTP clients for the Raspberry Pi Pico W platform. The extra complexities (viz. cryptography) introduced by the TLS layer of HTTPS, make corresponding HTTPS examples somewhat less common.

This repository contains a simple yet complete example C application which sends a single request to a web server over HTTPS and reads the resulting response.

## Requirements

* [Raspberry Pi Pico C/C++ SDK][pico-sdk] (tested against 1.5.0)

## Building

### Configuration

The following minimum build-time configuration is required for correct execution;

* In [picohttps.h](picohttps.h);
  * Set [PICOHTTPS_WIFI_SSID](picohttps.h#L28) to your wireless network SSID
  * Set [PICOHTTPS_HOSTNAME](picohttps.h#L48) to the web server hostname
  * Set [PICOHTTPS_CA_ROOT_CERT](picohttps.h#L64) to the CA certificate used to sign the web server's HTTPS certificate
* In [CMakeLists.txt](CMakeLists.txt);
  * Set the [path to your Pico SDK installation](CMakeLists.txt#L18)

More detailed documentation of the above (and additional optional) configuration parameters can be found in the source files.

Sample configurations for the `lwIP` and `Mbed TLS` libraries used by the example application are provided in [lwipopts.h](lwipopts.h) and [mbedtls_config.h](mbedtls_config.h) respectively. These should work 'out-of-the-box', but can be adjusted if necessary/desired.

### Invocation

```shell
~/picohttps/$ mkdir build
~/picohttps/$ cd build
~/picohttps/build$ PICOHTTPS_WIFI_PASSWORD=mywirelesspassword cmake -D"PICO_BOARD=pico_w" ..
~/picohttps/build$ make
```

N.b. Whilst the wireless network password can be set in [picohttps.h](picohttps.h#L44), it is __strongly recommended__ to set this from the build environment instead (as shown above) to minimise the risk of disclosure (e.g. via source code commits).

## Internals

### Sources

* [picohttps.h](picohttps.h): Example application header file
* [picohttps.c](picohttps.c): Example application implementation file
* [CMakeLists.txt](CMakeLists.txt): Example application build configuration
* [lwipopts.h](lwipopts.h): lwIP library configuration
* [mbedtls_config.h](mbedtls_config.h): Mbed TLS library configuration

### Overview

The example application performs the following actions in sequence;

1. Initialise Pico W I/O
2. Initialise Pico W wireless hardware
3. Connect to wireless network
4. Resolve server hostname
5. Connect to server over TCP + TLS
6. Send HTTP request over TCP + TLS
7. Read HTTP response over TCP + TLS

The function calls from [picohttps.c:main](picohttps.c#L36) which perform these actions are not deeply nested, and are declared and documented in [picohttps.h](picohttps.h).

### Libraries

Whilst the [Raspberry Pi Pico C/C++ SDK][pico-sdk] is the only requirement for the example application, this is only because it bundles several libraries which together provide the functionality required for HTTPS;

* [cyw43-driver][gh-cyw43] — Driver for the CYW43 wireless hardware on the Pico W
* [lwIP][gh-lwip] — Network stack. Provides DHCP, DNS and TCP/IP protocols
* [Mbed TLS][gh-mbedtls] — Cryptography suite providing cryptographic primitives and certificate handling required for TLS

Library functionality used by the example application:

* Pico SDK standard I/O ([pico_stdio][pico-stdio])
  * I/O over USB
* Pico SDK wireless architecture ([pico_cyw43_arch][pico-cyw43-arch])
  * Initialization of wireless hardware driver (CYW43) and networking stack (lwIP). Leverages the `thread_safe_background` abstraction for background maintenance of driver and networking stack.
  * Connection to wireless network
* [lwIP DNS 'raw' API ][lwip-dns]
  * Server hostname resolution
* [lwIP ALTCP 'raw' API][lwip-altcp]
  * Sending/receiving data over TCP. Received data handled asynchronously in interrupt context.
* [lwIP ALTCP TLS 'raw' API][lwip-altcp-tls]
  * Transparent addition of TLS to TCP code. Leverages the ALTCP compatible [Mbed TLS port][gh-lwip-mbedtls] bundled with the lwIP code base.
* [Mbed TLS][gh-mbedtls]
  * Authentication and cryptography required for TLS. Used indirectly via ALTCP compatible [Mbed TLS port][gh-lwip-mbedtls] bundled with the lwIP code base.

[gh-cyw43]: https://github.com/georgerobotics/cyw43-driver
[gh-lwip]: https://github.com/lwip-tcpip/lwip
[gh-mbedtls]: https://github.com/Mbed-TLS/mbedtls
[pico-stdio]: https://www.raspberrypi.com/documentation/pico-sdk/runtime.html#pico_stdio
[pico-cyw43-arch]: https://www.raspberrypi.com/documentation/pico-sdk/networking.html#pico_cyw43_arch
[lwip-dns]: https://www.nongnu.org/lwip/2_1_x/group__dns.html
[lwip-altcp-tls]: https://www.nongnu.org/lwip/2_1_x/group__altcp__tls.html
[gh-lwip-mbedtls]: https://github.com/lwip-tcpip/lwip/tree/master/src/apps/altcp_tls
[lwip-altcp]: https://www.nongnu.org/lwip/2_1_x/group__altcp.html

### Notes

* For simplicity, only most basic of error handling is included;
  * All functions return booleans (`true` on success) — no error codes
  * Errors printed to stdout
* Functions used as lwIP callbacks are prefixed with `callback_` for clarity. No [lock acquisition][pico-lwip-lock] is required when calling into the lwIP API from these.
* The [single common argument][lwip-arg] passed to lwIP connection callbacks is of type `struct altcp_callback_arg` and is used for accessing/modifying application state from callbacks. See [struct altcp_callback_arg declaration](picohttps.h#L154) for further documentation.
* Dynamically allocated variables;
  * TCP + TLS connection configuration (`struct altcp_tls_config config`): Allocated by lwIP API call (`altcp_tls_create_config_client()`), freed by lwIP API call (`altcp_close()`)
  * TCP + TLS connection PCB (`struct altcp_pcb pcb`): Allocated by lwIP API call (`altcp_tls_new()`), freed by lwIP API call (`altcp_tls_free_config()`)
  * TCP + TLS connection callback common argument (`struct altcp_callback_arg arg`): Allocated explicitly (`malloc()`), freed explicitly (`free()`)
  * lwIP packet buffer chain (`struct pbuf buf`): Allocated by lwIP, freed by lwIP API call (`pbuf_free()`)
* Server response printed to stdout on reception in `callback_altcp_recv()`
* Currently no clear way to cleanly disconnect from wireless networks
* Hardcoded five second timeout awaiting a response from the server

[pico-lwip-lock]: https://www.raspberrypi.com/documentation/pico-sdk/networking.html#ga6a1c4a2015fb4c2d47d6d05fc72d4cbe
[lwip-arg]: https://www.nongnu.org/lwip/2_1_x/group__altcp.html#ga197a33af038556a04d8f27c7033d771f

## References

* [Pico SDK documentation](https://www.raspberrypi.com/documentation/pico-sdk/)
* [Pico SDK source](https://github.com/raspberrypi/pico-sdk/blob/1.5.0)
* [lwIP documentation](https://www.nongnu.org/lwip/2_1_x/index.html)
* [lwIP source](https://github.com/lwip-tcpip/lwip/blob/239918ccc173cb2c2a62f41a40fd893f57faf1d6)
* [lwIP Application Developers Manual](https://lwip.fandom.com/wiki/Category:LwIP_Application_Developers_Manual)
* [Mbed TLS documentation](https://mbed-tls.readthedocs.io/en/latest/)
* [Mbed TLS source](https://github.com/Mbed-TLS/mbedtls/blob/mbedtls-2.28)
* [Clyde Pinky](https://www.unshiu.com/posts/pico-http-client-part-iii-mbedtls)

