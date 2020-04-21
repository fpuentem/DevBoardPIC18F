
/*
 * _24lc256.c
 *
 * Created: 02/11/2016 04:56:08 p.m.
 *  Author: USUARIO
 */

#include "main.h"
//#include "i2c.h"
#include <plib.h>
#include "24lc256.h"
#include <stdint.h>
#include "main.h"

uint8_t EE24256_Write(uint16_t addr, uint8_t* data, uint8_t len) {
    uint8_t nWrite; //number of bytes to write
    uint8_t nPage; //number of bytes remaing on current page, starting at addr

    if (addr + len > 32768)
        return 1; //error

    while (len) {
        nPage = 64 - (addr % 64);
        nWrite = len < nPage ? len : nPage;
        StartI2C();//I2C_Start();
        WriteI2C(0xA0 | 0x00);//I2C_Write(0xA0 | 0x00);
        WriteI2C(addr >> 8);
        WriteI2C(addr);
        for (uint8_t i = 0; i < nWrite; i++) {
            if (WriteI2C(data[i]))
                return 1;
        }
        StopI2C();//I2C_Stop();

        __delay_ms(10);

        addr += nWrite;
        data += nWrite;
        len -= nWrite;
    }
    return 0;
}

uint8_t EE24256_WriteByte(uint16_t addr, uint8_t data) {
    return EE24256_Write(addr, &data, 1);
}

#define I2C_READ 0x01
#define I2C_WRITE 0x00

uint8_t I2C_Receive(uint8_t address, uint8_t* data, uint16_t length) {

    if(length == 0)
	return 1;

    StartI2C();
    WriteI2C(address | I2C_READ);

    for (uint16_t i = 0; i < (length - 1); i++) {
        data[i] = ReadI2C();
        AckI2C();
    }
    data[(length - 1)] = ReadI2C();
    NotAckI2C();
    StopI2C();

    return 0;
}


uint8_t EE24256_Read(uint16_t addr, uint8_t *data, uint8_t len) {

    if(len == 0)
	return 1;

    if (addr + len > 32768)
        return 1;

    StartI2C();
    WriteI2C(0xA0 | 0x00);
    WriteI2C(addr >> 8);
    WriteI2C(addr);
    StopI2C();
    return I2C_Receive(0xA0, data, len);
}



uint8_t EE24256_ReadByte(uint16_t addr) {
    uint8_t data;
    uint8_t ret;

    ret = EE24256_Read(addr, &data, 1);
    return ret == 0 ? data : -ret;
}
