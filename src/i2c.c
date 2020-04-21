

#include "main.h"
#include "i2c.h"
#define SSPADD_VALUE  ((_XTAL_FREQ/4)/I2C_BAUDRATE - 1)

volatile uint8_t i2cTimeout = 0;

void I2C_Init(void) {
    TRISB1 = 1; /* SDA and SCL as input pin */
    TRISB0 = 1; /* these pins can be configured either i/p or o/p */
    SSPSTATbits.SMP = 1; /* Slew rate disabled */
    //    SSPCONbits.SSPEN = 1; //Enable SSP module
    //    SSPCONbits.SSPM = 0b1000; //Master mode
    SSPSTATbits.CKE = 1; //?¿
    SSPCON1bits.SSPEN = 1; //SSPEN = 1,
    SSPCON1bits.SSPM = 0x08; /* I2C Master mode, clock = FOSC/(4 * (SSPADD + 1)) */
    SSPCON2 = 0;
    PIR1bits.SSPIF = 0;
    PIR2bits.BCLIF = 0;
    SSPADD = SSPADD_VALUE;
}
void I2C_Idle(){
     i2cTimeout = 200;
    while (((!!(SSPCON2 & 0x1F)) || (!!(SSPSTATbits.R_W))) && (i2cTimeout > 0));
}
/********************************************************************
 *     Return Value:     void                                        *
 *     Parameters:       void                                        *
 *     Description:      Send I2C bus start condition.               *
 ********************************************************************/
void I2C_Start(void) {
    SSPCON2bits.SEN = 1;
    i2cTimeout = 200;
    while ((!!SSPCON2bits.SEN) && (i2cTimeout > 0));
}

/********************************************************************
 *     Return Value:     void                                        *
 *     Parameters:       void                                        *
 *     Description:      Send I2C bus restart condition.             *
 ********************************************************************/
void I2C_Restart(void) {
    SSPCON2bits.RSEN = 1;
    i2cTimeout = 200;
    while ((!!SSPCON2bits.RSEN) && (i2cTimeout > 0));
}

/********************************************************************
 *     Function Name:    WriteI2C                                    *
 *     Return Value:     Status byte for WCOL detection.             *
 *     Parameters:       Single data byte for I2C bus.               *
 *     Description:      This routine writes a single byte to the    *
 *                       I2C bus.                                    *
 ********************************************************************/
uint8_t I2C_Write(uint8_t data) {

    I2C_Idle();

    SSPBUF = data; // write single byte to SSPBUF

    I2C_Idle();

//    i2cTimeout = 200;
//    while ((!!SSPSTATbits.BF) && (i2cTimeout > 0)); // wait until write cycle is complete
//    if(!i2cTimeout)return 1;
//    //I2C_Wait();
//    i2cTimeout = 200;
//    while (((!!(SSPCON2 & 0x1F)) || (!!(SSPSTATbits.RW))) && (i2cTimeout > 0));
//    if(!i2cTimeout)return 1;
    return 0;
}

/********************************************************************
 *     Function Name:    I2C_Read                                    *
 *     Return Value:     contents of SSPBUF register                 *
 *     Parameters:       void                                        *
 *     Description:      Read single byte from I2C bus.              *
 ********************************************************************/
uint8_t I2C_Read(void) {
    uint8_t tmp;

    I2C_Idle();
    

    SSPCON2bits.RCEN = 1;
    i2cTimeout = 200;
    while ((!SSPSTATbits.BF) && (i2cTimeout > 0)); // wait until byte received
    tmp = SSPBUF;
    //I2C_Wait();
    i2cTimeout = 200;
    while (((!!(SSPCON2 & 0x1F)) || (!!(SSPSTATbits.RW))) && (i2cTimeout > 0));
    return tmp; // return with read byte
}

/********************************************************************
 *     Return Value:     void                                        *
 *     Parameters:       void                                        *
 *     Description:      Send I2C bus stop condition.                *
 ********************************************************************/
void I2C_Stop(void) {

    I2C_Idle();

    SSPCON2bits.PEN = 1;
    i2cTimeout = 200;
    while ((!!SSPCON2bits.PEN) && (i2cTimeout > 0));
}

/********************************************************************
 *     Return Value:     void                                        *
 *     Parameters:       void                                        *
 *     Description:      Initiate ACK bus condition.                 *
 ********************************************************************/
void I2C_Ack(void) {
    SSPCON2bits.ACKDT = 0; // set acknowledge bit state for ACK
    SSPCON2bits.ACKEN = 1; // initiate bus acknowledge sequence
    i2cTimeout = 200;
    while ((!!SSPCON2bits.ACKEN) && (i2cTimeout > 0));
}

/********************************************************************
 *     Return Value:     void                                        *
 *     Parameters:       void                                        *
 *     Description:      Initiate NOT ACK bus condition.             *
 ********************************************************************/
void I2C_NotAck(void) {
    SSPCON2bits.ACKDT = 1; // set acknowledge bit for not ACK
    SSPCON2bits.ACKEN = 1; // initiate bus acknowledge sequence
    i2cTimeout = 200;
    while ((!!SSPCON2bits.ACKEN) && (i2cTimeout > 0));
}

/*
Function: I2C_Wait
Return:
Arguments:
Description: wait for transfer to finish
 */
void I2C_Wait() {
    i2cTimeout = 200;
    while (((!!(SSPCON2 & 0x1F)) || (!!(SSPSTATbits.RW))) && (i2cTimeout > 0));
    /* wait for any pending transfer */
}

#define I2C_READ 0x01
#define I2C_WRITE 0x00

uint8_t I2C_Receive(uint8_t address, uint8_t* data, uint16_t length) {

    if(length == 0)
	return 1;

    I2C_Start();
    I2C_Write(address | I2C_READ);

    for (uint16_t i = 0; i < (length - 1); i++) {
        data[i] = I2C_Read();
        I2C_Ack();
    }
    data[(length - 1)] = I2C_Read();
    I2C_NotAck();
    I2C_Stop();

    return 0;
}