/*
 * Copyright (C) 2018 Kubos Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <nanopower-api/nanopower-api.h>
#include <kubos-hal/i2c.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

KEPSStatus k_eps_init()
{
    /*
     * All I2C configuration is done at the kernel level,
     * but we still need to pass a config structure to make
     * our I2C API happy.
     */
    KI2CConf conf = k_i2c_conf_defaults();

    KI2CStatus status;
    status = k_i2c_init(EPS_I2C_BUS, &conf);
    if (status != I2C_OK)
    {
        fprintf(stderr, "Failed to initialize EPS: %d\n", status);
        return EPS_ERROR;
    }

    return EPS_OK;
}

void k_eps_terminate()
{
    k_i2c_terminate(EPS_I2C_BUS);

    return;
}

KEPSStatus k_eps_configure_system(const eps_system_config_t * config)
{
    KEPSStatus status = EPS_OK;

    //TODO

    return status;
}

KEPSStatus k_eps_configure_battery(const eps_battery_config_t * config)
{
    KEPSStatus status = EPS_OK;

    //TODO

    return status;
}

KEPSStatus k_eps_ping()
{
    KEPSStatus status = EPS_OK;

    //TODO

    //TODO: non-standard response
    return status;
}

KEPSStatus k_eps_reset()
{
    KEPSStatus ret = EPS_OK;
    KI2CStatus  status;
    uint8_t     cmd = HARD_RESET;

    status = k_i2c_write(EPS_I2C_BUS, EPS_I2C_ADDR, (uint8_t *) &cmd, 1);
    if (status != I2C_OK)
    {
        fprintf(stderr, "Failed to reset EPS: %d\n",
                status);
        ret = EPS_ERROR;
    }

    return ret;
}

KEPSStatus k_eps_reboot()
{
    KEPSStatus ret = EPS_OK;
    KI2CStatus status;
    uint8_t packet[] = {0x80, 0x07, 0x80, 0x07};

    status = k_i2c_write(EPS_I2C_BUS, EPS_I2C_ADDR, packet, sizeof(packet));
    if (status != I2C_OK)
    {
        fprintf(stderr, "Failed to reboot EPS: %d\n",
                status);
        ret = EPS_ERROR;
    }

    return ret;
}

KEPSStatus k_eps_set_output(uint8_t channel_mask)
{
    KEPSStatus status = EPS_OK;

    //TODO

    return status;
}

KEPSStatus k_eps_set_single_output(uint8_t channel, uint8_t value, int16 delay)
{
    KEPSStatus status = EPS_OK;

    //TODO

    return status;
}

KEPSStatus k_eps_set_input_value(uint16 in1_voltage, uint16 in2_voltage, uint16 in3_voltage)
{
    KEPSStatus status = EPS_OK;

    //TODO

    return status;
}

KEPSStatus k_eps_set_input_mode(uint8_t mode)
{
    KEPSStatus status = EPS_OK;

    //TODO

    return status;
}

KEPSStatus k_eps_set_heater(uint8_t cmd, uint8_t header, uint8_t mode)
{
    KEPSStatus status = EPS_OK;

    //TODO
    //TODO: response
    return status;
}

KEPSStatus k_eps_get_housekeeping(eps_hk_t * buff)
{
    KEPSStatus status = EPS_OK;

    //TODO

    //TODO: response
    return status;
}

KEPSStatus k_eps_get_system_config(eps_system_config_t * buff)
{
    KEPSStatus status = EPS_OK;

    //TODO
    //TODO: response
    return status;
}

KEPSStatus k_eps_get_battery_config(eps_battery_config_t * buff)
{
    KEPSStatus status = EPS_OK;

    //TODO
    //TODO: response
    return status;
}

KEPSStatus k_eps_get_heater(uint8_t * bp4, uint8_t * onboard)
{
    KEPSStatus status = EPS_OK;

    //TODO
    //TODO: response
    return status;
}

{
    KEPSStatus status = EPS_OK;

    //TODO
    //TODO: response
    return status;
}

KEPSStatus k_eps_watchdog_kick()
{
    KI2CStatus  status;
    KEPSStatus ret = EPS_OK;
    uint8_t     cmd = WATCHDOG_RESET;

    //TODO. This has a SUUUUUUPER long sleep time between kicks

    return ret;
}

