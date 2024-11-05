/*
 * Single-File-Header for using the I2C peripheral in slave mode
 *
 * MIT License
 *
 * Copyright (c) 2024 Renze Nicolai
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __I2C_SLAVE_H
#define __I2C_SLAVE_H

#include <ch32v00x.h>
#include <ch32v00x_i2c.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define FUNCONF_SYSTEM_CORE_CLOCK 48000000  // Computed Clock in Hz (Default only for 003, other chips have other defaults)

typedef void (*i2c_write_callback_t)(uint8_t reg, uint8_t length);
typedef void (*i2c_read_callback_t)(uint8_t reg);

extern void setI2CSlave(uint8_t address, volatile uint8_t* registers, uint8_t size, i2c_write_callback_t write_callback, i2c_read_callback_t read_callback, bool read_only);


typedef struct  {
    uint8_t first_write;
    uint8_t offset;
    uint8_t position;
    volatile uint8_t* volatile registers1;
    uint8_t size1;
    volatile uint8_t* volatile registers2;
    uint8_t size2;
    i2c_write_callback_t write_callback1;
    i2c_read_callback_t read_callback1;
    bool read_only1;
    i2c_write_callback_t write_callback2;
    i2c_read_callback_t read_callback2;
    bool read_only2;
    bool writing;
    bool address2matched;
} _i2c_slave_state;


/*
void SetupSecondaryI2CSlave(uint8_t address, volatile uint8_t* registers, uint8_t size, i2c_write_callback_t write_callback, i2c_read_callback_t read_callback, bool read_only) {
    if (address > 0) {
        I2C1->OADDR2 = (address << 1) | 1;
        i2c_slave_state.registers2 = registers;
        i2c_slave_state.size2 = size;
        i2c_slave_state.write_callback2 = write_callback;
        i2c_slave_state.read_callback2 = read_callback;
        i2c_slave_state.read_only2 = read_only;
    } else {
        I2C1->OADDR2 = 0;
    }
}

void SetI2CSlaveReadOnly(bool read_only) {
    i2c_slave_state.read_only1 = read_only;
}

void SetSecondaryI2CSlaveReadOnly(bool read_only) {
    i2c_slave_state.read_only2 = read_only;
}

void I2C1_EV_IRQHandler(void) __attribute__((interrupt));
void I2C1_EV_IRQHandler(void) {
    uint16_t STAR1, STAR2 __attribute__((unused));
    STAR1 = I2C1->STAR1;
    STAR2 = I2C1->STAR2;

    if (STAR1 & I2C_STAR1_ADDR) { // Start event
        i2c_slave_state.first_write = 1; // Next write will be the offset
        i2c_slave_state.position = i2c_slave_state.offset; // Reset position
        i2c_slave_state.address2matched = !!(STAR2 & I2C_STAR2_DUALF);
    }

    if (STAR1 & I2C_STAR1_RXNE) { // Write event
        if (i2c_slave_state.first_write) { // First byte written, set the offset
            i2c_slave_state.offset = I2C1->DATAR;
            i2c_slave_state.position = i2c_slave_state.offset;
            i2c_slave_state.first_write = 0;
            i2c_slave_state.writing = false;
        } else { // Normal register write
            i2c_slave_state.writing = true;
            if (i2c_slave_state.address2matched) {
                if (i2c_slave_state.position < i2c_slave_state.size2 && !i2c_slave_state.read_only2) {
                    i2c_slave_state.registers2[i2c_slave_state.position] = I2C1->DATAR;
                    i2c_slave_state.position++;
                }
            } else {
                if (i2c_slave_state.position < i2c_slave_state.size1 && !i2c_slave_state.read_only1) {
                    i2c_slave_state.registers1[i2c_slave_state.position] = I2C1->DATAR;
                    i2c_slave_state.position++;
                }
            }
        }
    }

    if (STAR1 & I2C_STAR1_TXE) { // Read event
        i2c_slave_state.writing = false;
        if (i2c_slave_state.address2matched) {
            if (i2c_slave_state.position < i2c_slave_state.size2) {
                I2C1->DATAR = i2c_slave_state.registers2[i2c_slave_state.position];
                if (i2c_slave_state.read_callback2 != NULL) {
                    i2c_slave_state.read_callback2(i2c_slave_state.position);
                }
                i2c_slave_state.position++;
            } else {
                I2C1->DATAR = 0x00;
            }
        } else {
            if (i2c_slave_state.position < i2c_slave_state.size1) {
                I2C1->DATAR = i2c_slave_state.registers1[i2c_slave_state.position];
                if (i2c_slave_state.read_callback1 != NULL) {
                    i2c_slave_state.read_callback1(i2c_slave_state.position);
                }
                i2c_slave_state.position++;
            } else {
                I2C1->DATAR = 0x00;
            }
        }
    }

    if (STAR1 & I2C_STAR1_STOPF) { // Stop event
        I2C1->CTLR1 &= ~(I2C_CTLR1_STOP); // Clear stop
        if (i2c_slave_state.address2matched) {
            if (i2c_slave_state.write_callback2 != NULL) {
                i2c_slave_state.write_callback2(i2c_slave_state.offset, i2c_slave_state.position - i2c_slave_state.offset);
            }
        } else {
            if (i2c_slave_state.write_callback1 != NULL) {
                i2c_slave_state.write_callback1(i2c_slave_state.offset, i2c_slave_state.position - i2c_slave_state.offset);
            }
        }
    }
}

void I2C1_ER_IRQHandler(void) __attribute__((interrupt));
void I2C1_ER_IRQHandler(void) {
    uint16_t STAR1 = I2C1->STAR1;

    if (STAR1 & I2C_STAR1_BERR) { // Bus error
        I2C1->STAR1 &= ~(I2C_STAR1_BERR); // Clear error
    }

    if (STAR1 & I2C_STAR1_ARLO) { // Arbitration lost error
        I2C1->STAR1 &= ~(I2C_STAR1_ARLO); // Clear error
    }

    if (STAR1 & I2C_STAR1_AF) { // Acknowledge failure
        I2C1->STAR1 &= ~(I2C_STAR1_AF); // Clear error
    }
}
*/
#endif