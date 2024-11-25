/* Pico HTTPS request example *************************************************
 *                                                                            *
 *  An HTTPS client example for the Raspberry Pi Pico W                       *
 *                                                                            *
 *  A simple yet complete example C application which sends a single request  *
 *  to a web server over HTTPS and reads the resulting response.              *
 *                                                                            *
 ******************************************************************************/


/* Includes *******************************************************************/

// Pico SDK
#include "pico/stdlib.h"            // Standard library
#include "pico/cyw43_arch.h"        // Pico W wireless

// lwIP
#include "lwip/dns.h"               // Hostname resolution
#include "lwip/altcp_tls.h"         // TCP + TLS (+ HTTP == HTTPS)
#include "altcp_tls_mbedtls_structs.h"
#include "lwip/prot/iana.h"         // HTTPS port number

// Mbed TLS
#include "mbedtls/ssl.h"            // Server Name Indication TLS extension

// Pico HTTPS request example
#include "picohttps.h"              // Options, macros, forward declarations


/* Main ***********************************************************************/

void main(void){

    // Initialise standard I/O over USB
    if(!init_stdio()) return;

    // Initialise Pico W wireless hardware
    printf("Initializing CYW43\n");
    if(!init_cyw43()){
        printf("Failed to initialize CYW43\n");
        return;
    }
    printf("Initialized CYW43\n");

    // Connect to wireless network
    printf("Connecting to %s\n", PICOHTTPS_WIFI_SSID);
    if(!connect_to_network()){
        printf("Failed to connect to %s\n", PICOHTTPS_WIFI_SSID);
        cyw43_arch_deinit();            // Deinit Pico W wireless hardware
        return;
    }
    printf("Connected to %s\n", PICOHTTPS_WIFI_SSID);

    // Resolve server hostname
    ip_addr_t ipaddr;
    char* char_ipaddr;
    printf("Resolving %s\n", PICOHTTPS_HOSTNAME);
    if(!resolve_hostname(&ipaddr)){
        printf("Failed to resolve %s\n", PICOHTTPS_HOSTNAME);
                                        // TODO: Disconnect from network
        cyw43_arch_deinit();            // Deinit Pico W wireless hardware
        return;
    }
    cyw43_arch_lwip_begin();
    char_ipaddr = ipaddr_ntoa(&ipaddr);
    cyw43_arch_lwip_end();
    printf("Resolved %s (%s)\n", PICOHTTPS_HOSTNAME, char_ipaddr);

    // Establish TCP + TLS connection with server
    struct altcp_pcb* pcb = NULL;
    printf("Connecting to https://%s:%d\n", char_ipaddr, LWIP_IANA_PORT_HTTPS);
    if(!connect_to_host(&ipaddr, &pcb)){
        printf("Failed to connect to https://%s:%d\n", char_ipaddr, LWIP_IANA_PORT_HTTPS);
                                        // TODO: Disconnect from network
        cyw43_arch_deinit();            // Deinit Pico W wireless hardware
        return;
    }
    printf("Connected to https://%s:%d\n", char_ipaddr, LWIP_IANA_PORT_HTTPS);

    // Send HTTP request to server
    printf("Sending request\n");
    if(!send_request(pcb)){
        printf("Failed to send request\n");
        altcp_free_config(              // Free connection configuration
            ((struct altcp_callback_arg*)(pcb->arg))->config
        );
        altcp_free_arg(                 // Free connection callback argument
            (struct altcp_callback_arg*)(pcb->arg)
        );
        altcp_free_pcb(pcb);            // Free connection PCB
                                        // TODO: Disconnect from network
        cyw43_arch_deinit();            // Deinit Pico W wireless hardware
        return;
    }
    printf("Request sent\n");

    // Await HTTP response
    printf("Awaiting response\n");
    sleep_ms(5000);
    printf("Awaited response\n");

    // Return
    printf("Exiting\n");
    return;
    //printf("Exited 😉 \n");

}



/* Functions ******************************************************************/

// Initialise standard I/O over USB
bool init_stdio(void){
    if(!stdio_usb_init()) return false;
    stdio_set_translate_crlf(&stdio_usb, false);
    return true;
}

