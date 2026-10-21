## ⚠️*Disclaimer*

Este proyecto ha sido desarrollado exclusivamente con fines educativos, de investigación y para evaluaciones de seguridad en entornos controlados bajo autorización previa.
El uso de esta herramienta contra infraestructuras o terminales sin consentimiento explícito es responsabilidad exclusiva del usuario final.

---

# BlueDucky
Hardware de pentesting para pruebas de penetración a la vulnerabilidad CVE-2023-45866 basada en la ESP32 con almacenamiento MicroSD, suplantación dinámica de SSID y panel de control web.

--- 
## Descripción del proyecto
BlueDucky es un dispositivo diseñado para auditorías de seguridad y pruebas de inyección HID no autenticada vía Bluetooth.

El sistema opera de la siguiente forma:
1. **Escanea** redes en 2.4GHz y clasifica los proveedores más comunes mediante una lista enlazada dinámica en C.
2. **Determina** el proveedor predominante y levanta un punto de acceso bajo ese SSID.
3. **Despliega** una interfaz gráfica retro alojada en memoria de programa, permitiendo armar scripts visualmente, escribir DuckyScript o cargarlos desde una tarjeta MicroSD.
4. **Emula** teclado y ratón inalámbricos para inyección de comandos en terminales vulnerables.

---
## Hardware
| Componente | Especificación / Modelo | Propósito |
| :--- | :--- | :--- |
| **Microcontrolador** | ESP32 DevKit V1 (30 pines) | Escaneo Wi-Fi, servidor HTTP, pila Bluetooth BLE |
| **Regulador de Tensión** | L7805CV (TO-220, lineal) | Reducción de 9V (batería) a 5V regulados hacia el pin `VIN` |
| **Almacenamiento** | Módulo MicroSD | Persistencia de payloads DuckyScript y registros de auditoría |
| **Alimentación** | Batería alcalina de 9V (con switch ON/OFF) | Autonomía portátil |
| **Gabinete** | Impresión 3D MakerBot PLA | Alojamiento modular para protoboard y componentes |

---

## Arquitectura de Software

El firmware está estructurado modularmente para el entorno Arduino IDE:

```text
BlueDucky/
├── ESP32_Pentesting.ino          # configuración principal, bucle del servidor y AP
├── ESP32_Utils.hpp               # rutinas de escaneo Wi-Fi y funciones de red
├── lista_proveedores.hpp         # implementación de la lista enlazada en C puro
└── index.h                       # interfaz web retro (HTML/CSS/JS) almacenada en PROGMEM
```

--- 

## Guía de uso
1. **Encendido:** Presionar el interruptor lateral o conectar mediante cable USB-C.
2. **Reconocimiento:** El ESP32 identificará la red dominante.
3. **Conexión al Panel:**
   * Conéctate al SSID generado (`B1U3_<PROVEEDOR>` o `Totalplay_B1U3_2.4`).
   * Abre un navegador web e ingresa a `http://192.168.4.1`.
4. **Carga y Despliegue:**
   * Selecciona un payload desde la pestaña **LIBRARY**, o arma la secuencia en **BUILDER**.
   * Guarda las modificaciones en la MicroSD con **SAVE SD**.
   * Presiona **LANZAR** para iniciar la inyección sobre el dispositivo enlazado.

---

Proyecto desarrollado para la Experiencia Educativa de **Pentesting**:

* **Brayan Usciel Seseña Hidalgo**
* **Martha Denisse Lara Xocuis**
* **Alan Martínez Schleske**

*Facultad de Estadística e Informática (FEI)*
