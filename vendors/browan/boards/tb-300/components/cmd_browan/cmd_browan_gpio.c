#include "cmd_browan_gpio.h"
#include "cmd_browan_nvs.h"
#include "cmd_browan.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "driver/gpio.h"
#include "esp_log.h"

#define GPIO_LED GPIO_NUM_4
#define GPIO_SETUP_BTN GPIO_NUM_0
#define GPIO_RESET_BTN GPIO_NUM_23
#define ESP_INTR_FLAG_DEFAULT 0

static const char* TAG = "gpio";

static xQueueHandle gpio_evt_queue = NULL;
static TimerHandle_t HoldRstBtnTimer;
static TimerHandle_t HoldSetupBtnTimer;
static TimerHandle_t LedTimer;
static int HoldRstBtnTimer_Cnt = 0;
static bool HoldRstBtn_need_reset = false;
static int HoldSetupBtnTimer_Cnt = 0;
static bool HoldSetupBtn_need_reset = false;
static int LedBlink_cycle_time_ms = 1000;
static float LedBlink_duty_cycle = 0.0;

static void cmd_browan_gpio_task(void *arg);
static void vTimerCallback_HoldRstBtn(TimerHandle_t xTimer);
static void vTimerCallback_HoldSetupBtn(TimerHandle_t xTimer);
static void vTimerCallback_LedBlinking(TimerHandle_t xTimer);
static void (*LedBlinkFunc_next_func)(void);
static void (*LedBlinkFunc_on_func)(void);
static void (*LedBlinkFunc_off_func)(void);

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

void cmd_browan_gpio_init(void)
{
    gpio_config_t io_conf;

    // //disable interrupt
    // io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    // //set as output mode
    // io_conf.mode = GPIO_MODE_OUTPUT;
    // //bit mask of the pins that you want to set
    // io_conf.pin_bit_mask = (1ULL << GPIO_LED);
    // //disable pull-down mode
    // io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    // //disable pull-up mode
    // io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    // //configure GPIO with the given settings
    // gpio_config(&io_conf);

    //interrupt of any edge
    io_conf.intr_type = GPIO_PIN_INTR_ANYEDGE;
    //bit mask of the pins
    io_conf.pin_bit_mask = (1ULL << GPIO_RESET_BTN | 1ULL << GPIO_SETUP_BTN | 1ULL << GPIO_LED);
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //disable pull-down mode
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    //enable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(cmd_browan_gpio_task, "cmd_browan_gpio_task", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_RESET_BTN, gpio_isr_handler, (void *)GPIO_RESET_BTN);
    gpio_isr_handler_add(GPIO_SETUP_BTN, gpio_isr_handler, (void *)GPIO_SETUP_BTN);

    //init button timer
    HoldRstBtnTimer = xTimerCreate("HoldRstBtnTimer", 1000 / portTICK_PERIOD_MS, pdFALSE, 0, vTimerCallback_HoldRstBtn);
    HoldSetupBtnTimer = xTimerCreate("HoldSetupBtnTimer", 1000 / portTICK_PERIOD_MS, pdFALSE, 0, vTimerCallback_HoldSetupBtn);

    //init led ctrl
    LedTimer = xTimerCreate("LedTimer", 1000 / portTICK_PERIOD_MS, pdFALSE, 0, vTimerCallback_LedBlinking);
    LedBlinkFunc_next_func = &cmd_browan_gpio_led_turn_off;
    return;
}

