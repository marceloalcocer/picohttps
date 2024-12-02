/* Mbed-TLS configuration for Pico HTTPS example ******************************
 *                                                                            *
 *  Configuration for the Mbed-TLS library included in the Pico SDK and       *
 *  required for the Pico HTTPS example.                                      *
 *                                                                            *
 *  N.b. Not all options are strictly required; this is just an example       *
 *  configuration.                                                            *
 *                                                                            *
 *  https://github.com/Mbed-TLS/mbedtls/blob/v2.28.2/include/mbedtls/config.h *
 *                                                                            *
 ******************************************************************************/

/* Misc **********************************************************************/

// Workaround for some Mbed TLS source files using INT_MAX without including limits.h
#include <limits.h>

#define MBEDTLS_ALLOW_PRIVATE_ACCESS
#define MBEDTLS_SSL_OUT_CONTENT_LEN    2048

/* System ********************************************************************/

#define MBEDTLS_HAVE_TIME

/* Mbed TLS features *********************************************************/

// Entropy
#define MBEDTLS_NO_PLATFORM_ENTROPY                 // No HW entropy source
#define MBEDTLS_ENTROPY_HARDWARE_ALT                // Custom entropy collector

// Symmetric ciphers
#define MBEDTLS_CIPHER_MODE_CBC                     // Cipher block chaining

// Weak cipher suite removal

// Elliptic curves
#define MBEDTLS_ECP_DP_SECP192R1_ENABLED
#define MBEDTLS_ECP_DP_SECP224R1_ENABLED
#define MBEDTLS_ECP_DP_SECP256R1_ENABLED
#define MBEDTLS_ECP_DP_SECP384R1_ENABLED
#define MBEDTLS_ECP_DP_SECP521R1_ENABLED
#define MBEDTLS_ECP_DP_SECP192K1_ENABLED
#define MBEDTLS_ECP_DP_SECP224K1_ENABLED
#define MBEDTLS_ECP_DP_SECP256K1_ENABLED
#define MBEDTLS_ECP_DP_BP256R1_ENABLED
#define MBEDTLS_ECP_DP_BP384R1_ENABLED
#define MBEDTLS_ECP_DP_BP512R1_ENABLED
#define MBEDTLS_ECP_DP_CURVE25519_ENABLED

// Key exchange
#define MBEDTLS_KEY_EXCHANGE_RSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED

// PKCS
#define MBEDTLS_PKCS1_V15                           // PKCS#1 v1.5 encoding

// TLS extensions
#define MBEDTLS_SSL_SERVER_NAME_INDICATION          // TLS extension (RFC 6066)

// Protocols
#define MBEDTLS_SSL_PROTO_TLS1_2                    // Enable TLS version 1.2

/* Modules *******************************************************************/

// Ciphers
#define MBEDTLS_CIPHER_C                            // Symmetric cipher generic code
#define MBEDTLS_AES_C                               // AES

// Parsers
#define MBEDTLS_ASN1_PARSE_C                        // ASN1
#define MBEDTLS_PK_PARSE_C                          // PK

// Hashing
#define MBEDTLS_MD_C                                // MD generic code
#define MBEDTLS_MD5_C                               // MD5
#define MBEDTLS_SHA1_C                              // SHA 1
#define MBEDTLS_SHA224_C                            // SHA 224
#define MBEDTLS_SHA256_C                            // SHA 256
#define MBEDTLS_SHA512_C                            // SHA 512
#define MBEDTLS_SHA256_SMALLER

// Elliptic curves
#define MBEDTLS_ECDH_C                              // Diffie-Hellman
#define MBEDTLS_ECDSA_C                             // Signing
#define MBEDTLS_ECP_C                               // GF(p) implementation

// RSA
#define MBEDTLS_RSA_C                               // RSA

// Public Key
#define MBEDTLS_PK_C                                // Public key generic code
#define MBEDTLS_PKCS5_C                             // PKCS#5

// SSL/TLS
#define MBEDTLS_SSL_TLS_C                           // TLS generic code
#define MBEDTLS_SSL_CLI_C                           // TLS client code
#define MBEDTLS_SSL_SRV_C                           // TLS server code

// X.509 certificates
#define MBEDTLS_X509_USE_C                          // Core
#define MBEDTLS_X509_CRT_PARSE_C                    // Certificate parsing

// Misc
#define MBEDTLS_ENTROPY_C                           // Platform specific entropy
#define MBEDTLS_ERROR_C                             // Error code conversion
#define MBEDTLS_AES_FEWER_TABLES
#define MBEDTLS_PLATFORM_C                          // libc re-assignment
#define MBEDTLS_GCM_C
//#define MBEDTLS_DEBUG_C                           // Debug functions
//#define MBEDTLS_SSL_DEBUG_ALL                     // Debug output

// Requirements
#define MBEDTLS_BIGNUM_C                            // for define MBEDTLS_ECP_C, MBEDTLS_RSA_C, MBEDTLS_X509_USE_C
#define MBEDTLS_CTR_DRBG_C                          // for MBEDTLS_AES_C
#define MBEDTLS_OID_C                               // for MBEDTLS_RSA_C
#define MBEDTLS_ASN1_WRITE_C                        // for MBEDTLS_ECDSA_C

/* Module config *************************************************************/

