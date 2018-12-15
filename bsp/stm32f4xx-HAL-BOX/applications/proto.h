#ifndef _PROTO_H_
#define _PROTO_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>


//#define PROTO_DEBUG

#if defined(__cplusplus)
extern "C" {
#endif

extern uint8_t value89;

	
#define PROTO_NULL	0
#define PROTO_FIND	1

	
typedef void (*proto_send_cb)(uint8_t*, uint16_t);
	
	
/*
 * proto data analyze by byte
 * return: PROTO_NULL
 * 			PROTO_FIND
 */
int Proto_data_analyze_byte(uint8_t data);

/*
 * proto data analyze by array
 * return: PROTO_NULL
 * 			PROTO_FIND
 */
int Proto_data_analyze(uint8_t *data, uint8_t size);

/*
 * proto sensor cmd deal
 * when Proto_data_analyze_byte or Proto_data_analyze return PROTO_FIND call it
 * return: PROTO_NULL
 * 			PROTO_FIND
 */
int Proto_sensor_cmd_deal(void);

/*
 * proto set send function
 * return: NULL
 */
void proto_set_sendFunc(proto_send_cb sendfunc);





void DZP_deal(void);
void DHT11_deal(void);
void DS18B20_deal(void);
void DZP1_deal(void);
void MOTOR_deal(void);
void YG_deal(void);
void jiujing_deal(void);
void XZDWQ_deal(void);
void SOUNDSENSOR_deal(void);
void Dial_deal(void);
void traffic_deal(void);
void GUI_deal(void);
uint8_t Dial_value_get(void);

#if defined(PROTO_DEBUG)
void RunTests_proto(void);
#endif



#if defined(__cplusplus)
}
#endif

#endif
