/*
 *  --- Revised 3-Clause BSD License ---
 *  Copyright (C) 2016-2019, SEMTECH (International) AG.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *      * Redistributions of source code must retain the above copyright notice,
 *        this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above copyright notice,
 *        this list of conditions and the following disclaimer in the documentation
 *        and/or other materials provided with the distribution.
 *      * Neither the name of the copyright holder nor the names of its contributors
 *        may be used to endorse or promote products derived from this software
 *        without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL SEMTECH BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _sys_linux_h_
#define _sys_linux_h_

#include "rt.h"

#define EXIT_NOP          6
#define FATAL_GENERIC    30
#define FATAL_PTHREAD    31
#define FATAL_NOLOGGING  32
#define FATAL_MAX        40

#if defined(CFG_esp32_porting)
#define DT_LNK			10
#endif

#if defined(CFG_esp32_porting)
#include <sys/syslimits.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <signal.h>

#define TB300_BS_VERSION "0.1.2"

#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC    1

#define BASICSTATION_TOP_PATH           "/basicstation/"
#define BASICSTATION_HOME_PATH          "."
#define BASICSTATION_TEMP_PATH          "./tmp"
#define BASICSTATION_FILE_VERSION       "version.txt"
#define BASICSTATION_FILE_CONF          "station.conf"
#define BASICSTATION_CUPS_BOOT_URI      "cups-boot.uri"
#define BASICSTATION_CUPS_BOOT_TRUST    "cups-boot.trust"
#define BASICSTATION_CUPS_BOOT_CERT     "cups-boot.crt"
#define BASICSTATION_CUPS_BOOT_KEY      "cups-boot.key"
#define BASICSTATION_CUPS_URI           "cups.uri"
#define BASICSTATION_CUPS_TRUST         "cups.trust"
#define BASICSTATION_CUPS_CERT          "cups.crt"
#define BASICSTATION_CUPS_KEY           "cups.key"
#define BASICSTATION_CUPS_BAK_URI       "cups-bak.uri"
#define BASICSTATION_CUPS_BAK_TRUST     "cups-bak.trust"
#define BASICSTATION_CUPS_BAK_CERT      "cups-bak.crt"
#define BASICSTATION_CUPS_BAK_KEY       "cups-bak.key"
#define BASICSTATION_CUPS_BAK_DONE      "cups-bak.done"
#define BASICSTATION_TC_BOOT_URI        "tc-boot.uri"
#define BASICSTATION_TC_BOOT_TRUST      "tc-boot.trust"
#define BASICSTATION_TC_BOOT_CERT       "tc-boot.crt"
#define BASICSTATION_TC_BOOT_KEY        "tc-boot.key"
#define BASICSTATION_TC_URI             "tc.uri"
#define BASICSTATION_TC_TRUST           "tc.trust"
#define BASICSTATION_TC_CERT            "tc.crt"
#define BASICSTATION_TC_KEY             "tc.key"
#define BASICSTATION_TC_BAK_URI         "tc-bak.uri"
#define BASICSTATION_TC_BAK_TRUST       "tc-bak.trust"
#define BASICSTATION_TC_BAK_CERT        "tc-bak.crt"
#define BASICSTATION_TC_BAK_KEY         "tc-bak.key"
#define BASICSTATION_TC_BAK_DONE        "tc-bak.done"

enum { STAVERSION, STACONF,  \
    CUPS_BOOT_URI, CUPS_BOOT_TRUST, CUPS_BOOT_CERT, CUPS_BOOT_KEY,  \
    CUPS_URI ,CUPS_TRUST, CUPS_CERT, CUPS_KEY,  \
    CUPS_BAK_URI ,CUPS_BAK_TRUST, CUPS_BAK_CERT, CUPS_BAK_KEY,  \
    TC_BOOT_URI, TC_BOOT_TRUST, TC_BOOT_CERT, TC_BOOT_KEY,  \
    TC_URI, TC_TRUST, TC_CERT, TC_KEY, \
    TC_BAK_URI, TC_BAK_TRUST, TC_BAK_CERT, TC_BAK_KEY,  \
    CUPS_BAK_DONE, TC_BAK_DONE, STAFILE_NUM };

static char *bsfilename[] = { BASICSTATION_FILE_VERSION, BASICSTATION_FILE_CONF,    \
                        BASICSTATION_CUPS_BOOT_URI, BASICSTATION_CUPS_BOOT_TRUST, BASICSTATION_CUPS_BOOT_CERT, BASICSTATION_CUPS_BOOT_KEY,  \
                        BASICSTATION_CUPS_URI, BASICSTATION_CUPS_TRUST, BASICSTATION_CUPS_CERT, BASICSTATION_CUPS_KEY,  \
                        BASICSTATION_CUPS_BAK_URI, BASICSTATION_CUPS_BAK_TRUST, BASICSTATION_CUPS_BAK_CERT, BASICSTATION_CUPS_BAK_KEY,  \
                        BASICSTATION_TC_BOOT_URI, BASICSTATION_TC_BOOT_TRUST, BASICSTATION_TC_BOOT_CERT, BASICSTATION_TC_BOOT_KEY,  \
                        BASICSTATION_TC_URI, BASICSTATION_TC_TRUST, BASICSTATION_TC_CERT, BASICSTATION_TC_KEY,  \
                        BASICSTATION_TC_BAK_URI, BASICSTATION_TC_BAK_TRUST, BASICSTATION_TC_BAK_CERT, BASICSTATION_TC_BAK_KEY,  \
                        BASICSTATION_CUPS_BAK_DONE, BASICSTATION_TC_BAK_DONE };

#define BASICSTATION_FILE_VERSION_DATA  "1.0.0"

// s2e default files
#define BASICSTATION_S2E_DEFAULT_CUPS_BOOT_URI_DATA      \
    "https://s2.sm.tc:7007"
#define BASICSTATION_S2E_DEFAULT_CUPS_BOOT_TRUST_DATA    \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIBlzCCATwCAQwwCgYIKoZIzj0EAwIwVzEQMA4GA1UEAwwHUm9vdCBDQTEgMB4G\n" \
    "A1UECwwXVHJhY2tDZW50cmFsIChlaWFQb1VjOSkxFDASBgNVBAoMC1RyYWNrTmV0\n" \
    "LmlvMQswCQYDVQQGEwJDSDAeFw0xODExMDgxMjQyMDBaFw0yNDExMDYxMjQyMDBa\n" \
    "MFcxEDAOBgNVBAMMB1Jvb3QgQ0ExIDAeBgNVBAsMF1RyYWNrQ2VudHJhbCAoZWlh\n" \
    "UG9VYzkpMRQwEgYDVQQKDAtUcmFja05ldC5pbzELMAkGA1UEBhMCQ0gwWTATBgcq\n" \
    "hkjOPQIBBggqhkjOPQMBBwNCAAQpHfNq86xR2+pZY0dpurnn7IrkH9YaBoEGQ3K+\n" \
    "ZXDYaR2PviPw8sbClqQG/pYEYsLpWNk9UeJtFv5oJtFXmgC9MAoGCCqGSM49BAMC\n" \
    "A0kAMEYCIQDQBlCSN+Bo+U2xnaPBE9RWUVjY9/95I64rilCRofn0kwIhAJZ3zlGc\n" \
    "yO4Wie3jJKKNCxOioaJZUdaJZUo14vkH9OFp\n" \
    "-----END CERTIFICATE-----\n"
#define BASICSTATION_S2E_DEFAULT_CUPS_BOOT_CERT_DATA    \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIBkzCCATmgAwIBAgIBQTAKBggqhkjOPQQDAjBXMRAwDgYDVQQDDAdSb290IENB\n" \
    "MSAwHgYDVQQLDBdUcmFja0NlbnRyYWwgKGVpYVBvVWM5KTEUMBIGA1UECgwLVHJh\n" \
    "Y2tOZXQuaW8xCzAJBgNVBAYTAkNIMB4XDTE5MDMwNzE1MjgwNVoXDTIxMDMwNjE1\n" \
    "MjgwNVowTzETMBEGA1UEAwwKZ3dwcm92LTo6MDEVMBMGA1UECwwMVHJhY2tDZW50\n" \
    "cmFsMRQwEgYDVQQKDAtUcmFja05ldC5pbzELMAkGA1UEBhMCQ0gwWTATBgcqhkjO\n" \
    "PQIBBggqhkjOPQMBBwNCAASFyfmqBPkza9kd2IkrbU3zSRIriq5vOcgwfilctZVs\n" \
    "j8Z7XVBMdNLjriSjchVgyBTJfvMVMbvYTdzD7w5GBSNwMAoGCCqGSM49BAMCA0gA\n" \
    "MEUCIQCFGgSlP6GYM6uXd6KXuEwc098p5orHP/dxupGAZifquwIgbUh9UAR8tgAi\n" \
    "8TH36s9z+mz27ea7ezcmH8aM4W3Meng=\n" \
    "-----END CERTIFICATE-----\n"
#define BASICSTATION_S2E_DEFAULT_CUPS_BOOT_KEY_DATA    \
    "-----BEGIN EC PRIVATE KEY-----\n" \
    "MHcCAQEEIJbt0pUj24UIWS6HngUqJtP3uPpweSIqhB4TRoyHdzdqoAoGCCqGSM49\n" \
    "AwEHoUQDQgAEhcn5qgT5M2vZHdiJK21N80kSK4qubznIMH4pXLWVbI/Ge11QTHTS\n" \
    "464ko3IVYMgUyX7zFTG72E3cw+8ORgUjcA==\n" \
    "-----END EC PRIVATE KEY-----\n"
// ttn default files
#define BASICSTATION_TTN_DEFAULT_FILE_URI_DATA      \
    "wss://browan.eu1.cloud.thethings.industries:8887"

#define BASICSTATION_TTN_DEFAULT_FILE_TRUST_DATA    \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/\n" \
    "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
    "DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow\n" \
    "SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT\n" \
    "GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC\n" \
    "AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF\n" \
    "q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8\n" \
    "SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0\n" \
    "Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA\n" \
    "a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj\n" \
    "/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T\n" \
    "AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG\n" \
    "CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv\n" \
    "bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k\n" \
    "c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw\n" \
    "VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC\n" \
    "ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz\n" \
    "MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu\n" \
    "Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF\n" \
    "AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo\n" \
    "uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/\n" \
    "wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu\n" \
    "X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG\n" \
    "PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6\n" \
    "KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==\n" \
    "-----END CERTIFICATE-----\n"

#define BASICSTATION_FILE_CONF_DATA     \
    "{\n" \
    "\t\"SX1301_conf\": {\n" \
    "\t\t\"lorawan_public\": true,\n" \
    "\t\t\"clksrc\": 1,\n" \
    "\t\"device\": \"/dev/spidev1.0\",\n" \
    "\t\"radio_0\": {\n" \
    "\t\t\"type\": \"SX1257\",\n" \
    "\t\t\"rssi_offset\": -166.0,\n" \
    "\t\t\"tx_enable\": true,\n" \
    "\t\t\"antenna_gain\": 0\n" \
    "\t},\n" \
    "\t\"radio_1\": {\n" \
    "\t\t\"type\": \"SX1257\",\n" \
    "\t\t\"rssi_offset\": -166.0,\n" \
    "\t\t\"tx_enable\": false\n" \
    "\t}\n" \
    "\t},\n" \
    "\t\"station_conf\": {\n" \
    "\t\t\"log_level\": \"DEBUG\",\n" \
    "\t\t\"log_size\": 10000000,\n" \
    "\t\t\"log_rotate\":  3,\n" \
    "\t\t\"CUPS_RESYNC_INTV\": \"1s\"\n" \
    "\t}\n" \
    "}\n"
#endif

struct logfile {
    str_t path;
    int   size;
    int   rotate;
};

extern str_t  sys_slaveExec;  // template to start slave processes

void     sys_startLogThread ();
void     sys_iniLogging (struct logfile* lf, int captureStdio);
void     sys_flushLog ();
int      sys_findPids (str_t device, u4_t* pids, int n_pids);
dbuf_t   sys_checkFile (str_t filename);
void     sys_writeFile (str_t filename, dbuf_t* data);
void     sys_startupSlave (int rdfd, int wrfd);
int      sys_enableGPS (str_t device);
void     sys_enableCmdFIFO (str_t file);
int      station_main (void);
#endif // _sys_linux_h_
