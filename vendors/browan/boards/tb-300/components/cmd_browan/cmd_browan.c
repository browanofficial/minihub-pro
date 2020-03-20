
/* Console example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

#include "esp_log.h"
#include "esp_console.h"
#include "esp_err.h"

#include "argtable3/argtable3.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "event_groups.h"
#include "semphr.h"

#include "cmd_browan.h"
#include "cmd_browan_gpio.h"
#include "cmd_browan_nvs.h"

/* lora and basicstation */
#include "test_loragw.h"
#include "util_tx_continuous.h"
#include "util_tx_test.h"
#include "util_pkt_logger.h"
#include "lora_config.h"
#include "rt.h"
#include "fs.h"
#include "sys_linux.h"
#if defined(CFG_selftests)
#include "selftests.h"
#endif

#include "wifi_manager.h"
#include "aws_iot.h"

#include "aws_dev_mode_key_provisioning.h"

static const char TAG[] = "cmd_browan";

static struct main_arg arg;
static bool util_lora_is_running = false;
xSemaphoreHandle lora_alive;

static int browan_test(int argc, char **argv)
{
    printf("argc: %d, argv: ", argc);
    for (uint8_t i = 0; i < argc; i++) {
        printf("\"%s\" ", argv[i]);
    }
    printf("\n");
    return 0;
}

