#include "pci_module.h"

#define VEL_REST_LIMIT 80
#define VEL_MAX_VALUE 160

static int current_hr = 0;
static int prev_vel_z = -1;

static int fc_reposo_max = 0;
static int fc_trabajo_max = 0;

// PCI escalado x100
// Ejemplo: 255 = 2.55
static int pci_value_x100 = 0;

void pci_reset(void)
{
    current_hr = 0;
    prev_vel_z = -1;

    fc_reposo_max = 0;
    fc_trabajo_max = 0;

    pci_value_x100 = 0;
}

void pci_update_hr(int hr)
{
    if (hr > 0)
    {
        current_hr = hr;
    }
}

void pci_update_vel_z(int vel_z)
{
    // Detecta fin del ciclo 0–20 cuando vuelve a 0
    if (prev_vel_z != -1 && vel_z < prev_vel_z)
    {
        if (fc_reposo_max > 0 && fc_trabajo_max > 0)
        {
            int diff = fc_trabajo_max - fc_reposo_max;

            if (diff < 0)
            {
                diff = 0;
            }

            pci_value_x100 = ((diff * 100) + (VEL_MAX_VALUE / 2)) / VEL_MAX_VALUE;
        }
        else
        {
            pci_value_x100 = 0;
        }

        // Reinicio para nuevo ciclo
        fc_reposo_max = 0;
        fc_trabajo_max = 0;
    }

    // Primer tramo: reposo
    if (vel_z < VEL_REST_LIMIT)
    {
        if (current_hr > fc_reposo_max)
        {
            fc_reposo_max = current_hr;
        }
    }
    // Segundo tramo: trabajo
    else
    {
        if (fc_trabajo_max < fc_reposo_max)
        {
            fc_trabajo_max = fc_reposo_max;
        }

        if (current_hr > fc_trabajo_max)
        {
            fc_trabajo_max = current_hr;
        }
    }

    prev_vel_z = vel_z;
}

int pci_get_value(void)
{
    return pci_value_x100;
}