// Initialise Pico W wireless hardware
bool init_cyw43(void){
    return !((bool)cyw43_arch_init_with_country(PICOHTTPS_INIT_CYW43_COUNTRY));
}

// Connect to wireless network
bool connect_to_network(void){
    cyw43_arch_enable_sta_mode();
    return !(
        (bool)cyw43_arch_wifi_connect_timeout_ms(
            PICOHTTPS_WIFI_SSID,
            PICOHTTPS_WIFI_PASSWORD,
            CYW43_AUTH_WPA2_AES_PSK,
            PICOHTTPS_WIFI_TIMEOUT
        )
    );
}

// Resolve hostname
bool resolve_hostname(ip_addr_t* ipaddr){

    // Zero address
    ipaddr->addr = IPADDR_ANY;

    // Attempt resolution
    cyw43_arch_lwip_begin();
    lwip_err_t lwip_err = dns_gethostbyname(
        PICOHTTPS_HOSTNAME,
        ipaddr,
        callback_gethostbyname,
        ipaddr
    );
    cyw43_arch_lwip_end();
    if(lwip_err == ERR_INPROGRESS){

        // Await resolution
        //
        //  IP address will be made available shortly (by callback) upon DNS
        //  query response.
        //
        while(ipaddr->addr == IPADDR_ANY)
            sleep_ms(PICOHTTPS_RESOLVE_POLL_INTERVAL);
        if(ipaddr->addr != IPADDR_NONE)
            lwip_err = ERR_OK;

    }

    // Return
    return !((bool)lwip_err);

}

// Free TCP + TLS protocol control block
void altcp_free_pcb(struct altcp_pcb* pcb){
    cyw43_arch_lwip_begin();
    lwip_err_t lwip_err = altcp_close(pcb);         // Frees PCB
    cyw43_arch_lwip_end();
    while(lwip_err != ERR_OK)
        sleep_ms(PICOHTTPS_ALTCP_CONNECT_POLL_INTERVAL);
        cyw43_arch_lwip_begin();
        lwip_err = altcp_close(pcb);                // Frees PCB
        cyw43_arch_lwip_end();
}

// Free TCP + TLS connection configuration
void altcp_free_config(struct altcp_tls_config* config){
    cyw43_arch_lwip_begin();
    altcp_tls_free_config(config);
    cyw43_arch_lwip_end();
}

// Free TCP + TLS connection callback argument
void altcp_free_arg(struct altcp_callback_arg* arg){
    if(arg){
        free(arg);
    }
}

