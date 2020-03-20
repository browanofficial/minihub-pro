#ifndef __LORA_CONFIG_H__
#define __LORA_CONFIG_H__

#include <stdint.h>
#include <stdbool.h>
#include "loragw_hal.h"

/* LORA UTIL TEST */

#define LORA_TXCN "txcn"
#define LORA_TXPKT "txpkt"
#define LORA_TXPKT_LBT "txpktlbt"
#define LORA_RXPKT "rxpkt"
#define LORA_CFG_CLEAN "clean"

typedef enum
{
    LORA_CFG_CLEAN_LEN = 2,
    LORA_TXCN_CMD_LEN = 14,
    LORA_TXPKT_CMD_LEN = 16,
    LORA_RXPKT_CMD_LEN = 4
}lora_config_cmd_length;

/* txcn */
typedef struct
{
    uint32_t freq_hz;
    enum lgw_radio_type_e radio_type;
    uint8_t g_dig;
    uint8_t g_dac;
    uint8_t g_mix;
    uint8_t g_pa;
    char mod[64];
    uint8_t sf;
    unsigned int bw_khz;
    float br_kbps;
    uint8_t fdev_khz;
    uint8_t bt;
    unsigned int tx_notch_freq;
}lora_config_txcn_info_st;

extern lora_config_txcn_info_st *txcn_info;

typedef enum
{
    TXCN_FREQ = 2,
    TXCN_RADIO,
    TXCN_NOTCH,
    TXCN_DIG,
    TXCN_MIX,
    TXCN_PA,
    TXCN_MOD,
    TXCN_SF,
    TXCN_BW,
    TXCN_BR,
    TXCN_FDEV,
    TXCN_BT,
}lora_config_txcn_para;

/*txpkt*/
typedef struct
{
    uint32_t freq_hz;
    enum lgw_radio_type_e radio_type;
    char mod[64];
    uint8_t sf;
    unsigned int bw_khz;
    int cr;
    float br_kbps;
    uint8_t fdev_khz;
    unsigned int tx_notch_freq;
    struct lgw_tx_gain_lut_s txgain_lut;
    int8_t power;
    uint16_t preamb;
    uint16_t pl_size;
    bool invert;
    int tm;
}lora_config_txpkt_info_st;

extern lora_config_txpkt_info_st *txpkt_info;

typedef enum
{
    TXPKT_FREQ = 2,
    TXPKT_RADIO,
    TXPKT_NOTCH,
    TXPKT_MOD,
    TXPKT_SF,
    TXPKT_BW,
    TXPKT_CR,
    TXPKT_BR,
    TXPKT_FDEV,
    TXPKT_PWR,
    TXPKT_PREAMB,
    TXPKT_PL_SIZE,
    TXPKT_IVERT,
    TXPKT_TM
}lora_config_txpkt_para;

/*rxpkt*/

typedef enum
{
    REGION_US = 0,
    REGION_EU
}lora_config_region;

typedef struct
{
    uint8_t region;
    uint8_t group_id;
}lora_config_rxpkt_info_st;

extern lora_config_rxpkt_info_st *rxpkt_info;

typedef enum
{
    RXPKT_REGION = 2,
    RXPKT_CH_GROUP

}lora_config_rxpkt_para;

void check_cmd_status(int source,int target);
void lora_config_help(char *cmd);
void lora_config_set(int argc, char **argv);
void lora_config_free(void);
void lora_config_txcn(int argc, char **argv);
void lora_config_txpkt(int argc, char **argv);
void lora_config_rxpkt(int argc, char **argv);
#endif //__LORA_CONFIG_H__
