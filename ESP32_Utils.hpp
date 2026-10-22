// https://www.luisllamas.es/como-generar-una-red-wifi-con-el-esp8266-modo-ap/
// este archivo básicamente es el que tiene las funciones de conexión

// no se utilizó IA en este código jejeje

#include "esp_bt_defs.h"
#include "lista_proveedores.hpp"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include <string.h>
#include <NimBLEDevice.h>

#include <string>
#include <vector>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

// https://zbotic.in/esp32-ble-scanner-detect-nearby-devices-signal-strength/

int scanTime = 5;
NimBLEScan* pBLEScan;

typedef struct bluetooth_capture {
  esp_bd_addr_t MAC;
  int RSSI;
  String name;
  String companyName;
} BT_CAPTURE;

// aqui se almacenan los dispositivos capturados por Bluetooth, todos diferentes
std::vector<bluetooth_capture> dispositivosDetectados;

// Pasamos el arreglo de destino como parámetro (uint8_t* o esp_bd_addr_t)
bool parseMACString(std::string strng, uint8_t* addr)
{
  // Verificamos de forma muy básica que la longitud sea correcta (mínimo 17 caracteres para XX:XX:XX:XX:XX:XX)
  if (strng.length() < 17 && strng.find(":") == 0) {
    return false; // Error: formato incorrecto
  }

  // Extraemos los bytes directamente al arreglo que pasamos por parámetro
  int extraidos = sscanf(strng.c_str(), "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
                         &addr[0], &addr[1], &addr[2], &addr[3], &addr[4], &addr[5]);

  for (int i = 0; i < 6; i++)
  {
    Serial.printf("%x\n", addr[i]);
  }
  
  // Si logró extraer los 6 bloques, devuelve true
  return (extraidos == 6);
}

// esta es una clase callback que se ejecuta automáticamente cada vez que el ESP32
// detecta un dispositivo BLE
class MyScanCallbacks: public NimBLEScanCallbacks {
  void onResult(const NimBLEAdvertisedDevice* advertisedDevice) override {
    Serial.print("[Ducky dice]: Dispositivo detectado ദ്ദി◝ ⩊ ◜) = ");
    Serial.print(advertisedDevice->getAddress().toString().c_str());
    Serial.print(" | RSSI: ");
    Serial.print(advertisedDevice->getRSSI());
    Serial.print(" dBm");

    String deviceName = "Desconocido";
    String companyName = "Desconocida";

    // mostrar nombre si lo tiene disponible
    if (advertisedDevice->haveName())
    {
      deviceName = String(advertisedDevice->getName().c_str());
      Serial.print(" | Name: ");
      Serial.print(deviceName);
    }

    // obtener compañía
    // intentar saber le sistema operativo por el Manufacturer Data
    if(advertisedDevice->haveManufacturerData())
    {
      std::string mData= advertisedDevice->getManufacturerData();

      // los dos primeros bytes del manufacturer son el company ID
      if(mData.length() >= 2)
      {
        uint16_t companyID=(uint8_t)mData[1]<<8 | (uint8_t)mData[0];

        Serial.print(" | Tipo: ");
        switch(companyID) {
          case 0x004C: companyName = "Apple"; break;
          case 0x0006: companyName = "Microsoft"; break;
          case 0x00E0: 
          case 0x055D: companyName = "Google/Android"; break;
          case 0x0075: companyName = "Samsung"; break;
          case 0x012D: companyName = "Sony"; break;
          default:     companyName = "Otro"; break;
        }
        Serial.print(companyName);

      }
    }
    Serial.println();

    // convierte la direccion MAC a un arreglo de 6 nibbles
    uint8_t macConvertida[6];
    parseMACString(advertisedDevice->getAddress().toString(), macConvertida);

    bluetooth_capture nuevoDispositivo;
    memcpy(nuevoDispositivo.MAC, macConvertida, 6);
    //nuevoDispositivo.MAC = (_desp_bd_addr_t) macConvertida;
    nuevoDispositivo.RSSI = advertisedDevice->getRSSI();
    nuevoDispositivo.name = deviceName;
    nuevoDispositivo.companyName = companyName;

    Serial.print("{");
    for (int i = 0; i < 6; i++)
    {
      Serial.printf("%2x", nuevoDispositivo.MAC[i]);
    }
    Serial.println("}");
    //Serial.printf(" %d %s %s", nuevoDispositivo.RSSI,
    //              nuevoDispositivo.name, nuevoDispositivo.companyName);

    // se inserta el dispositivo encontrado para que se muestre en el sitio web
    //dispositivosDetectados.push_back(nuevoDispositivo);
    //Serial.println("1");
    
    Serial.printf(" %d %s %s\n", 
                  nuevoDispositivo.RSSI, 
                  nuevoDispositivo.name.c_str(), 
                  nuevoDispositivo.companyName.c_str());

    // Se inserta en el vector global
    dispositivosDetectados.push_back(nuevoDispositivo);
  }
};


