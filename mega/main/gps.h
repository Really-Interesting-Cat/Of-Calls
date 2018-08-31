#include <stdint.h>
#include <string.h>

//------------------------------ Definition Constant
#define GPS_DMA_RECEIVE_SIZE 1024
#define GPS_PACKET_SIZE 36

#define STD_LONGITUDE_DEGREES 127
#define STD_LONGITUDE_MINUTE 24
#define STD_LONGITUDE_SECOND 51

#define STD_LATITUDE_DEGREES 36
#define STD_LATITUDE_MINUTE 19
#define STD_LATITUDE_SECOND 10

#define LATITUDE_DEGREES_DISTANCE 111
#define LATITUDE_MINUTE_DISTANCE 1.85
#define LATITUDE_SECOND_DISTANCE 30.8

//------------------------------ Use to get gps datag
typedef struct _GPS {
	unsigned long	iTow;		// GPS time of week of the navigation epoch
	unsigned long	hAcc;		// Horizontal accuracy estimate
	unsigned long	vAcc;		// Vertical accuracy estimate
	signed long		longitude;	// �浵
	signed long		latitude;	// ����
	signed long		height;		// Height above ellipsoid
	signed long		hMSL;		// Height above mean sea level 
}Gps;


//------------------------------ Use to translate for Union
typedef union _GPS_UNION
{
	uint8_t       buff[4];
	float         datag_float;
	int           datag_int;
	long		  datag_long;
	unsigned long datag_Ulong;
}GpsUnion;

//------------------------------ Posllh Variable
extern Gps		datag;
extern GpsUnion	gps_union;

//------------------------------ GPS Default info
extern uint8_t		new_packet_flag;
extern uint8_t		packet_complete_flag;
extern uint8_t		posllh_checksum_flag;
extern uint8_t		checksum_flag;
extern uint8_t		count;

extern int	received_size;
extern int	loop_counter;

//------------------------------- Buffer for uart and decoding
extern uint8_t	uart_receive_buff[GPS_DMA_RECEIVE_SIZE];
extern uint8_t gps_packet_buff[GPS_PACKET_SIZE];

//------------------------------- 

void init_gps();

void check_gps_packet();

void decode_gps_posllh_packet();
