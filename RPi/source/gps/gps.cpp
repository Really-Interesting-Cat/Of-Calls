#include "gps.h"

Gps			data;
GpsUnion	gps_union;

uint8_t		new_packet_flag;
uint8_t		packet_complete_flag;
uint8_t		posllh_checksum_flag;
uint8_t		checksum_flag;
uint8_t		count;

int	received_size;
int	loop_counter;

uint8_t	uart_receive_buff[GPS_DMA_RECEIVE_SIZE];
uint8_t gps_packet_buff[GPS_PACKET_SIZE];

void init_gps()
{
	new_packet_flag = 1;
	packet_complete_flag = 0;
	checksum_flag = 0;
	count = 0;
	loop_counter = 0;
}

void check_gps_packet()
{
	int i;
	uint8_t CK_A, CK_B;

	CK_A = 0;
	CK_B = 0;

	for (i = 2; i < GPS_PACKET_SIZE - 2; i++)
	{
		CK_A = CK_A + gps_packet_buff[i];
		CK_B = CK_B + CK_A;
	}

	if ((CK_A != gps_packet_buff[GPS_PACKET_SIZE - 2]) || (CK_B != gps_packet_buff[GPS_PACKET_SIZE - 1]))
		checksum_flag = 0;

	else
		checksum_flag = 1;
}

void decode_gps_posllh_packet()
{
	int   i = 0;
	int   j = 0;
	int   temp = 0;
	float float_temp = 0.0;
	int   buff_idx = 6;

	for (j = 0; j < 4; j++) // iTow
		gps_union.buff[j] = gps_packet_buff[buff_idx++];
	data.iTow = gps_union.data_Ulong;

	for (j = 0; j < 4; j++) // Longitude
		gps_union.buff[j] = gps_packet_buff[buff_idx++];
	data.longitude = gps_union.data_long;

	for (j = 0; j < 4; j++) // Latitude
		gps_union.buff[j] = gps_packet_buff[buff_idx++];
	data.latitude = gps_union.data_long;

	for (j = 0; j < 4; j++) // height
		gps_union.buff[j] = gps_packet_buff[buff_idx++];
	data.height = gps_union.data_long;

	for (j = 0; j < 4; j++) // hMSL
		gps_union.buff[j] = gps_packet_buff[buff_idx++];
	data.hMSL = gps_union.data_long;

	for (j = 0; j < 4; j++) // hAcc
		gps_union.buff[j] = gps_packet_buff[buff_idx++];
	data.hAcc = gps_union.data_Ulong;

	for (j = 0; j < 4; j++) // vAcc
		gps_union.buff[j] = gps_packet_buff[buff_idx++];
	data.vAcc = gps_union.data_Ulong;
}
