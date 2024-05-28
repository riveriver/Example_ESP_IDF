/*  WiFi softAP Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define EXAMPLE_ESP_WIFI_SSID      "river_esp32_test"
#define EXAMPLE_ESP_WIFI_PASS      "12345678"
#define EXAMPLE_ESP_WIFI_CHANNEL   1
#define EXAMPLE_MAX_STA_CONN       4

static const char *TAG = "wifi softAP";

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{   
    // 客户端连接事件时
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    // 客户端断开事件时
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
}

void wifi_init_softap(void)
{
    // 初始化网络接口
    ESP_ERROR_CHECK(esp_netif_init());
    // 创建默认的事件循环
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    // 创建默认的
    esp_netif_create_default_wifi_ap();
    // 初始化Wi-Fi模块
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    /* 注册一个事件处理程序实例来处理Wi-Fi事件。
    该函数注册了一个事件处理程序，以便在发生Wi-Fi事件时调用指定的处理程序。
    它使用了ESP_EVENT_ANY_ID作为事件ID，意味着该处理程序会对所有Wi-Fi事件作出响应。
    参数:
    - WIFI_EVENT: 指定要注册处理程序的事件类型，这里是Wi-Fi事件。
    - ESP_EVENT_ANY_ID: 用于指定处理程序应该对所有该类型的事件作出响应。
    - wifi_event_handler: 指向处理程序函数的指针。
    - NULL: 传递给事件处理程序的用户数据。
    - NULL: 用于指定事件循环实例。
    */
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));
    // 填充AP的配置参数
    wifi_config_t wifi_config = {
        .ap = {
            /* SSID */
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            /* SSID的长度 */
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            /* 频道 */
            .channel = EXAMPLE_ESP_WIFI_CHANNEL,
            /* 密码。 */
            .password = EXAMPLE_ESP_WIFI_PASS,
            /* 允许的最大连接数 */
            .max_connection = EXAMPLE_MAX_STA_CONN,
            /* 认证模式 */
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
            /* PMF配置，配置为不强制要求。 
            PMF（Protected Management Frames）是Wi-Fi网络安全的一项功能，
            它基于802.11w标准，旨在保护管理帧（如关联请求、认证帧等）免受中间人攻击和伪造的管理帧攻击。
            管理帧通常不被802.11i/WPA2的加密保护覆盖，因此它们容易成为网络攻击的目标。
            PMF通过加密这些管理帧来增强无线网络的安全性。*/
            .pmf_cfg = {
                    .required = false,
            },
        },
    };
    // 如果Wi-Fi密码为空，则认证模式设置为开放
    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }
    // 设置Wi-Fi模式为AP（Access Point，接入点）
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    // 设置AP的配置参数
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    // 启动Wi-Fi
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS, EXAMPLE_ESP_WIFI_CHANNEL);
}

void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    wifi_init_softap();
}