/* 
 * File:   i2c.h
 * Author: GUS
 *
 * Created on 9 de abril de 2016, 05:42 PM
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdint.h>

#define I2C_BAUDRATE 400000

    void I2C_Init(void);
    void I2C_Start(void);
    void I2C_Restart(void);
    uint8_t I2C_Write(uint8_t data);
    uint8_t I2C_Read(void);
    void I2C_Stop(void);
    void I2C_Ack(void);
    void I2C_NotAck(void);
    void I2C_Wait(void);
    uint8_t I2C_Receive(uint8_t address, uint8_t* data, uint16_t length);


#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

