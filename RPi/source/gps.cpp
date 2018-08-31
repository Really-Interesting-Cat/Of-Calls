#include "gps.h"

Gps			data;
GpsUnion	gps_union;

uint8_t		new_packet_flag;
uint8_t		packet_complete_flag;
uint8_t		posllh_checksum_flag;
uint8_t		check_flag;
uint8_t		count;

int	received_size;
int	loop_counter;

uint8_t	uart_receive_buff[GPS_DMA_RECEIVE_SIZE];
uint8_t gps_packet_buff[GPS_PACKET_SIZE];

void read_gps()
{
	receive_gps_packet();

	if (packet_complete_flag == 1)
	{
		check_gps_packet();
		packet_complete_flag = 0;

		if (posllh_checksum_flag == 1)
		{
			posllh_checksum_flag = 0;

			decode_gps_posllh_packet();

			loop_counter++;
		}
	}

}

void init_gps()
{
	new_packet_flag = 1;
	packet_complete_flag = 0;
	check_flag = 0;
	count = 0;
	loop_counter = 0;

}


void receive_gps_packet()
{
	static int prev_ndt = GPS_DMA_RECEIVE_SIZE, curr_ndt = 0;
	static int new_data_start_idx = 0, next_data_start_idx = 0;
	static int packet_start_idx = 0, packet_end_idx = GPS_DMA_RECEIVE_SIZE;
	int check_idx, next_check_idx;
	uint8_t received_gps_data_size = 0;

	if (prev_ndt > curr_ndt)
		received_gps_data_size = prev_ndt - curr_ndt;

	else if (prev_ndt < curr_ndt)
		received_gps_data_size = prev_ndt + (GPS_DMA_RECEIVE_SIZE - curr_ndt);

	else
		return;

	received_size += received_gps_data_size;
	next_data_start_idx = (new_data_start_idx + received_gps_data_size) % GPS_DMA_RECEIVE_SIZE;

	check_idx = new_data_start_idx - 1;
	next_check_idx = check_idx + 1;
	if (check_idx == -1)
	{
		check_idx = GPS_DMA_RECEIVE_SIZE - 1;
		next_check_idx = 0;
	}

	while (!(next_check_idx == next_data_start_idx))
	{
		if ((new_packet_flag) && (uart_receive_buff[check_idx] == 0xB5) && (uart_receive_buff[next_check_idx] == 0x62))
		{
			new_packet_flag = 0;
			packet_start_idx = check_idx;
			packet_end_idx = (packet_start_idx + (GPS_PACKET_SIZE - 1)) % GPS_DMA_RECEIVE_SIZE;
		}

		check_idx = (check_idx + 1) % GPS_DMA_RECEIVE_SIZE;
		next_check_idx = (check_idx + 1) % GPS_DMA_RECEIVE_SIZE;


		if (check_idx == packet_end_idx)
		{
			if (packet_start_idx < packet_end_idx)
			{
				memcpy(gps_packet_buff, (uart_receive_buff + packet_start_idx), ((packet_end_idx - packet_start_idx) + 1));
			}
			else if (packet_start_idx < packet_end_idx)
			{
				memcpy(gps_packet_buff, (uart_receive_buff + packet_start_idx), (GPS_DMA_RECEIVE_SIZE - packet_start_idx));
				memcpy((gps_packet_buff + (GPS_DMA_RECEIVE_SIZE - packet_start_idx)), uart_receive_buff, (packet_end_idx + 1));
			}
			new_packet_flag = 1;
			packet_complete_flag = 1;
		}
	}
	prev_ndt = curr_ndt;
	new_data_start_idx = next_data_start_idx;
}


void check_gps_packet()
{
	int i;
	uint8_t CK_A, CK_B;

	CK_A = 0;
	CK_B = 0;

	for (i = 2; i < NAV_POSLLH_SIZE - 2; i++)
	{
		CK_A = CK_A + gps_packet_buff[i];
		CK_B = CK_B + CK_A;
	}

	if (CK_A != gps_packet_buff[NAV_POSLLH_SIZE - 2] || CK_B != gps_packet_buff[NAV_POSLLH_SIZE - 1])
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
	int   buff_idx = 4;

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