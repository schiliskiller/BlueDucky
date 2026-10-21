// https://www.luisllamas.es/como-generar-una-red-wifi-con-el-esp8266-modo-ap/
// este archivo básicamente es el que tiene las funciones de conexión

// no se utilizó IA en este código jejeje

#include "lista_proveedores.hpp"
#include <string.h>
#include <NimBLEDevice.h>

#include <FS.h>
#include <SD.h>
#include <SPI.h>

// https://zbotic.in/esp32-ble-scanner-detect-nearby-devices-signal-strength/

int scanTime = 5;
NimBLEScan* pBLEScan;

class MyScanCallbacks : public NimBLEScanCallbacks {
  void onResult(const NimBLEAdvertisedDevice* advertisedDevice) override {
    Serial.print("Device Found: ");
    Serial.print(advertisedDevice->getAddress().toString().c_str());
    Serial.print(" | RSSI: ");
    Serial.print(advertisedDevice->getRSSI());
    Serial.print(" dBm");
    
    if (advertisedDevice->haveName()) {
      Serial.print(" | Name: ");
      Serial.print(advertisedDevice->getName().c_str());
    }
    Serial.println();
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
    Serial.println("BlueDucky no encontró redes :c");
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

void initScanBLE(bool activeScan, uint16_t interval, uint16_t window)
{
  NimBLEDevice::init("");
  pBLEScan = NimBLEDevice::getScan();
  pBLEScan->setScanCallbacks(new MyScanCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(40); 
}

void scanBLE(uint32_t duration)
{
  if (millis() - ultimoEscaneo > intervaloEscaneo)
  {
    if (!pBLEScan->start(duration))
    {
      Serial.println("Error: Conflictos con escaneo Bluetooth");
    }

    NimBLEScanResults foundDevices = pBLEScan->getResults();
    Serial.print("Devices found: ");
    Serial.println(foundDevices.getCount());

    pBLEScan->clearResults(); // Free memory

    ultimoEscaneo = millis();
  }
}