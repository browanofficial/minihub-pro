#ifndef __CMD_BROWAN_GPIO_H__
#define __CMD_BROWAN_GPIO_H__

#include <stdio.h>
#include <stdint.h>

enum
{
    CMD_BROWAN_GPIO_CTRL_LED_HIGH = 0,
    CMD_BROWAN_GPIO_CTRL_LED_LOW,
    CMD_BROWAN_GPIO_CTRL_LED_INPUT,
};

enum
{
    CMD_BROWAN_GPIO_LED_RED_to_NONE = 0,
    CMD_BROWAN_GPIO_LED_GREEN_to_NONE,
    CMD_BROWAN_GPIO_LED_RED_to_GREEN,
};

void cmd_browan_gpio_init(void);
void cmd_browan_gpio_ctrl(uint8_t ctrl);

void cmd_browan_gpio_led_turn_off(void);
void cmd_browan_gpio_led_turn_on_red(void);
void cmd_browan_gpio_led_turn_on_green(void);

/*
 * LED Blinking Example:
 *  turn off
 *      cmd_browan_gpio_led_ctrl(CMD_BROWAN_GPIO_LED_RED_to_NONE, 0, 0.0);
 *  turn on red led
 *      cmd_browan_gpio_led_ctrl(CMD_BROWAN_GPIO_LED_RED_to_NONE, 0, 1.0);
 *  fast blink green led (cycle time 500ms, duty cycle 50%)
 *      cmd_browan_gpio_led_ctrl(CMD_BROWAN_GPIO_LED_GREEN_to_NONE, 500, 0.5);
 *  slow blink between red and green (cycle time 2000ms, duty cycle 50%)
 *      cmd_browan_gpio_led_ctrl(CMD_BROWAN_GPIO_LED_RED_to_GREEN, 2000, 0.5);
 */
void cmd_browan_gpio_led_ctrl(int led_action, int cycle_time_ms, float duty_cycle);

#endif //__CMD_BROWAN_GPIO_H__
