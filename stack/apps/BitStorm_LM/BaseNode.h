/*
 * EndDevice.h
 *
 * Created: 7/8/2014 1:23:53 PM
 *  Author: Eric Rudisill
 */ 


#ifndef BASENODE_H_
#define BASENODE_H_

#include <stdlib.h>
#include "nwk.h"
#include "sysTimer.h"
#include "BitStorm.h"

typedef void (*appDataConf_ptr_t)(NWK_DataReq_t *req);
typedef bool (*appDataInd_ptr_t)(NWK_DataInd_t *ind);
typedef void (*appDataSendingTimerHandler_ptr_t)(SYS_Timer_t *timer);

extern NWK_DataReq_t nwkDataReq;
extern AppMessage_t appMsg;
extern uint16_t mac_short;
extern SYS_Timer_t appDataSendingTimer;
extern SYS_Timer_t routerAgingTimer;

void baseNode_lowPower(void);
void baseNode_sendDataCurrent(appDataConf_ptr_t);
void baseNode_sendData(appDataConf_ptr_t);
void baseNode_initDevice(void);
void baseNode_initMessage(void);
void baseNode_initNetwork(appDataInd_ptr_t);
void baseNode_initDataSendingTimer(appDataSendingTimerHandler_ptr_t);
void baseNode_initAgingTimer(appDataSendingTimerHandler_ptr_t);
void baseNode_setAckByte(uint8_t ack);

#endif /* BASENODE_H_ */