#include "ch32v00x.h"
#include "i2cSlave.h"
#include "gpio.h"
#include <stdio.h>
#include "debug.h"

#define GPIO_digitalWrite(GPIOv, lowhigh)				GPIO_digitalWrite_##lowhigh(GPIOv)

// The I2C slave library uses a one byte address so you can extend the size of this array up to 256 registers
// note that the register set is modified by interrupts, to prevent the compiler from accidently optimizing stuff
// away make sure to declare the register array volatile

volatile uint8_t i2c_registers[32] = {0x00};
_i2c_slave_state i2c_slave_state;

void onWrite(uint8_t reg, uint8_t length) {
    //funDigitalWrite(PA2, i2c_registers[0] & 1);
    PIN_output(PA2);
}
void setI2CSlave(uint8_t address, volatile uint8_t* registers, uint8_t size, i2c_write_callback_t write_callback, i2c_read_callback_t read_callback, bool read_only) {
    i2c_slave_state.first_write = 1;
    i2c_slave_state.offset = 0;
    i2c_slave_state.position = 0;
    i2c_slave_state.registers1 = registers;
    i2c_slave_state.size1 = size;
    i2c_slave_state.registers2 = NULL;
    i2c_slave_state.size2 = 0;
    i2c_slave_state.write_callback1 = write_callback;
    i2c_slave_state.read_callback1 = read_callback;
    i2c_slave_state.read_only1 = read_only;
    i2c_slave_state.write_callback2 = NULL;
    i2c_slave_state.read_callback2 = NULL;
    i2c_slave_state.read_only2 = false;

    // Enable I2C1
    RCC->APB1PCENR |= RCC_APB1Periph_I2C1;

    // Reset I2C1 to init all regs
    RCC->APB1PRSTR |= RCC_APB1Periph_I2C1;
    RCC->APB1PRSTR &= ~RCC_APB1Periph_I2C1;

    I2C1->CTLR1 |= I2C_CTLR1_SWRST;
    I2C1->CTLR1 &= ~I2C_CTLR1_SWRST;

    // Set module clock frequency
    uint32_t prerate = 2000000; // I2C Logic clock rate, must be higher than the bus clock rate
    I2C1->CTLR2 |= (FUNCONF_SYSTEM_CORE_CLOCK/prerate) & I2C_CTLR2_FREQ;

    // Enable interrupts
    I2C1->CTLR2 |= I2C_CTLR2_ITBUFEN | I2C_CTLR2_ITEVTEN | I2C_CTLR2_ITERREN;

    NVIC_EnableIRQ(I2C1_EV_IRQn); // Event interrupt
    NVIC_SetPriority(I2C1_EV_IRQn, 2 << 4);
    NVIC_EnableIRQ(I2C1_ER_IRQn); // Error interrupt
    NVIC_SetPriority(I2C1_ER_IRQn, 2 << 4);

    // Set clock configuration
    uint32_t clockrate = 1000000; // I2C Bus clock rate, must be lower than the logic clock rate
    I2C1->CKCFGR = ((FUNCONF_SYSTEM_CORE_CLOCK/(3*clockrate))&I2C_CKCFGR_CCR) | I2C_CKCFGR_FS; // Fast mode 33% duty cycle
    //I2C1->CKCFGR = ((FUNCONF_SYSTEM_CORE_CLOCK/(25*clockrate))&I2C_CKCFGR_CCR) | I2C_CKCFGR_DUTY | I2C_CKCFGR_FS; // Fast mode 36% duty cycle
    //I2C1->CKCFGR = (FUNCONF_SYSTEM_CORE_CLOCK/(2*clockrate))&I2C_CKCFGR_CCR; // Standard mode good to 100kHz

    // Set I2C address
    I2C1->OADDR1 = address << 1;
    I2C1->OADDR2 = 0;

    // Enable I2C
    I2C1->CTLR1 |= I2C_CTLR1_PE;

    // Acknowledge bytes when they are received
    I2C1->CTLR1 |= I2C_CTLR1_ACK;
}

int i2cSlave() {
    SystemInit();
    //funGpioInitAll();
    PORTA_enable();
    PORTC_enable();

    // Initialize I2C slave
    //funPinMode(PC1, GPIO_CFGLR_OUT_10Mhz_AF_OD); // SDA
    PIN_output_OD(PC1);
    //funPinMode(PC2, GPIO_CFGLR_OUT_10Mhz_AF_OD); // SCL
    PIN_output_OD(PC2);
    setI2CSlave(0x9, i2c_registers, sizeof(i2c_registers), onWrite, NULL, false);

    // Initialize LED
    //funPinMode(PA2, GPIO_CFGLR_OUT_10Mhz_PP); // LED
    PIN_output(PA2);

    //while (1) {} // Do not let main exit, you can do other things here
}