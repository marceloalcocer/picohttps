/* lwIP configuration for Pico HTTPS example **********************************
 *                                                                            *
 *  Configuration for the lwIP network library included in the Pico SDK and   *
 *  required for the Pico HTTPS example.                                      *
 *                                                                            *
 *  N.b. Not all options are strictly required; this is just an example       *
 *  configuration.                                                            *
 *                                                                            *
 *  https://www.nongnu.org/lwip/2_1_x/group__lwip__opts.html                  *
 *  https://github.com/lwip-tcpip/lwip/blob/master/src/include/lwip/opt.h     *
 *                                                                            *
 ******************************************************************************/

#ifndef _LWIPOPTS_EXAMPLE_COMMONH_H
#define _LWIPOPTS_EXAMPLE_COMMONH_H

/* System options *************************************************************/

// Run without OS
//
//  No OS on Pico W platform
//
#define NO_SYS                      1



/* Memory options *************************************************************/

// Disable native C-library malloc
//
//  Incompatible with `pico_cyw43_arch_lwip_threadsafe_background`
//
#define MEM_LIBC_MALLOC             0

// Byte alignment
#define MEM_ALIGNMENT               4       // bytes

// Heap size
#define MEM_SIZE                    4000    // bytes



/* Memory pool options ********************************************************/

// Max queued ARP packets
#define MEMP_NUM_ARP_QUEUE          10
//
// Max queued TCP segments
#define MEMP_NUM_TCP_SEG            32



/* ARP options ****************************************************************/

// Enable ARP support
//
//  Required for IP layer of network stack
//
#define LWIP_ARP                    1



/* ICMP options ***************************************************************/

// Enable ICMP support
//
//  Probably required for IP layer of network stack?
//
#define LWIP_ICMP                   1



/* IP options *****************************************************************/

// Enable IPv4 support
#define LWIP_IPV4                   1



/* DHCP options ***************************************************************/

// Enable DHCP support
//
//  Required for connecting to wireless network
//
#define LWIP_DHCP                   1

// Disable address conflict detection
#define LWIP_DHCP_DOES_ACD_CHECK    0

// Disable ARP check
#define DHCP_DOES_ARP_CHECK         0

/* DNS options ****************************************************************/

// Enable DNS support
//
//  Required for hostname resolution
//
#define LWIP_DNS                    1



/* UDP options ****************************************************************/

// Enable UDP support
//
//  Probably required for DNS queries?
//
#define LWIP_UDP                    1



/* TCP options ****************************************************************/

// Enable TCP support
#define LWIP_TCP                    1

// Max segment size
#define TCP_MSS                     1460

// Window size
#define TCP_WND                     (8 * TCP_MSS)

// Send buffer size
#define TCP_SND_BUF                 (8 * TCP_MSS)

// Send queue length
#define TCP_SND_QUEUELEN            ((4 * (TCP_SND_BUF) + (TCP_MSS - 1)) / (TCP_MSS))

// TCP options
#define LWIP_TCP_KEEPALIVE          1


/* ALTCP options **************************************************************/

// Enable ALTCP support
//
//  ALTCP is lwIP interface for TCP + X. In the case of the Pico HTTPS example,
//  X should be TLS, as required for HTTPS.
//
#define LWIP_ALTCP                  1

// Enable ALTCP-compatible TLS interface
//
//  i.e. Set X to TLS in ALTCP = TCP + X
//
#define LWIP_ALTCP_TLS              1

// Enable ALTCP-compatible TLS interface
//
//  A port of the Mbed-TLS library is included in lwIP. __N.b. this is not a
//  full MbedTLS distribution__, but rather simply provides an lwIP compatible
//  interface to Mbed-TLS.
//
#define LWIP_ALTCP_TLS_MBEDTLS      1



/* Mbed-TLS options ***********************************************************/

// Require TLS authentication (certificate)
//
//  Cause ignoring certificate errors leads to bad things…
//
#define ALTCP_MBEDTLS_AUTHMODE      MBEDTLS_SSL_VERIFY_REQUIRED



/* Network interface options **************************************************/

// Disable NETIF API support
//
//  Not needed. Sequential API, and therefore for platforms with OSes only.
//
#define LWIP_NETIF_API              0

// Set interface name from hostname
#define LWIP_NETIF_HOSTNAME         1

// Enable callback on interface state change
#define LWIP_NETIF_STATUS_CALLBACK  1

// Enable callback on link state change
#define LWIP_NETIF_LINK_CALLBACK    1

// Try to put all TX data in single pbuf
#define LWIP_NETIF_TX_SINGLE_PBUF   1



/* Sequntial API options ******************************************************/

// Disable socket support
//
//  Not needed. Sequential API, and therefore for platforms with OSes only.
//
#define LWIP_SOCKET                 0

// Disable netconn support
//
//  Not needed. Sequential API, and therefore for platforms with OSes only.
//
#define LWIP_NETCONN                0



/* Statistics options *********************************************************/

// Enable statistics
#define LWIP_STATS                  1

// Enable statistics display function
#define LWIP_STATS_DISPLAY          1

// Enable memory stats
#define MEM_STATS                   1

// Disable system stats
#define SYS_STATS                   0

// Disable memory pool stats
#define MEMP_STATS                  0

// Disable link stats
#define LINK_STATS                  0



/* Debug options **************************************************************/

// Enable debugging
#define LWIP_DEBUG                  1


#endif //_LWIPOPTS_EXAMPLE_COMMONH_H
