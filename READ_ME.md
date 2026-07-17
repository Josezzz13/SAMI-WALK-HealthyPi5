# SAMI WALK – HealthyPi 5

Sistema portátil de monitoreo fisiológico orientado a la evaluación de marcha y al cálculo del Índice de Costo Fisiológico (PCI) en adultos mayores.

## Objetivo

El proyecto adapta la plataforma HealthyPi 5 para adquirir ECG, calcular frecuencia cardíaca, simular la velocidad de marcha y estimar el PCI en tiempo real.

## Hardware utilizado

- HealthyPi 5
- RP2040
- AFE MAX30001 para ECG
- Display ILI9488
- IMU BNO086 Qwiic, considerada para futura integración
- Simulador Fluke ProSim 8 para validación de frecuencia cardíaca

## Variables mostradas

- ECG
- Frecuencia cardíaca
- Velocidad de marcha simulada en m/min
- Índice de Costo Fisiológico

## Flujo de procesamiento

1. El MAX30001 adquiere la señal ECG.
2. El firmware detecta los complejos QRS.
3. Se calculan los intervalos RR.
4. Se obtiene la frecuencia cardíaca.
5. La velocidad de marcha se simula mediante un contador.
6. Se calcula el PCI:

PCI = (FC_trabajo_max - FC_reposo_max) / velocidad

7. El PCI se muestra con dos decimales en el display.

## Lógica de validación

- Primera fase del ciclo: se obtiene la FC máxima de reposo.
- Segunda fase: se obtiene la FC máxima de trabajo.
- Cuando el contador de velocidad vuelve a cero, se calcula el PCI.
- El valor calculado permanece visible durante el siguiente ciclo.

## Archivos principales

- `app/src/main.c`: integración general del firmware.
- `app/src/pci_module.c`: cálculo del PCI.
- `app/src/pci_module.h`: interfaz del módulo PCI.
- `app/src/bno086_module.c`: contador de velocidad simulada.
- `app/src/display_module.c`: actualización de la interfaz gráfica.
- `app/src/ui/screens/scr_ecg.c`: gráfica ECG.
- `app/src/ui/screens/scr_splash.c`: pantalla inicial SAMI WALK.
- `app/src/ecg_hr_algorithm.c`: procesamiento de picos y frecuencia cardíaca.
- `app/src/images/sami_walk_logo.c`: logo convertido a formato LVGL.

## Compilación

```bash
west build -p always -b healthypi5 app -- \
-DEXTRA_CONF_FILE="overlay-display-ili9488.conf;overlay-logger-sd.conf" \
-DCONFIG_HEALTHYPI_OP_MODE_DISPLAY=y \
-DEXTRA_DTC_OVERLAY_FILE=healthypi5_rp2040_display_ili9488.overlay
