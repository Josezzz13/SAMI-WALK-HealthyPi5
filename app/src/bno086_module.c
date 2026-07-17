#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
#include "display_module.h"
#include <string.h>

LOG_MODULE_REGISTER(bno086_module, LOG_LEVEL_INF);

#define BNO086_NODE DT_NODELABEL(bno086)

static const struct i2c_dt_spec bno086 = I2C_DT_SPEC_GET(BNO086_NODE);

static uint8_t seq_control = 0;

static int bno086_send_packet(uint8_t channel, uint8_t *data, uint16_t len)
{
    uint8_t packet[32];

    uint16_t total_len = len + 4;

    packet[0] = total_len & 0xFF;
    packet[1] = (total_len >> 8) & 0x7F;
    packet[2] = channel;
    packet[3] = seq_control++;

    memcpy(&packet[4], data, len);

    return i2c_write_dt(&bno086, packet, total_len);
}

void bno086_enable_linear_accel(void)
{
    uint8_t cmd[17] = {0};

    cmd[0] = 0xFD;       // Set Feature Command
    cmd[1] = 0x04;       // Linear Acceleration report
    cmd[2] = 0x00;

    uint32_t interval_us = 100000; // 10 Hz

    cmd[5] = interval_us & 0xFF;
    cmd[6] = (interval_us >> 8) & 0xFF;
    cmd[7] = (interval_us >> 16) & 0xFF;
    cmd[8] = (interval_us >> 24) & 0xFF;

    bno086_send_packet(2, cmd, sizeof(cmd)); // channel 2 = control
}

void bno086_test(void)
{
    if (!device_is_ready(bno086.bus)) {
        LOG_ERR("I2C bus BNO086 no listo");
        return;
    }

    uint8_t hdr[4];
    int ret = i2c_read_dt(&bno086, hdr, sizeof(hdr));

    if (ret == 0)
    {
    	LOG_INF("SHTP Header: %02X %02X %02X %02X",
        	hdr[0], hdr[1], hdr[2], hdr[3]);

   	 hpi_scr_update_spo2(1);
    }
    else
    {
    	LOG_ERR("I2C read error %d", ret);
   	 hpi_scr_update_spo2(-ret);
	 k_msleep(5000);
    }

}

int bno086_read_packet(uint8_t *buffer, int max_len)
{
    uint8_t hdr[4];

    int ret = i2c_read_dt(&bno086, hdr, 4);

    if (ret != 0)
        return ret;

    int packet_len = ((hdr[1] & 0x7F) << 8) | hdr[0];

    if (packet_len < 4)
        return 0;

    if (packet_len > max_len)
        return -1;

    memcpy(buffer, hdr, 4);

    ret = i2c_read_dt(&bno086, buffer + 4, packet_len - 4);

    if (ret != 0)
        return ret;

    return packet_len;
}

int bno086_get_packet_size(void)
{
    uint8_t hdr[4];
    int ret = i2c_read_dt(&bno086, hdr, sizeof(hdr));

    if (ret != 0)
    {
        return -ret;
    }

    int packet_len = ((hdr[1] & 0x7F) << 8) | hdr[0];

    if (packet_len < 4 || packet_len > 512)
    {
        return 0;
    }

    return packet_len;
}

int bno086_get_vel_z(void)
{
    static int vel_z = -16;

    vel_z += 16;

    if (vel_z > 160)
    {
        vel_z = 0;
    }

    return vel_z;
}
