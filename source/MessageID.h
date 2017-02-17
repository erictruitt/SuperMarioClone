/***************************************************************
|	File:		MessageID.h
|	Author:		Eric Truitt
|	Course:		SGD 1410
|	Purpose:	MessageID enum declares the message enumerators
***************************************************************/

#ifndef MESSAGEID_H
#define MESSAGEID_H


/**************************************************************/
// MessageID enum
//	- enumerated list of message identifiers
enum class MessageID 
{ 
	MSG_UNKNOWN, 
	MSG_DESTROY_ENTITY,
	MSG_CREATE_POWER_UP
};


#endif //MESSAGEID_H