// Establish TCP + TLS connection with server
bool connect_to_host(ip_addr_t* ipaddr, struct altcp_pcb** pcb){

    // Instantiate connection configuration
    u8_t ca_cert[] = PICOHTTPS_CA_ROOT_CERT;
    cyw43_arch_lwip_begin();
    struct altcp_tls_config* config = altcp_tls_create_config_client(
        ca_cert,
        LEN(ca_cert)
    );
    cyw43_arch_lwip_end();
    if(!config) return false;

    // Instantiate connection PCB
    //
    //  Can also do this more generically using;
    //
    //    altcp_allocator_t allocator = {
    //      altcp_tls_alloc,       // Allocator function
    //      config                 // Allocator function argument (state)
    //    };
    //    altcp_new(&allocator);
    //
    //  No benefit in doing this though; altcp_tls_alloc calls altcp_tls_new
    //  under the hood anyway.
    //
    cyw43_arch_lwip_begin();
    *pcb = altcp_tls_new(config, IPADDR_TYPE_V4);
    cyw43_arch_lwip_end();
    if(!(*pcb)){
        altcp_free_config(config);
        return false;
    }

    // Configure hostname for Server Name Indication extension
    //
    //  Many servers nowadays require clients to support the [Server Name
    //  Indication[wiki-sni] (SNI) TLS extension. In this extension, the
    //  hostname is included in the in the ClientHello section of the TLS
    //  handshake.
    //
    //  Mbed TLS provides client-side support for SNI extension
    //  (`MBEDTLS_SSL_SERVER_NAME_INDICATION` option), but requires the
    //  hostname in order to do so. Unfortunately, the Mbed TLS port supplied
    //  with lwIP (ALTCP TLS) does not currently provide an interface to pass
    //  the hostname to Mbed TLS. This is a [known issue in lwIP][gh-lwip-pr].
    //
    //  As a workaround, the hostname can instead be set using the underlying
    //  Mbed TLS interface (viz. `mbedtls_ssl_set_hostname` function). This is
    //  somewhat inelegant as it tightly couples our application code to the
    //  underlying TLS library (viz. Mbed TLS). Given that the Pico SDK already
    //  tightly couples us to lwIP though, and that any fix is unlikely to be
    //  backported to the lwIP version in the Pico SDK, this doesn't feel like
    //  too much of a crime…
    //
    //  [wiki-sni]: https://en.wikipedia.org/wiki/Server_Name_Indication
    //  [gh-lwip-pr]: https://github.com/lwip-tcpip/lwip/pull/47/commits/c53c9d02036be24a461d2998053a52991e65b78e
    //
    cyw43_arch_lwip_begin();
    mbedtls_err_t mbedtls_err = mbedtls_ssl_set_hostname(
        &(
            (
                (altcp_mbedtls_state_t*)((*pcb)->state)
            )->ssl_context
        ),
        PICOHTTPS_HOSTNAME
    );
    cyw43_arch_lwip_end();
    if(mbedtls_err){
        altcp_free_pcb(*pcb);
        altcp_free_config(config);
        return false;
    }

    // Configure common argument for connection callbacks
    //
    //  N.b. callback argument must be in scope in callbacks. As callbacks may
    //  fire after current function returns, cannot declare argument locally,
    //  but rather should allocate on the heap. Must then ensure allocated
    //  memory is subsequently freed.
    //
    struct altcp_callback_arg* arg = malloc(sizeof(*arg));
    if(!arg){
        altcp_free_pcb(*pcb);
        altcp_free_config(config);
        return false;
    }
    arg->config = config;
    arg->connected = false;
    cyw43_arch_lwip_begin();
    altcp_arg(*pcb, (void*)arg);
    cyw43_arch_lwip_end();

    // Configure connection fatal error callback
    cyw43_arch_lwip_begin();
    altcp_err(*pcb, callback_altcp_err);
    cyw43_arch_lwip_end();

    // Configure idle connection callback (and interval)
    cyw43_arch_lwip_begin();
    altcp_poll(
        *pcb,
        callback_altcp_poll,
        PICOHTTPS_ALTCP_IDLE_POLL_INTERVAL
    );
    cyw43_arch_lwip_end();

    // Configure data acknowledge callback
    cyw43_arch_lwip_begin();
    altcp_sent(*pcb, callback_altcp_sent);
    cyw43_arch_lwip_end();

    // Configure data reception callback
    cyw43_arch_lwip_begin();
    altcp_recv(*pcb, callback_altcp_recv);
    cyw43_arch_lwip_end();

    // Send connection request (SYN)
    cyw43_arch_lwip_begin();
    lwip_err_t lwip_err = altcp_connect(
        *pcb,
        ipaddr,
        LWIP_IANA_PORT_HTTPS,
        callback_altcp_connect
    );
    cyw43_arch_lwip_end();

    // Connection request sent
    if(lwip_err == ERR_OK){

        // Await connection
        //
        //  Sucessful connection will be confirmed shortly in
        //  callback_altcp_connect.
        //
        while(!(arg->connected))
            sleep_ms(PICOHTTPS_ALTCP_CONNECT_POLL_INTERVAL);

    } else {

        // Free allocated resources
        altcp_free_pcb(*pcb);
        altcp_free_config(config);
        altcp_free_arg(arg);

    }

    // Return
    return !((bool)lwip_err);

}

