# CMake configuration for Pico HTTPS example ###################################
#                                                                              #
#   Configuration for the CMake build system used to build the Pico HTTPS      #
#   example.                                                                   #
#                                                                              #
#   To be used with the Pico SDK.                                              #
#                                                                              #
################################################################################

cmake_minimum_required(VERSION 3.13)

# Include Pico SDK CMake macros
#
#   Adjust the path below to the local Pico SDK installation
#
#   https://github.com/raspberrypi/pico-sdk/blob/master/pico_sdk_init.cmake
#
include(/usr/local/src/pico-sdk/pico_sdk_init.cmake)

# Declare CMake project
project(picohttps)

# Initialize Pico SDK
#
#   Defined in Pico SDK macros — must be called after macro include.
#
#   Requires CMake project — must be called after project declaration.
#
pico_sdk_init()

# Define build inputs/outputs
#
#   https://cmake.org/cmake/help/latest/command/add_executable.html
#
add_executable(

    # Target
    picohttps

    # Source
    picohttps.c

)

# Configure preprocessor directives
#
#   For compile-time (i.e. Pico SDK, Appexndix B) configuration.
#
#   https://cmake.org/cmake/help/latest/command/target_compile_definitions.html
#
target_compile_definitions(

    # Target
    picohttps

    # Standard I/O over USB delay configuration
    #
    #   Pico SDK option
    #
    PRIVATE PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS=3000

    # Wireless network password (from environment)
    #
    #   Can also be defined directly in source (picohttps.h), but recommended
    #   to set here from environment.
    #
    PRIVATE PICOHTTPS_WIFI_PASSWORD=\"$ENV{PICOHTTPS_WIFI_PASSWORD}\"

)

# Configure preprocessor search paths
#
#   To include additional header files (i.e. external to Pico SDK)
#
target_include_directories(

    # Target
    picohttps

    # Current working directoy
    #
    #   To include header files local to project, viz;
    #
    #   - picohttps.h               # Local project header
    #   - lwipopts.h                # lwIP library configuration
    #   - mbedtls_config.h          # Mbed TLS library configuration
    #
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}

    # Mbed TLS port includes
    #
    #   Internal headers for the Mbed TLS port provided with lwIP. They are not
    #   typically in the pre-processor search path, but are required when using
    #   underlying Mbed TLS interfaces not exposed by the Mbed TLS port (more
    #   specifically, casting for an `mbedtls_ssl_set_hostname` call for Server
    #   Name Indication TLS extension configuration).
    #
    #   See the `connect_to_host` function in `picohttps.c` for details.
    #
    ${PICO_LWIP_PATH}/src/apps/altcp_tls/

)

# Configure linker
#
#   To link (statically) to additional libraries files (i.e. external to Pico SDK)
#
#   https://cmake.org/cmake/help/latest/command/target_link_libraries.html
#
target_link_libraries(

    # Target
    picohttps

    # C standard library
    pico_stdlib

    # Standard I/O over USB
    pico_stdio_usb

    # Pico W wireless libraries
    #
    #   Pulls in libraries for hardware driver (`pico_cyw43_driver`) and TCP/IP
    #   stack (`pico_lwip`).
    #
    pico_cyw43_arch_lwip_threadsafe_background

    # TLS library
    #
    #   Required for HTTPS (= TCP + TLS + HTTP).
    #
    #   A port of the Mbed TLS library is included in lwIP, and therefore in
    #   the Pico SDK. __N.b. this is not a full MbedTLS distribution__, but
    #   rather simply provides an lwIP compatible interface to Mbed TLS.
    #
    pico_lwip_mbedtls
    #
    #   A full Mbed TLS distributiom is however included in the Pico SDK
    #   (currently as a submodule). This is currently (Pico SDK 1.5.0) not well
    #   documented however.
    #
    pico_mbedtls                                 # mbedtls distributed with pico-sdk

)


# Configure binary output
#
#
#   Request additional binary formats (.uf2, .hex, etc.) to be built.
#
#   Defined in Pico SDK macros.
#
#   https://github.com/raspberrypi/pico-sdk/blob/master/src/rp2_common.cmake
#
pico_add_extra_outputs(

    # Target
    picohttps

)


