#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>

#include "bno086_sh2/sh2/sh2_hal.h"

#define BNO086_NODE DT_NODELABEL(bno086)

static const struct i2c_dt_spec bno086 =
    I2C_DT_SPEC_GET(BNO086_NODE);