// Send HTTP request
bool send_request(struct altcp_pcb* pcb){

    const char request[] = PICOHTTPS_REQUEST;

    // Check send buffer and queue length
    //
    //  Docs state that altcp_write() returns ERR_MEM on send buffer too small
    //  _or_ send queue too long. Could either check both before calling
    //  altcp_write, or just handle returned ERR_MEM — which is preferable?
    //
    //if(
    //  altcp_sndbuf(pcb) < (LEN(request) - 1)
    //  || altcp_sndqueuelen(pcb) > TCP_SND_QUEUELEN
    //) return -1;

    // Write to send buffer
    cyw43_arch_lwip_begin();
    lwip_err_t lwip_err = altcp_write(pcb, request, LEN(request) -1, 0);
    cyw43_arch_lwip_end();

    // Written to send buffer
    if(lwip_err == ERR_OK){

        // Output send buffer
        ((struct altcp_callback_arg*)(pcb->arg))->acknowledged = 0;
        cyw43_arch_lwip_begin();
        lwip_err = altcp_output(pcb);
        cyw43_arch_lwip_end();

        // Send buffer output
        if(lwip_err == ERR_OK){

            // Await acknowledgement
            while(
                !((struct altcp_callback_arg*)(pcb->arg))->acknowledged
            ) sleep_ms(PICOHTTPS_HTTP_RESPONSE_POLL_INTERVAL);
            if(
                ((struct altcp_callback_arg*)(pcb->arg))->acknowledged
                != (LEN(request) - 1)
            ) lwip_err = -1;

        }

    }

    // Return
    return !((bool)lwip_err);

}

// DNS response callback
void callback_gethostbyname(
    const char* name,
    const ip_addr_t* resolved,
    void* ipaddr
){
    if(resolved) *((ip_addr_t*)ipaddr) = *resolved;         // Successful resolution
    else ((ip_addr_t*)ipaddr)->addr = IPADDR_NONE;          // Failed resolution
}

// TCP + TLS connection error callback
void callback_altcp_err(void* arg, lwip_err_t err){

    // Print error code
    printf("Connection error [lwip_err_t err == %d]\n", err);

    // Free ALTCP TLS config
    if( ((struct altcp_callback_arg*)arg)->config )
        altcp_free_config( ((struct altcp_callback_arg*)arg)->config );

    // Free ALTCP callback argument
    altcp_free_arg((struct altcp_callback_arg*)arg);

}

// TCP + TLS connection idle callback
lwip_err_t callback_altcp_poll(void* arg, struct altcp_pcb* pcb){
    // Callback not currently used
    return ERR_OK;
}

// TCP + TLS data acknowledgement callback
lwip_err_t callback_altcp_sent(void* arg, struct altcp_pcb* pcb, u16_t len){
    ((struct altcp_callback_arg*)arg)->acknowledged = len;
    return ERR_OK;
}

// TCP + TLS data reception callback
lwip_err_t callback_altcp_recv(
    void* arg,
    struct altcp_pcb* pcb,
    struct pbuf* buf,
    lwip_err_t err
){

    // Store packet buffer at head of chain
    //
    //  Required to free entire packet buffer chain after processing.
    //
    struct pbuf* head = buf;

    switch(err){

        // No error receiving
        case ERR_OK:

            // Handle packet buffer chain
            //
            //  * buf->tot_len == buf->len — Last buf in chain
            //    * && buf->next == NULL — last buf in chain, no packets in queue
            //    * && buf->next != NULL — last buf in chain, more packets in queue
            //

            if(buf){

                // Print packet buffer
                u16_t i;
                while(buf->len != buf->tot_len){
                    for(i = 0; i < buf->len; i++) putchar(((char*)buf->payload)[i]);
                    buf = buf->next;
                }
                for(i = 0; i < buf->len; i++) putchar(((char*)buf->payload)[i]);
                assert(buf->next == NULL);

                // Advertise data reception
                altcp_recved(pcb, head->tot_len);

            }

            // …fall-through…

        case ERR_ABRT:

            // Free buf
            pbuf_free(head);        // Free entire pbuf chain

            // Reset error
            err = ERR_OK;           // Only return ERR_ABRT when calling tcp_abort()

    }

    // Return error
    return err;

}

// TCP + TLS connection establishment callback
lwip_err_t callback_altcp_connect(
    void* arg,
    struct altcp_pcb* pcb,
    lwip_err_t err
){
    ((struct altcp_callback_arg*)arg)->connected = true;
    return ERR_OK;
}



