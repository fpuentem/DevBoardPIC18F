/*
 * parser.h
 *
 * Created: 27/10/2016 07:31:39 p.m.
 *  Author: USUARIO
 */ 


#ifndef PARSER_H_
#define PARSER_H_


#include <stdint.h>
#include <stdbool.h>

#define DATAFRAME_MAXLENGTH 36


bool Parser_Process(uint8_t* buf, uint16_t len);
uint8_t Parser_GetPacket(uint8_t* p_packet);



#endif /* PARSER_H_ */