#ifndef __CMD_BROWAN_NVS_H__
#define __CMD_BROWAN_NVS_H__

#include "stdint.h"

enum
{
    CMD_BROWAN_NVS_OK = 0,
    CMD_BROWAN_NVS_FAIL,

    CMD_BROWAN_NVS_OPMODE_config_mode,
    CMD_BROWAN_NVS_OPMODE_wifi_sta_mode,
    CMD_BROWAN_NVS_OPMODE_unknown,
};

uint8_t cmd_browan_nvs_init(void);
uint8_t cmd_browan_nvs_set_opmode(uint8_t opmode);
uint8_t cmd_browan_nvs_get_opmode(uint8_t *opmode);

#endif //__CMD_BROWAN_NVS_H__
