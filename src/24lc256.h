
#ifndef _24LC256_H
#define	_24LC256_H

#ifdef	__cplusplus
extern "C" {
#endif

uint8_t EE24256_Write(uint16_t addr, uint8_t* data, uint8_t len);
uint8_t EE24256_WriteByte(uint16_t addr, uint8_t data);
uint8_t EE24256_Read(uint16_t addr, uint8_t *data, uint8_t len);
uint8_t EE24256_ReadByte(uint16_t addr);


#ifdef	__cplusplus
}
#endif

#endif	/* 24LC256_H */

