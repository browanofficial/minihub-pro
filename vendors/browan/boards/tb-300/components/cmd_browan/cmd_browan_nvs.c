#include "cmd_browan_nvs.h"

#include "esp_log.h"
#include "esp_err.h"

#include "nvs_flash.h"

#define TAG             "cmd_browan_nvs"
#define NVS_NAMESPACE   "browan_nvs"

uint8_t cmd_browan_nvs_init(void)
{
    nvs_flash_init();
    return CMD_BROWAN_NVS_OK;
}

uint8_t cmd_browan_nvs_set_opmode(uint8_t opmode)
{
    nvs_handle handle;
    esp_err_t esp_err;

    esp_err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if(esp_err != ESP_OK)
    {
        ESP_LOGE(TAG, "Cannot open NVS.");
        return CMD_BROWAN_NVS_FAIL;
    }

    esp_err = nvs_set_u8(handle, "opmode", opmode);
    if(esp_err != ESP_OK)
    {
        ESP_LOGE(TAG, "Cannot open write opmode to NVS.");
        return CMD_BROWAN_NVS_FAIL;
    }
    ESP_LOGI(TAG, "Write opmode: %d to NVS.", opmode);
    return CMD_BROWAN_NVS_OK;
}

uint8_t cmd_browan_nvs_get_opmode(uint8_t *opmode)
{
    nvs_handle handle;
    esp_err_t esp_err;

    esp_err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if(esp_err != ESP_OK)
    {
        ESP_LOGE(TAG, "Cannot open NVS.");
        *opmode = CMD_BROWAN_NVS_OPMODE_unknown;
        return CMD_BROWAN_NVS_FAIL;
    }

    esp_err = nvs_get_u8(handle, "opmode", opmode);
    if(esp_err != ESP_OK)
    {
        ESP_LOGE(TAG, "Cannot open read opmode from NVS.");
        *opmode = CMD_BROWAN_NVS_OPMODE_unknown;
        return CMD_BROWAN_NVS_FAIL;
    }
    ESP_LOGI(TAG, "Read opmode: %d from NVS.", *opmode);
    return CMD_BROWAN_NVS_OK;
}
