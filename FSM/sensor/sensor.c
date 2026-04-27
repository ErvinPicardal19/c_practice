#include <stddef.h>

/* Simulate Sensor */
static unsigned int distance = 20; // Obstacle 20m away

unsigned int readSensor(void)
{
	return distance;
}

void decDistance(unsigned int dec)
{
	distance -= dec;
}
