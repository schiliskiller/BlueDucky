#include <Arduino.h>
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_l2cap_api.h"
#include "esp_sdp_api.h"
#include "esp_gap_bt_api.h"
#include "esp_hidd_api.h"

uint16_t hidd_conn_id = 0;

void l2cap_callback(esp_bt_l2cap_cb_event_t event, esp_bt_l2cap_cb_param_t *param) {
  switch (event) {
    case ESP_BT_L2CAP_INIT_EVT:
      Serial.println("L2CAP Inicializado");
      break;
    case ESP_BT_L2CAP_OPEN_EVT:
      if (param->open.status == ESP_BT_L2CAP_SUCCESS) {
        Serial.printf("Conectado! L2CAP Handle: %d\n", param->open.handle);
        // Aquí ya puedes enviar datos usando esp_bt_l2cap_vfs_send() o esp_bt_l2cap_write()
      } else {
        Serial.println("Fallo al abrir L2CAP");
      }
      break;
    case ESP_BT_L2CAP_DATA_IND_EVT:
      Serial.println("Datos L2CAP recibidos del objetivo");
      break;
    case ESP_BT_L2CAP_CLOSE_EVT:
      Serial.println("Conexión L2CAP cerrada");
      break;
    default:
      break;
  }
}

void initBluetoothStack()
{
    // 1. Liberar memoria de BLE si no se usa (muy útil para evitar Kernel Panics)
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    // 2. Inicializar el controlador
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_bt_controller_init(&bt_cfg);
    esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);

    // 3. Inicializar Bluedroid (Stack principal)
    esp_bluedroid_init();
    esp_bluedroid_enable();

    // 4. Registrar Callback de L2CAP e inicializarlo
    esp_bt_l2cap_register_callback(l2cap_callback);
    esp_bt_l2cap_init();
}