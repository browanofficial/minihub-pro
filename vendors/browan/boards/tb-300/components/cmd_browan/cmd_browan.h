
/* Console example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Register browan command functions
void register_browan(void);

// Command
int browan_cmd_restore_default(int argc, char **argv);
int browan_cmd_bs_restore(int argc, char **argv);
int browan_cmd_opmode_conf(int argc, char **argv);
int browan_cmd_opmode_get(int argc, char **argv);
int browan_cmd_station(int argc, char **argv);

#ifdef __cplusplus
}
#endif
