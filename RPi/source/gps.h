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

//------------------------------ Use to get gps data
typedef struct _GPS {
	unsigned long	iTow;		// GPS time of week of the navigation epoch
	unsigned long	hAcc;		// Horizontal accuracy estimate
	unsigned long	vAcc;		// Vertical accuracy estimate
	signed long		longitude;	// 경도
	signed long		latitude;	// 위도
	signed long		height;		// Height above ellipsoid
	signed long		hMSL;		// Height above mean sea level 
}Gps;


//------------------------------ Use to translate for Union
typedef union _GPS_UNION
{
	uint8_t       buff[4];
	float         data_float;
	int           data_int;
	long		  data_long;
	unsigned long data_Ulong;
}GpsUnion;

//------------------------------ Posllh Variable
extern Gps		data;
extern GpsUnion	gps_union;

//------------------------------ GPS Default info
extern uint8_t		new_packet_flag;
extern uint8_t		packet_complete_flag;
extern uint8_t		posllh_checksum_flag;
extern uint8_t		check_flag;
extern uint8_t		count;

extern int	received_size;
extern int	loop_counter;

//------------------------------- Buffer for uart and decoding
extern uint8_t	uart_receive_buff[GPS_DMA_RECEIVE_SIZE];
extern uint8_t gps_packet_buff[GPS_PACKET_SIZE];

//------------------------------- 

void read_gps();

void init_gps();

void receive_gps_packet();

void check_gps_packet();

void decode_gps_posllh_packet();