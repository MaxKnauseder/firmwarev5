#include "crc.h"

void crc16(unsigned char serial_data)
{
    crc = (unsigned char)(crc >> 8) | (crc << 8);
    crc ^= serial_data;
    crc ^= (unsigned char)(crc & 0xff) >> 4;
    crc ^= (crc << 8) << 4;
    crc ^= ((crc & 0xff) << 4) << 1;
}

void crc_start()
{
    crc = 0xffff;
}
