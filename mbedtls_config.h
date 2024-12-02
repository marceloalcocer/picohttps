/* Mbed TLS configuration for Pico HTTPS example ******************************
 *                                                                            *
 *  Configuration for the Mbed TLS library included in the Pico SDK and       *
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



/* System ********************************************************************/

#define MBEDTLS_HAVE_TIME



/* Mbed TLS features *********************************************************/

// Entropy
#define MBEDTLS_NO_PLATFORM_ENTROPY                 // No HW entropy source
#define MBEDTLS_ENTROPY_HARDWARE_ALT                // Custom entropy collector (pico-sdk:pico_mbedtls.c)

// Symmetric ciphers
#define MBEDTLS_CIPHER_MODE_CBC                     // Cipher block chaining
#define MBEDTLS_CIPHER_MODE_CFB                     // Cipher feedback mode
#define MBEDTLS_CIPHER_MODE_CTR                     // Counter block cipher mode
#define MBEDTLS_CIPHER_MODE_OFB                     // Output feedback mode
#define MBEDTLS_CIPHER_MODE_XTS                     // XOR-encrypt-XOR
#define MBEDTLS_CIPHER_PADDING_PKCS7                // Padding modes
#define MBEDTLS_CIPHER_PADDING_ONE_AND_ZEROS
#define MBEDTLS_CIPHER_PADDING_ZEROS_AND_LEN
#define MBEDTLS_CIPHER_PADDING_ZEROS

// Weak cipher suite removal
#define MBEDTLS_REMOVE_ARC4_CIPHERSUITES            // ARC4
#define MBEDTLS_REMOVE_3DES_CIPHERSUITES            // 3DES

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
#define MBEDTLS_ECP_DP_CURVE448_ENABLED
#define MBEDTLS_ECP_NIST_OPTIM                      // NIST optimizations
#define MBEDTLS_ECDSA_DETERMINISTIC                 // Deterministic ECDSA (more secure)

// Key exchange
#define MBEDTLS_KEY_EXCHANGE_RSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDH_ECDSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDH_RSA_ENABLED

// PKCS
#define MBEDTLS_PKCS1_V15                           // PKCS#1 v1.5 encoding
#define MBEDTLS_PKCS1_V21                           // PKCS#1 v2.1 encoding

// TLS records
#define MBEDTLS_SSL_ALL_ALERT_MESSAGES              // Send alert records
#define MBEDTLS_SSL_RECORD_CHECKING                 // Validate records

// TLS extensions
#define MBEDTLS_SSL_ENCRYPT_THEN_MAC                // TLS extension (RFC 7366)
#define MBEDTLS_SSL_EXTENDED_MASTER_SECRET          // TLS extension (RFC 7627)
#define MBEDTLS_SSL_MAX_FRAGMENT_LENGTH             // TLS extension (RFC 6066)
#define MBEDTLS_SSL_SERVER_NAME_INDICATION          // TLS extension (RFC 6066)
#define MBEDTLS_SSL_TRUNCATED_HMAC                  // TLS extension (RFC 6066)

// Protocols
#define MBEDTLS_SSL_PROTO_TLS1_2                    // Enable TLS version 1.2

// X.509
#define MBEDTLS_X509_CHECK_KEY_USAGE                // Verify keyUsage extension
#define MBEDTLS_X509_CHECK_EXTENDED_KEY_USAGE       // Verify extendedKeyUsage extension



/* Modules *******************************************************************/

// Ciphers
#define MBEDTLS_CIPHER_C                            // Symmetric cipher generic code
#define MBEDTLS_AES_C                               // AES

// Parsers
#define MBEDTLS_ASN1_PARSE_C                        // ASN1
#define MBEDTLS_PEM_PARSE_C                         // PEM
#define MBEDTLS_PK_PARSE_C                          // PK

// Hashing
#define MBEDTLS_MD_C                                // MD generic code
#define MBEDTLS_MD5_C                               // MD5
#define MBEDTLS_POLY1305_C                          // Poly1305 MAC
#define MBEDTLS_SHA256_C                            // SHA 256
#define MBEDTLS_SHA512_C                            // SHA 512

// Elliptic curves
#define MBEDTLS_ECDH_C                              // Diffie-Hellman
#define MBEDTLS_ECDSA_C                             // Signing
#define MBEDTLS_ECP_C                               // GF(p) implementation

// RSA
#define MBEDTLS_RSA_C                               // RSA

// Public Key
#define MBEDTLS_PK_C                                // Public key generic code
#define MBEDTLS_PKCS5_C                             // PKCS#5
#define MBEDTLS_PKCS12_C                            // PKCS#12

// SSL/TLS
#define MBEDTLS_SSL_TLS_C                           // TLS generic code
#define MBEDTLS_SSL_CLI_C                           // TLS client code

// X.509 certificates
#define MBEDTLS_X509_USE_C                          // Core
#define MBEDTLS_X509_CRT_PARSE_C                    // Certificate parsing

// Requirements
#define MBEDTLS_ENTROPY_C                           // for ALTCP TLS
#define MBEDTLS_BIGNUM_C                            // for define MBEDTLS_ECP_C, MBEDTLS_RSA_C, MBEDTLS_X509_USE_C
#define MBEDTLS_BASE64_C                            // for MBEDTLS_PEM_PARSE_C
#define MBEDTLS_HMAC_DRBG_C                         // for MBEDTLS_ECDSA_DETERMINISTIC
#define MBEDTLS_CTR_DRBG_C                          // for MBEDTLS_AES_C
#define MBEDTLS_OID_C                               // for MBEDTLS_RSA_C
#define MBEDTLS_ASN1_WRITE_C                        // for MBEDTLS_ECDSA_C

// Misc
#define MBEDTLS_ERROR_C                             // Error code conversion
#define MBEDTLS_PLATFORM_C                          // libc re-assignment

// Debug
//#define MBEDTLS_DEBUG_C                           // Debug functions
//#define MBEDTLS_SSL_DEBUG_ALL                     // Debug output



/* Module config *************************************************************/