static void cmd_browan_gpio_task(void *arg)
{
    uint32_t io_num;

    while (1)
    {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY))
        {
            int level = gpio_get_level(io_num);
            ESP_LOGI(TAG, "GPIO[%d] intr, val: %d\n", io_num, level);

            switch (io_num)
            {
                case GPIO_RESET_BTN:
                {
                    if (level == 0)
                    {
                        //press reset btn
                        HoldRstBtnTimer_Cnt = 0;
                        vTimerCallback_HoldRstBtn(HoldRstBtnTimer);
                    }
                    else
                    {
                        //release reset btn
                        xTimerStop(HoldRstBtnTimer, 0);
                        if (HoldRstBtn_need_reset)
                            esp_restart();
                    }
                    break;
                }
                case GPIO_SETUP_BTN:
                {
                    if (level == 0)
                    {
                        //press setup btn
                        browan_cmd_opmode_get(0, 0);
                        HoldSetupBtnTimer_Cnt = 0;
                        vTimerCallback_HoldSetupBtn(HoldSetupBtnTimer);
                    }
                    else
                    {
                        //release setup btn
                        xTimerStop(HoldSetupBtnTimer, 0);
                        if (HoldSetupBtn_need_reset)
                            esp_restart();
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
}

static void vTimerCallback_HoldRstBtn(TimerHandle_t xTimer)
{
    if (HoldRstBtnTimer_Cnt < 5)
    {
        printf("Detected reset button... (%d)\n", HoldRstBtnTimer_Cnt);
        HoldRstBtnTimer_Cnt++;
        xTimerReset(HoldRstBtnTimer, 0);
    }
    else
    {
        browan_cmd_restore_default(0, 0);
        //HoldRstBtn_need_reset = true;
        esp_restart(); //reboot directly
    }
    return;
}

static void vTimerCallback_HoldSetupBtn(TimerHandle_t xTimer)
{
    if (HoldSetupBtnTimer_Cnt < 5)
    {
        printf("Detected setup button... (%d)\n", HoldSetupBtnTimer_Cnt);
        HoldSetupBtnTimer_Cnt++;
        xTimerReset(HoldSetupBtnTimer, 0);
    }
    else
    {
        uint8_t opmode = CMD_BROWAN_NVS_OPMODE_unknown;
        cmd_browan_nvs_get_opmode(&opmode);
        if(opmode == CMD_BROWAN_NVS_OPMODE_config_mode)
        {
            printf("Opmode: configuration mode, trigger restart.\n");
            //HoldSetupBtn_need_reset = true;
            esp_restart();  //reboot directly
        }
        else if(opmode == CMD_BROWAN_NVS_OPMODE_wifi_sta_mode)
        {
            if(HoldSetupBtnTimer_Cnt < 10)
            {
                printf("Detected setup button... (%d)\n", HoldSetupBtnTimer_Cnt);
                HoldSetupBtnTimer_Cnt++;
                xTimerReset(HoldSetupBtnTimer, 0);
            }
            else
            {
                printf("Switch to configuration mode.\n");
                cmd_browan_nvs_set_opmode(CMD_BROWAN_NVS_OPMODE_config_mode);
                //HoldSetupBtn_need_reset = true;
                esp_restart();  //reboot directly
            }
        }
    }
    return;
}

void cmd_browan_gpio_ctrl(uint8_t ctrl)
{
    switch (ctrl)
    {
        case CMD_BROWAN_GPIO_CTRL_LED_HIGH:
            gpio_set_direction(GPIO_LED, GPIO_MODE_OUTPUT);
            gpio_set_level(GPIO_LED, 1);
            gpio_pullup_en(GPIO_LED);
            gpio_pulldown_dis(GPIO_LED);
            break;

        case CMD_BROWAN_GPIO_CTRL_LED_LOW:
            gpio_set_direction(GPIO_LED, GPIO_MODE_OUTPUT);
            gpio_set_level(GPIO_LED, 0);
            gpio_pullup_dis(GPIO_LED);
            gpio_pulldown_en(GPIO_LED);
            break;

        case CMD_BROWAN_GPIO_CTRL_LED_INPUT:
            gpio_set_direction(GPIO_LED, GPIO_MODE_INPUT);
            break;

        default:
            break;
    }
    return;
}

void cmd_browan_gpio_led_turn_off(void)
{
    return cmd_browan_gpio_ctrl(CMD_BROWAN_GPIO_CTRL_LED_INPUT);
}

void cmd_browan_gpio_led_turn_on_red(void)
{
    return cmd_browan_gpio_ctrl(CMD_BROWAN_GPIO_CTRL_LED_HIGH);
}

void cmd_browan_gpio_led_turn_on_green(void)
{
    return cmd_browan_gpio_ctrl(CMD_BROWAN_GPIO_CTRL_LED_LOW);
}

static void vTimerCallback_LedBlinking(TimerHandle_t xTimer)
{
    if(LedBlink_duty_cycle >= 1.0)
        return LedBlinkFunc_on_func();
    if(LedBlink_duty_cycle <= 0.0)
        return LedBlinkFunc_off_func();
    else
        LedBlinkFunc_next_func();

    int timeout = 0;
    if(LedBlinkFunc_next_func == LedBlinkFunc_on_func)
    {
        LedBlinkFunc_next_func = LedBlinkFunc_off_func;
        timeout = (int)((float)LedBlink_cycle_time_ms * LedBlink_duty_cycle);
    }
    else
    {
        LedBlinkFunc_next_func = LedBlinkFunc_on_func;
        timeout = (int)((float)LedBlink_cycle_time_ms * (1.0-LedBlink_duty_cycle));
    }
    xTimerChangePeriod(xTimer, timeout / portTICK_PERIOD_MS, 0);
    xTimerStart(xTimer, 0);
    return;
}

void cmd_browan_gpio_led_ctrl(int led_action, int cycle_time_ms, float duty_cycle)
{
    //printf("cmd_browan_gpio_led_ctrl: %d %d %f\n", led_action, cycle_time_ms, duty_cycle);
    switch (led_action)
    {
        case CMD_BROWAN_GPIO_LED_RED_to_NONE:
        {
            LedBlinkFunc_on_func = &cmd_browan_gpio_led_turn_on_red;
            LedBlinkFunc_off_func = &cmd_browan_gpio_led_turn_off;
            break;
        }
        case CMD_BROWAN_GPIO_LED_GREEN_to_NONE:
        {
            LedBlinkFunc_on_func = &cmd_browan_gpio_led_turn_on_green;
            LedBlinkFunc_off_func = &cmd_browan_gpio_led_turn_off;
            break;
        }
        case CMD_BROWAN_GPIO_LED_RED_to_GREEN:
        {
            LedBlinkFunc_on_func = &cmd_browan_gpio_led_turn_on_red;
            LedBlinkFunc_off_func = &cmd_browan_gpio_led_turn_on_green;
            break;
        }
        default:
            return;
    }
    LedBlinkFunc_next_func = LedBlinkFunc_on_func;

    LedBlink_cycle_time_ms = cycle_time_ms;
    LedBlink_duty_cycle = (duty_cycle>1.0) ? 1.0:duty_cycle;
    // printf("LedBlink_cycle_time_ms: %d, LedBlink_duty_cycle: %f\n",
    //     LedBlink_cycle_time_ms, LedBlink_duty_cycle);
    xTimerStop(LedTimer, 0);
    vTimerCallback_LedBlinking(LedTimer);
    return;
}
