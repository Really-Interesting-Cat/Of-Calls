#ifndef __MAIN__
#define __MAIN__

struct gyro_data
{
  int8_t roll;
  int8_t pitch;
};

struct heart_data
{
  uint8_t user;
  uint8_t beat;
};

struct gps_data
{
  signed long longitude;
  signed long latitude;
};

struct all_data
{
	struct gyro_data gyro;
	struct heart_data heart;
  struct gps_data gps;
};

#endif
