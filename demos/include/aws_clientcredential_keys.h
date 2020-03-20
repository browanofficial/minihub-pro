/*
 * Amazon FreeRTOS V201912.00
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

#ifndef AWS_CLIENT_CREDENTIAL_KEYS_H
#define AWS_CLIENT_CREDENTIAL_KEYS_H

/*
 * @brief PEM-encoded client certificate.
 *
 * @todo If you are running one of the Amazon FreeRTOS demo projects, set this
 * to the certificate that will be used for TLS client authentication.
 *
 * @note Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 */
#define keyCLIENT_CERTIFICATE_PEM \
"-----BEGIN CERTIFICATE-----\n"\
"MIIDWjCCAkKgAwIBAgIVAJwMua/cvMnL15yRAWdpNofuTjaPMA0GCSqGSIb3DQEB\n"\
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n"\
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMDAxMTkwNzA5\n"\
"MTNaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n"\
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDgacMZjPXBYQKNBX1s\n"\
"BKwy4nbBuubUThjyqgTTdAq/dZ90iEF6yKJ1YvBfcOH/DuqUXYddGmvNGfzNQv4e\n"\
"WjTbxfUN+zjBzWbqAZSZYkbXu7QBXE4WXMEHHcNNOjCoBAQpmahT3fM78mpce5tB\n"\
"+b5LvPwGL1hLQrMgXQ+omazfOGvCna6DT6ty8D8urgEUISnwqyKIMkloHaPzlIu8\n"\
"/vW/3JqlJrkgnvQCFV+FER7+d0BYtR96REXeIJv++W30uInGvDXyEdZ2XpTXlMje\n"\
"5gpHf3nJ4f73SoGX9GwBmNiUOk5mjTylL8oFIwyEfm3h8BCCAqajAuNebpV2ImDa\n"\
"40ABAgMBAAGjYDBeMB8GA1UdIwQYMBaAFGPL2Uq9CZ6ng+JJGOjT/Z3qcDS2MB0G\n"\
"A1UdDgQWBBS4L5WeOKQ7XGkvV3PQln0yvDRzwTAMBgNVHRMBAf8EAjAAMA4GA1Ud\n"\
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAnzbC337m3+QZy0PrS3x74jc7\n"\
"0sCEF73+qOOgRC3v34FgPlI+Sy8ccVuaRcaKX8TVS9M6Z+E5nU5TZ3zRMPP+R6q2\n"\
"LVfs+qkAe/WFIEhTq2H1KJl4419tnQC97mZZOaNat1IlQGimtHUgASR+I2emTPQs\n"\
"QMbVwh1UPt1ZGmmHFbZWFnncDQcfogB4BGk/Vl03rTvvSEtby18udhMG95+c95X9\n"\
"3IeauEHY4+ZAyicWhf3VMZAJek93/cyn862qZpMhiu+cmguqQ0BoXzVZBEMCWo4b\n"\
"YWLQT7X9kG85uN3yiWlkyRwksx/JejBRjqsR++gdUp6GkHdsNkbcpSMluBV+xw==\n"\
"-----END CERTIFICATE-----\n"

/*
 * @brief PEM-encoded issuer certificate for AWS IoT Just In Time Registration (JITR).
 *
 * @todo If you are using AWS IoT Just in Time Registration (JITR), set this to
 * the issuer (Certificate Authority) certificate of the client certificate above.
 *
 * @note This setting is required by JITR because the issuer is used by the AWS
 * IoT gateway for routing the device's initial request. (The device client
 * certificate must always be sent as well.) For more information about JITR, see:
 *  https://docs.aws.amazon.com/iot/latest/developerguide/jit-provisioning.html,
 *  https://aws.amazon.com/blogs/iot/just-in-time-registration-of-device-certificates-on-aws-iot/.
 *
 * If you're not using JITR, set below to NULL.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 */
#define keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM  NULL

/*
 * @brief PEM-encoded client private key.
 *
 * @todo If you are running one of the Amazon FreeRTOS demo projects, set this
 * to the private key that will be used for TLS client authentication.
 *
 * @note Must include the PEM header and footer:
 * "-----BEGIN RSA PRIVATE KEY-----\n"\
 * "...base64 data...\n"\
 * "-----END RSA PRIVATE KEY-----\n"
 */
