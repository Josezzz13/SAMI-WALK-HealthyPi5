# Código fuente del firmware

## main.c
Inicializa el sistema y conecta los módulos de velocidad, PCI y display.

## pci_module.c
Implementa la lógica del PCI:
- FC máxima de reposo.
- FC máxima de trabajo.
- Reinicio por ciclo.
- Cálculo con dos decimales.
- Conservación del último valor calculado.

## bno086_module.c
Contiene la simulación temporal de velocidad de marcha en m/min.

## ecg_hr_algorithm.c
Procesa la señal ECG para detectar complejos QRS, calcular intervalos RR y estimar la frecuencia cardíaca.

## display_module.c
Gestiona:
- navegación entre pantallas;
- actualización de HR;
- visualización de Vel. Z;
- visualización del PCI;
- gráfica ECG;
- carga de la pantalla Splash.

## ui/screens/scr_ecg.c
Construye la pantalla de ECG y actualiza la gráfica en tiempo real.

## ui/screens/scr_splash.c
Muestra el logo SAMI WALK al iniciar el dispositivo.

## images/sami_walk_logo.c
Contiene el logo convertido al formato compatible con LVGL.