const char* cifrado(wifi_auth_mode_t t)
{
  switch(t)
  {
    case WIFI_AUTH_OPEN:            return "ABIERTA";
    case WIFI_AUTH_WEP:             return "WEP";
    case WIFI_AUTH_WPA_PSK:         return "WPA";
    case WIFI_AUTH_WPA2_PSK:        return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK:    return "WPA/WPA2";
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-EAP";
    case WIFI_AUTH_WPA3_PSK:        return "WPA3";
    case WIFI_AUTH_WPA2_WPA3_PSK:   return "WPA2/WPA3";
    default:                        return "DESCONOCIDO";
  }
}

// https://www.ukrabot.cl/blog/esp32-como-analizador-wifi-deteccion-de-redes-y-dispositivos
void Escanear()
{
  // proveedores más comunes jejsj
  Serial.printf("[   Te damos la bienvenida a BlueDucky ദ്ദി◝ ⩊ ◜)  ]");

  // primero escanea todas las redes cercanas
  int n = WiFi.scanNetworks(); // escaneo bloqueante

  if(n == 0)
  {
    Serial.println("[Ducky dice]: No encontré redes (° n °)");
  }
  else
  {
    // debug
    Serial.printf("%d redes encontradas!! :D \n\n", n);
    //Serial.println("  # | Ch | RSSI | Cifrado    | SSID");
    //Serial.println("----+----+------+------------+-----------------");

    for(int i=0; i<n; i++)
    {
    //  Serial.printf("%3d | %2d | %4d | %-10s | %s\n",
    //                i + 1,
    //                WiFi.channel(i),
    //                WiFi.RSSI(i),
    //                cifrado(WiFi.encryptionType(i)),
    //                WiFi.SSID(i).c_str()
    //  );

      String ssidStr = WiFi.SSID(i);
      char* palabrita = (char *)ssidStr.c_str();
      Serial.printf("%s \n", palabrita);

      if(strstr(palabrita, "TP-Link")) agregar_palabra("TP-Link");
      if(strstr(palabrita, "TP-LINK")) agregar_palabra("TP-LINK");
      if(strstr(palabrita, "Megacable")) agregar_palabra("Megacable");
      if(strstr(palabrita, "TOTALPLAY")) agregar_palabra("TOTALPLAY");
      if(strstr(palabrita, "INFINITUM")) agregar_palabra("INFINITUM");
      if(strstr(palabrita, "Mega")) agregar_palabra("Mega");

      delay(5);
    }

    WiFi.scanDelete(); // libera la memoria del resultado
  }

  // asignar ssid 
  const char* ssid_definido = obtener_predominante();

  if(strcmp(ssid_definido, "Totalplay_B1U3_2.4") == 0)
  {
    strncpy(ssid_blueducky, ssid_definido, sizeof(ssid_blueducky));
  }
  else if (strcmp(ssid_definido, "INFINITUM") == 0)
  {
    snprintf(ssid_blueducky, sizeof(ssid_blueducky), "%sB1U3_2.4", ssid_definido);
  }
  else if (strcmp(ssid_definido, "Mega") == 0)
  {
    snprintf(ssid_blueducky, sizeof(ssid_blueducky), "%s_2.4G_B1U3", ssid_definido);
  }
  else
  {
    snprintf(ssid_blueducky, sizeof(ssid_blueducky), "%s_B1U3_2.4", ssid_definido);
  }

  Serial.printf("Red elegida -> %s\n", ssid_blueducky);

}

void ConnectWiFi_AP()
{
  Serial.println("");
  WiFi.mode(WIFI_AP);

  while(!WiFi.softAP(ssid, passw))
  {
    Serial.println(".");
    delay(100);
  }

  Serial.println("");
  Serial.print("Iniciando AP:\t");
  Serial.println(ssid);
  Serial.print("Direccion IP:\t");
  Serial.println(WiFi.softAPIP());
}

void initScanBLE()
{
  // inicializar la pila BLE 
  NimBLEDevice::init("");
  // crear y configurar el objeto de escaneo
  pBLEScan = NimBLEDevice::getScan();

  // vincular la clase de callback para que procese los dispositivos encontrados
  pBLEScan->setScanCallbacks(new MyScanCallbacks()); // false para procesar duplicados
  
  // solicita más datos al dispositivos
  pBLEScan->setActiveScan(true);

  // tiempo que pasa escuchando en cada canal en milisegundos
  pBLEScan->setInterval(160); //100ms
  pBLEScan->setWindow(80);  // 50ms

  pBLEScan->setDuplicateFilter(true);
}

void ContinuousBLE()
{
  // si no está corriendo, arrancarlo en modo infinito
  if (!pBLEScan->isScanning())
  {
    Serial.println("[Ducky dice]: Iniciando escaneo continuo BLE ദ്ദി◝ ⩊ ◜)...");
    // duration = 0 significa escaneo continuo en segundo plano
    // scanCompleteCB = nullptr
    // is_continue = false
    pBLEScan->start(0, false);
  }
}

void scanBLE()
{
  // forzar la duración a 10 segundos 
  Serial.println("\n[Ducky dice]: Comenzando el escaneo BLE de 10 segundos... ദ്ദി◝ ⩊ ◜)");
  pBLEScan = NimBLEDevice::getScan();
  pBLEScan->start(10,false);
  pBLEScan->clearResults();
}