pthread_t handle_watchdog = { 0 };

void * kprv_eps_watchdog_thread(void * args)
{
    KEPSStatus status;

    while (1)
    {
        k_eps_watchdog_kick();

        sleep(EPS_WD_TIMEOUT / 3);
    }

    return NULL;
}

KEPSStatus k_eps_watchdog_start()
{
    if (handle_watchdog != 0)
    {
        fprintf(stderr, "EPS watchdog thread already started\n");
        return EPS_OK;
    }

    if (EPS_WD_TIMEOUT == 0)
    {
        fprintf(
            stderr,
            "EPS watchdog has been disabled. No thread will be started\n");
        return EPS_OK;
    }

    if (pthread_create(&handle_watchdog, NULL, kprv_eps_watchdog_thread, NULL)
        != 0)
    {
        perror("Failed to create EPS watchdog thread");
        handle_watchdog = 0;
        return EPS_ERROR;
    }

    return EPS_OK;
}

KEPSStatus k_eps_watchdog_stop()
{
    /* Send the cancel request */
    if (pthread_cancel(handle_watchdog) != 0)
    {
        perror("Failed to cancel EPS watchdog thread");
        return EPS_ERROR;
    }

    /* Wait for the cancellation to complete */
    if (pthread_join(handle_watchdog, NULL) != 0)
    {
        perror("Failed to rejoin EPS watchdog thread");
        return EPS_ERROR;
    }

    handle_watchdog = 0;

    return EPS_OK;
}

KEPSStatus k_eps_passthrough(const uint8_t * tx, int tx_len, uint8_t * rx,
                               int rx_len)
{
    if (tx == NULL || tx_len < 1 || (rx == NULL && rx_len != 0))
    {
        return EPS_ERROR_CONFIG;
    }

    KI2CStatus status;

    status = k_i2c_write(EPS_I2C_BUS, EPS_I2C_ADDR, (uint8_t *) tx, tx_len);
    if (status != I2C_OK)
    {
        fprintf(stderr, "Failed to send EPS passthrough packet: %d\n", status);
        return EPS_ERROR;
    }

    if (rx_len != 0)
    {
        status = k_i2c_read(EPS_I2C_BUS, EPS_I2C_ADDR, rx, rx_len);
        if (status != I2C_OK)
        {
            fprintf(stderr, "Failed to read EPS passthrough response: %d\n",
                    status);
            return EPS_ERROR;
        }
    }

    return EPS_OK;
}

KEPSStatus kprv_eps_transfer(const uint8_t * tx, int tx_len, uint8_t * rx,
                               int rx_len)
{
    KI2CStatus status;

    if (tx == NULL || tx_len < 1 || rx == NULL
        || rx_len < (int) sizeof(eps_resp_header))
    {
        return ADCS_ERROR_CONFIG;
    }

    status = k_i2c_write(IMTQ_I2C_BUS, EPS_I2C_ADDR, (uint8_t *) tx, tx_len);
    if (status != I2C_OK)
    {
        fprintf(stderr, "Failed to send MTQ command: %d\n", status);
        return EPS_ERROR;
    }

    status = k_i2c_read(EPS_I2C_BUS, EPS_I2C_ADDR, rx, rx_len);

    if (status != I2C_OK)
    {
        fprintf(stderr, "Failed to read EPS response (%x): %d\n", tx[0],
                status);
        return EPS_ERROR;
    }

    eps_resp_header response = {.cmd = rx[0], .status = rx[1] };

    if (response.cmd == 0xFF)
    {
        /*
         * This isn't always an error, so we'll let the caller decide whether
         * or not to print an error message
         */
        return EPS_ERROR_NO_RESPONSE;
    }
    else if (response.cmd != tx[0])
    {
        /* Echoed command should match command requested */
        fprintf(stderr, "Command mismatch - Sent: %x Received: %x\n", tx[0],
                response.cmd);
        return EPS_ERROR;
    }

    /* Check the status byte */
    if (response.status != 0)
    {
        fprintf(stderr, "EPS returned an error (%x): %d\n", tx[0],
                response.status);
        return EPS_ERROR_INTERNAL;
    }

    return EPS_OK;
}