#define keyCLIENT_PRIVATE_KEY_PEM \
"-----BEGIN RSA PRIVATE KEY-----\n"\
"MIIEowIBAAKCAQEA4GnDGYz1wWECjQV9bASsMuJ2wbrm1E4Y8qoE03QKv3WfdIhB\n"\
"esiidWLwX3Dh/w7qlF2HXRprzRn8zUL+Hlo028X1Dfs4wc1m6gGUmWJG17u0AVxO\n"\
"FlzBBx3DTTowqAQEKZmoU93zO/JqXHubQfm+S7z8Bi9YS0KzIF0PqJms3zhrwp2u\n"\
"g0+rcvA/Lq4BFCEp8KsiiDJJaB2j85SLvP71v9yapSa5IJ70AhVfhREe/ndAWLUf\n"\
"ekRF3iCb/vlt9LiJxrw18hHWdl6U15TI3uYKR395yeH+90qBl/RsAZjYlDpOZo08\n"\
"pS/KBSMMhH5t4fAQggKmowLjXm6VdiJg2uNAAQIDAQABAoIBAQC4nCikZ0JwP7bS\n"\
"ffasmrdx8LzTKEbl1g1N6zGnJeoFkOSI/+Il+NE0khy91uYDZajyPnKx059lDYDa\n"\
"9saH/5tGRHYQd188hxWnYX92ItO2ezJcgCu417rveLuGV11ZilxGQKKpw4dqBEUd\n"\
"KY4SWzoBokkNaXtQBmHHOOc9wpXk1kGvZSvxHNAeVFxvK/PJe3hTx5VXYDXIV5S7\n"\
"6FCpDELGiBEljwCuIljDrCOA6mvMFpO4qImWoLe3HsogR2MCOVH3O2o71DXP6xy1\n"\
"1Z/XiKo0gfsx4Wvh8RqzclEKlN3T+U/6mD9IPTuQQxVKc/o2KsS3hxVDpwT/us1B\n"\
"sDCV5VcxAoGBAPgyO6Bo9H1nwHSkzjsyVbrxEAT0dHdlE4xEpjBG2pyBjPjAF+Rw\n"\
"rS8LSCn1+Tu8+UkRm+IlqDP9UeW6MmFehceEkaQfcTm50Q7RdflrJVvn5Y9nJnj3\n"\
"dht6tugmT1uf6izDNt8mAq9AZH0E3jCz6+SE3VGCgMDGqBFCWwKa4V/VAoGBAOd4\n"\
"GH8EsfcQ70kPyVFqiaBlELHSc2v8PiTeBoAJYC8VW2BvanRib7FEhNtt8yytguyV\n"\
"TuBQQQDptVNvcbNB0CSx2fQ3vzyAEquQn+RkTcHQAk48SV+MxYAId0vBPOJJKBak\n"\
"JfS52YAmKf646MR6zq++7IJv+KgYVs5LOC8TCSF9AoGAPnUFpDxFKjmtqKzAaQ0l\n"\
"ncGD8ntBw7nfzXCJEaUC5leDjiVo3cetDSWdp1GKwomvgb+4ppQrW8nnjhPpEa+j\n"\
"oq2tqMtwxsIH5d63/WgFIrecgRXIDLDZO9kJpind1oTLfTAy4rzB+DAQGZXcBA2C\n"\
"0yLMPvLES76yFIXKOUGYbXECgYBLkXXN4iaH1suIOtMq9XFBID23EFIGay+Osb3F\n"\
"h0oTlRFkBnvmy0xTv17wlJDTgUJBIu3BZfIS7jn4p2WjdFUT5cyWYqzYf6eYw3qw\n"\
"Ad8w60wVZrE9tKO2DQ2Feez6o4+m9Kg/oX1UzSDgvFWh78YSsm5cl1nC509Pwlbx\n"\
"Z3GPsQKBgCSFNwaPyNBNi1RVpJ+WYq4UY0uReu4iCEYDmkoWwLpIJG3D45F1yl6g\n"\
"0B0+c/Vbdxb8cKU5b+0TPjp33wEGOrNnUKhXWff7IDBancYvaD3a5+eF7R718mD3\n"\
"mD+urX2u6limQfsrLv5Dv2DWH1wP7HSYKDZ5L/uE0VtdI7d3Qc/1\n"\
"-----END RSA PRIVATE KEY-----\n"


#endif /* AWS_CLIENT_CREDENTIAL_KEYS_H */
