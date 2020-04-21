/*
* parser.c
*
* Created: 27/10/2016 07:31:54 p.m.
*  Author: USUARIO
*/



#include <string.h>
#include <stdbool.h>
#include "parser.h"

enum {
	START = 0x7E, ESC = 0x7D, XON = 0x11, XOFF = 0x13
};

enum ParsingState {
	WAIT_START, GET_LENMSB, GET_LENLSB, GET_DATA, GET_CHECKSUM
} _parserState = WAIT_START;

uint8_t packetReceived[DATAFRAME_MAXLENGTH];
uint8_t packetReceived_length;

/**
*
* @param buf: Puntero al buffer del serial con los bytes a leer.
* @param len: Longitud del buffer.
* @return : True si un paquete ha sido recibido correctamente.
*/
bool Parser_Process(uint8_t* buf, uint16_t len) {
	static uint8_t _bytesReceived;
	static uint8_t _byteEscaped = false;
	static uint8_t _chksum;
	static uint8_t _packetLength;
	static uint8_t _currentPacket[DATAFRAME_MAXLENGTH];
	uint8_t _currentByte;
	bool _packetIsAvailable = false;
	uint8_t i = 0;

	while (i < len) {
		_currentByte = buf[i++];

		if (_currentByte == START) {
			_byteEscaped = false;
			_bytesReceived = 0;
			_chksum = 0;
			_parserState = GET_LENMSB; //Next State
			continue;
		}

		if (_byteEscaped) {
			_byteEscaped = false;
			_currentByte = _currentByte^0x20;
			} else if (_currentByte == ESC) {
			_byteEscaped = true;
			continue;
		}

		switch (_parserState) {
			case WAIT_START:
			break;
			
			case GET_LENMSB:
			//_packetLength = ((uint16_t) _currentByte) << 8;
			_packetLength = 0x00; //Con fines de optimización.
			_parserState = GET_LENLSB;
			break;
			
			case GET_LENLSB:
			//_packetLength += (uint16_t) _currentByte;
			_packetLength = _currentByte;
			if (_packetLength <= DATAFRAME_MAXLENGTH) {
				_parserState = GET_DATA;
			} else
			_parserState = WAIT_START;
			break;
			

			case GET_DATA:
			_chksum += _currentByte;
			_currentPacket[_bytesReceived++] = _currentByte;

			if (_bytesReceived == _packetLength) {
				_parserState = GET_CHECKSUM;
			}
			break;
			
			case GET_CHECKSUM:
			_chksum += _currentByte;
			if ((_chksum & 0xff) == 0xff) {

				memcpy(&packetReceived[0], &_currentPacket[0], _packetLength);
				packetReceived_length = _packetLength;
				_packetIsAvailable = true;
			}
			_parserState = WAIT_START;
			break;
			
		}
	}
	return _packetIsAvailable;
}

/**
*
* @param packet: Puntero a byte del array en donde se copiara el paquete recibido
* @return : Longitud del paquete recibido.
*/
uint8_t Parser_GetPacket(uint8_t* packet) {
	memcpy(packet, &packetReceived[0], packetReceived_length);
	return packetReceived_length;
}