static void register_browan_test(void)
{
	const esp_console_cmd_t cmd = {
		.command = "test",
		.help = "Test command.",
		.hint = NULL,
		.func = &browan_test,
	};
	ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

int browan_util_lora_status(void)
{
    if (util_lora_is_running)
    {
        printf("ERROR: LoRa already started, try 'util_stop_lora'\n");
        return 1;
    }
    return 0;
}

int browan_util_cmd_status(char *cmd, int length)
{
    int target_length = 0;
    int cmd_flag = 1;

    if ( length < 2)
    {
        printf("ERROR: Invalid command : NULL ( should be 'txcn', 'txpkt', 'rxpkt' or 'clean')\n");
        return 1;
    }

    if (!strncasecmp(cmd, LORA_CFG_CLEAN, strlen(LORA_CFG_CLEAN)))
    {
        target_length = LORA_CFG_CLEAN_LEN;
    }
    else if (!strncasecmp(cmd, LORA_TXCN, strlen(LORA_TXCN)))
    {
        target_length = LORA_TXCN_CMD_LEN;
    }
    else if (!strncasecmp(cmd, LORA_TXPKT, strlen(LORA_TXPKT)))
    {
        target_length = LORA_TXPKT_CMD_LEN;
    }
    else if (!strncasecmp(cmd, LORA_RXPKT, strlen(LORA_RXPKT)))
    {
        target_length = LORA_RXPKT_CMD_LEN;
    }
    else
    {
        printf("ERROR: Invalid command : %s ( should be 'txcn', 'txpkt', 'rxpkt' or 'clean')\n", cmd);
        cmd_flag = 0;
        return 1;
    }

    if ( cmd_flag && length !=  target_length)
    {
        printf("ERROR: Invalid command length : %d (should be %d)\n", length, target_length);
        lora_config_help(cmd);
        return 1;
    }

    return 0;
}

static struct {
    struct arg_int *led_action;
    struct arg_end *end;
} led_args;

static int browan_cmd_led(int argc, char **argv)
{
    int ctrl = -1;
    if(argc > 1)
        ctrl = strtol(argv[1], NULL, 10);

    switch (ctrl) {
    case 0:
        // printf("set led pin to output high\n");
        printf("Turn on red LED\n");
        cmd_browan_gpio_led_turn_on_red();
        break;

    case 1:
        // printf("set led pin to output low\n");
        printf("Turn on green LED\n");
        cmd_browan_gpio_led_turn_on_green();
        break;

    default:
        // printf("set led pin to input type\n");
        printf("Turn off LED\n");
        cmd_browan_gpio_led_turn_off();
        break;
    }
    return 0;
}

static void register_browan_cmd_led(void)
{
    led_args.led_action = arg_int0(NULL, NULL, "<0|1|2>", "0: turn on green LED, 1: turn on green LED, others: turn off LED");
    led_args.end = arg_end(1);
    const esp_console_cmd_t cmd = {
        .command = "led",
        .help = "Turn on/off LED.\n",
        .hint = NULL,
        .func = &browan_cmd_led,
        .argtable = &led_args,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

static int browan_cmd_time(int argc, char **argv)
{
	time_t now;
	struct tm timeinfo;
	char strftime_buf[64];

	time(&now);
	localtime_r(&now, &timeinfo);
	strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
	printf("%s\n", strftime_buf);
	return 0;
}

static void register_browan_cmd_time(void)
{
	const esp_console_cmd_t cmd = {
		.command = "time",
		.help = "Get system time.\n",
		.hint = NULL,
		.func = &browan_cmd_time,
		.argtable = NULL,
	};
	ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

static int browan_cmd_lora_config(int argc, char **argv)
{
    if (!browan_util_cmd_status(argv[1], argc))
    {
        lora_config_set(argc, argv);
    }
    return 0;
}

static void register_browan_cmd_lora_config(void)
{
    const esp_console_cmd_t cmd =
    {
        .command = "lora_config",
        .help = "lora_configs -h\n",
        .hint = NULL,
        .func = &browan_cmd_lora_config
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static int browan_cmd_util_tx_continuous(int argc, char **argv)
{
    if (!browan_util_lora_status())
    {
        xTaskCreate(util_tx_continuous, "util_tx_continuous_task", 4096, NULL, tskIDLE_PRIORITY + 2, NULL);
        util_lora_is_running = true;
    }
    return 0;
}

static void register_browan_cmd_util_tx_continuous(void)
{
    const esp_console_cmd_t cmd =
    {
        .command = "util_tx_continuous",
        .help = "util_tx_continuous -h\n",
        .hint = NULL,
        .func = &browan_cmd_util_tx_continuous,
        .argtable = NULL,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}
static int browan_cmd_util_tx_test(int argc, char **argv)
{
    if (!browan_util_lora_status())
    {
        xTaskCreate(util_tx_test, "util_tx_test_task", 4096, NULL, tskIDLE_PRIORITY + 2, NULL);
        util_lora_is_running = true;
    }
    return 0;
}

static void register_browan_cmd_util_tx_test(void)
{
    const esp_console_cmd_t cmd =
    {
        .command = "util_tx_test",
        .help = "util_tx_test -h\n",
        .hint = NULL,
        .func = &browan_cmd_util_tx_test,
        .argtable = NULL,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}
static int browan_cmd_util_pkt_logger(int argc, char **argv)
{
    if (!browan_util_lora_status())
    {
         xTaskCreate(util_pkt_logger, "util_pkt_logger_task", 10240, NULL, tskIDLE_PRIORITY + 2, NULL);

        util_lora_is_running = true;
    }
    return 0;
}

static void register_browan_cmd_util_pkt_logger(void)
{
    const esp_console_cmd_t cmd =
    {
        .command = "util_pkt_logger",
        .help = "util_pkt_logger -h\n",
        .hint = NULL,
        .func = &browan_cmd_util_pkt_logger,
        .argtable = NULL,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static int browan_cmd_util_stop_lora(int argc, char **argv)
{
    if(util_lora_is_running)
    {
        xSemaphoreGive(lora_alive);
        util_lora_is_running = false;
        lora_config_free();
    }
    else
    {
        printf("WARNING: LoRa had been stopped\n");
    }
    return 0;
}

static void register_browan_cmd_util_stop_lora(void)
{
    const esp_console_cmd_t cmd =
    {
        .command = "util_stop_lora",
        .help = "Stop lora test.\n",
        .hint = NULL,
        .func = &browan_cmd_util_stop_lora,
        .argtable = NULL,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static int browan_cmd_clean_wifi(int argc, char **argv)
{
    wifi_manager_clean_wifi_sta_config();
    return 0;
}

static void register_browan_cmd_clean_wifi(void)
{
    const esp_console_cmd_t cmd =
    {
        .command = "clean_wifi",
        .help = "Clean WiFi data.\n",
        .hint = NULL,
        .func = &browan_cmd_clean_wifi,
        .argtable = NULL,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static void fs_delete_file(char *path)
{
    int fd;
    fd = fs_access(path, 0);
    if(fd >= 0) {
        LOG(MOD_SYS|INFO, "File %s is exist. Remove it.", path);
        fs_unlink(path);
    }
}

static void restore_file(char *path, char *data)
{
    int fd;
    fd = fs_access(path, 0);
    if(fd >= 0)
    {
        ESP_LOGI(TAG, "File %s is exist. Remove and create default file.", path);
        fs_unlink(path);
    }
    else
    {
        ESP_LOGI(TAG, "File %s isn't exist. Create default file.", path);
    }
    fd = fs_open(path, O_CREAT|O_TRUNC|O_WRONLY, 0777);
    if(fd >= 0)
    {
        fs_write(fd, data, strlen(data)+1);
        fs_close(fd);
    }
    else
    {
        ESP_LOGE(TAG, "Create default file %s fail. (%d)\n", path, fd);
    }
    return;
}

int browan_cmd_bs_restore(int argc, char **argv)
{
    char path[128];
    int i = 0;

    for( i = 0; i < STAFILE_NUM; i++) {
        fs_delete_file(bsfilename[i]);
    }
    snprintf(path, 127, "%s%s", BASICSTATION_TOP_PATH, BASICSTATION_FILE_VERSION);
    restore_file(path, BASICSTATION_FILE_VERSION_DATA);
    snprintf(path, 127, "%s%s", BASICSTATION_TOP_PATH, BASICSTATION_FILE_CONF);
    restore_file(path, BASICSTATION_FILE_CONF_DATA);

    if( argc == 2 && !strcmp(argv[1], "ttn") ) {
        snprintf(path, 127, "%s%s", BASICSTATION_TOP_PATH, BASICSTATION_TC_URI);
        restore_file(path, BASICSTATION_TTN_DEFAULT_FILE_URI_DATA);
        snprintf(path, 127, "%s%s", BASICSTATION_TOP_PATH, BASICSTATION_TC_TRUST);
        restore_file(path, BASICSTATION_TTN_DEFAULT_FILE_TRUST_DATA);
    }
    else
    {
        snprintf(path, 127, "%s%s", BASICSTATION_TOP_PATH, BASICSTATION_CUPS_BOOT_URI);
        restore_file(path, BASICSTATION_S2E_DEFAULT_CUPS_BOOT_URI_DATA);
        snprintf(path, 127, "%s%s", BASICSTATION_TOP_PATH, BASICSTATION_CUPS_BOOT_TRUST);
        restore_file(path, BASICSTATION_S2E_DEFAULT_CUPS_BOOT_TRUST_DATA);
        snprintf(path, 127, "%s%s", BASICSTATION_TOP_PATH, BASICSTATION_CUPS_BOOT_CERT);
        restore_file(path, BASICSTATION_S2E_DEFAULT_CUPS_BOOT_CERT_DATA);
        snprintf(path, 127, "%s%s", BASICSTATION_TOP_PATH, BASICSTATION_CUPS_BOOT_KEY);
        restore_file(path, BASICSTATION_S2E_DEFAULT_CUPS_BOOT_KEY_DATA);
    }
    fs_gc(0);
    return 0;
}

int browan_cmd_restore_default(int argc, char **argv)
{
    //do factory reset
    printf("Restore to default...\n");
    browan_cmd_clean_wifi(0, 0);
    cmd_browan_nvs_set_opmode(CMD_BROWAN_NVS_OPMODE_config_mode);

    //do basic station file reset
    browan_cmd_bs_restore(argc, argv);
    return 0;
}

static void register_browan_cmd_restore_default(void)
{
    const esp_console_cmd_t cmd =
    {
        .command = "restore_default",
        .help = "Restore to default (factory reset).\n",
        .hint = NULL,
        .func = &browan_cmd_restore_default,
        .argtable = NULL,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

#if defined(CFG_selftests)

static void browan_selftest_task(void *pvParameter)
{
	selftest_fs();
	vTaskDelete(NULL);
}

static int browan_cmd_selftest(int argc, char **argv)
{
	xTaskCreate(browan_selftest_task, "browan_selftest_task", 1024*10, NULL, tskIDLE_PRIORITY + 2, NULL);
	return 0;
}

static void register_browan_cmd_selftest(void)
{
    const esp_console_cmd_t cmd =
    {
        .command = "selftest",
        .help = "selftest for basicstation fs.\n",
        .hint = NULL,
        .func = &browan_cmd_selftest,
        .argtable = NULL,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}
#endif //CFG_selftests

static int browan_cmd_fscmd(int argc, char **argv)
{
    char cmd[128] = "";
    if(argc == 1)
        sprintf(cmd, "help");
    else
        for(int i=1; i<argc; i++)
            snprintf(cmd, 127, "%s%s ", cmd, argv[i]);
    fs_shell(cmd);
    return 0;
}

static void register_browan_cmd_fscmd(void)
{
    const esp_console_cmd_t cmd =
    {
        .command = "fscmd",
        .help = "fscmd help.\n",
        .hint = NULL,
        .func = &browan_cmd_fscmd,
        .argtable = NULL,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static int browan_cmd_aws_cert_restore(int argc, char **argv)
{
    char path[128];
    snprintf(path, 127, "%s/%s", AWS_IOT_TOP_DIR, AWS_IOT_CERTIFICATE_PEM_FILE);
    restore_file(path, AWS_IOT_CERTIFICATE_PEM_FILE_DATA);

    snprintf(path, 127, "%s/%s", AWS_IOT_TOP_DIR, AWS_IOT_PRIVATE_KEY_PEM_FILE);
    restore_file(path, AWS_IOT_PRIVATE_KEY_PEM_FILE_DATA);

    fs_gc(0);
    return 0;
}

static void register_browan_cmd_aws_cert_restore(void)
{
    const esp_console_cmd_t cmd =
    {
        .command = "aws_cert_restore",
        .help = "Restore AWS IoT credentials to default.\n",
        .hint = NULL,
        .func = &browan_cmd_aws_cert_restore,
        .argtable = NULL,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static int browan_cmd_aws_key_provision(int argc, char **argv)
{
	vDevModeKeyProvisioning();
    return 0;
}

static void register_browan_cmd_aws_key_provision(void)
{
    const esp_console_cmd_t cmd =
    {
        .command = "aws_key_provision",
        .help = "AWS Key Provision.\n",
        .hint = NULL,
        .func = &browan_cmd_aws_key_provision,
        .argtable = NULL,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static int browan_cmd_wifi_info(int argc, char **argv)
{
    wifi_manager_show_info();
    return 0;
}

static void register_browan_cmd_wifi_info(void)
{
    const esp_console_cmd_t cmd =
    {
        .command = "wifi_info",
        .help = "Show WiFi information.\n",
        .hint = NULL,
        .func = &browan_cmd_wifi_info,
        .argtable = NULL,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static struct {
    struct arg_str *ssid;
    struct arg_str *pwd;
    struct arg_end *end;
} wifi_conf_args;

static int browan_cmd_wifi_conf(int argc, char **argv)
{
    if(argc > 2)
        return wifi_manager_set_info(argv[1], argv[2]);
    else if(argc == 2)
        return wifi_manager_set_info(argv[1], NULL);
    else
        return -1;
}

static void register_browan_cmd_wifi_conf(void)
{
    wifi_conf_args.ssid = arg_str1(NULL, NULL, "<SSID>", "WiFi Station SSID");
    wifi_conf_args.pwd = arg_str1(NULL, NULL, "<Password>", "WiFi Station Password");
    wifi_conf_args.end = arg_end(2);

    const esp_console_cmd_t cmd =
    {
        .command = "wifi_conf",
        .help = "Configure WiFi station information.\n",
        .hint = NULL,
        .func = &browan_cmd_wifi_conf,
        .argtable = &wifi_conf_args
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

static void browan_cmd_station_task(void *pvParameter)
{
    station_main();
    vTaskDelete(NULL);
}

int browan_cmd_station(int argc, char **argv)
{
    //xTaskCreate(browan_cmd_station_task, "browan_cmd_station_task", 1024000, NULL, tskIDLE_PRIORITY + 3, NULL);
    station_main();
    return 0;
}

static void register_browan_cmd_station(void)
{
    const esp_console_cmd_t cmd =
    {
        .command = "station",
        .help = "station -h\n",
        .hint = NULL,
        .func = &browan_cmd_station,
        .argtable = NULL,
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

int browan_cmd_opmode_conf(int argc, char **argv)
{
    if(argc == 2)
    {
        if(strcmp(argv[1], "0") != 0 && strcmp(argv[1], "1") != 0)
        {
            printf("ERROR: invalid mode.\n");
            return -1;
        }
        uint8_t mode = strtol(argv[1], NULL, 10);
        if(mode == 0)
        {
            cmd_browan_nvs_set_opmode(CMD_BROWAN_NVS_OPMODE_config_mode);
        }
        else //mode == 1
        {
            cmd_browan_nvs_set_opmode(CMD_BROWAN_NVS_OPMODE_wifi_sta_mode);
        }
        return 0;
    }
    else
    {
        printf("ERROR: invalid mode.\n");
        return -1;
    }
}

static void register_browan_cmd_opmode_conf(void)
{
    static struct {
        struct arg_str *opmode;
        struct arg_end *end;
    } opmode_conf_args;

    opmode_conf_args.opmode = arg_str1(NULL, NULL, "<opmode:0|1>",
        "0: Configuration Mode. 1: WiFi Station Mode.");
    opmode_conf_args.end = arg_end(2);

    const esp_console_cmd_t cmd =
    {
        .command = "opmode_conf",
        .help = "Configure operation mode.\n",
        .hint = NULL,
        .func = &browan_cmd_opmode_conf,
        .argtable = &opmode_conf_args
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

int browan_cmd_opmode_get(int argc, char **argv)
{
    uint8_t mode;
    cmd_browan_nvs_get_opmode(&mode);
    printf("* Operation mode: %s mode (%d)\n",
        (mode - CMD_BROWAN_NVS_OPMODE_config_mode == 1) ? "wifi station":"configuration",
        mode - CMD_BROWAN_NVS_OPMODE_config_mode);
    return 0;
}

static void register_browan_cmd_opmode_get(void)
{
    const esp_console_cmd_t cmd =
    {
        .command = "opmode_get",
        .help = "Show current operation mode.\n",
        .hint = NULL,
        .func = &browan_cmd_opmode_get,
        .argtable = NULL
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

void register_browan(void)
{
    //init
    arg.argc = 0;
    cmd_browan_gpio_init();

    fs_ini(NULL);
    fs_chdir(BASICSTATION_TOP_PATH);

    //show some infomation
    browan_cmd_opmode_get(0, 0);

    //register command
    register_browan_test();
    register_browan_cmd_led();
    register_browan_cmd_time();
    register_browan_cmd_lora_config();
    register_browan_cmd_util_tx_continuous();
    register_browan_cmd_util_tx_test();
    register_browan_cmd_util_pkt_logger();
    register_browan_cmd_util_stop_lora();
    register_browan_cmd_clean_wifi();
    register_browan_cmd_restore_default();
#if defined(CFG_selftests)
    register_browan_cmd_selftest();
#endif
    register_browan_cmd_fscmd();
    register_browan_cmd_wifi_info();
    register_browan_cmd_wifi_conf();
    register_browan_cmd_opmode_conf();
    register_browan_cmd_opmode_get();
    register_browan_cmd_aws_cert_restore();
    register_browan_cmd_aws_key_provision();
    register_browan_cmd_station();
}
