
#ifndef __I2C_H__
#define __I2C_H__

#define I2C_PORT_OUT            P4OUT
#define I2C_PORT_DIR            P4DIR
#define I2C_PORT_SEL            P4SEL
#define I2C_SCL_PIN             BIT2
#define I2C_SDA_PIN             BIT1

extern void i2c_init(uint8_t deviceAddress);
extern void i2c_write_register(uint8_t registerAddress, uint16_t value);
extern uint16_t i2c_read_register(uint8_t registerAddress);


#endif /* __I2C_H__ */
