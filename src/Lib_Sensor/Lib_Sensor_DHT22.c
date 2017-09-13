/*
 * Lib_Sensor.c
 *
 *  Created on: Mar 11, 2015
 *      Author: cedric
 */

/*******************************************
*			  I N C L U D E 			   *
********************************************/
//#include <stdlib.h>
//#include <string.h>
//
//#include "Lib_Sensor_DHT22.h"
//#include "bbb_mmio.h"
///*******************************************
//*               D E F I N E                *
//********************************************/
//
//// This is the only processor specific magic value, the maximum amount of time to
//// spin in a loop before bailing out and considering the read a timeout. This should
//// be a high value, but if you're running on a much faster platform than a Raspberry
//// Pi or Beaglebone Black then it might need to be increased.
//#define DHT_MAXCOUNT 32000
//
//// Number of bit pulses to expect from the DHT. Note that this is 41 because
//// the first pulse is a constant 50 microsecond pulse, with 40 pulses to represent
//// the data afterwards.
//#define DHT_PULSES 41
//
///*******************************************
//*   T Y P E D E F   &  C O N S T A N T E   *
//********************************************/
//
///*******************************************
//*	 G L O B A L   V A R I A B L E S  	   *
//********************************************/
//
///*******************************************
//*	        F U N C T I O N S   	       *
//********************************************/
///*
// ============================================
// Function     : bbb_dht_read()
// Parameter    :
// Return Value : int
// Description  :
// ============================================
// */
//int bbb_dht_read(int type, int gpio_base, int gpio_number, float* humidity, float* temperature) {
//// Validate humidity and temperature arguments and set them to zero.
//if (humidity == NULL || temperature == NULL) {
//return DHT_ERROR_ARGUMENT;
//}
//*temperature = 0.0f;
//*humidity = 0.0f;
//// Store the count that each DHT bit pulse is low and high.
//// Make sure array is initialized to start at zero.
//int pulseCounts[DHT_PULSES*2] = {0};
//// Get GPIO pin and set it as an output.
//gpio_t pin;
//if (bbb_mmio_get_gpio(gpio_base, gpio_number, &pin) < 0) {
//return DHT_ERROR_GPIO;
//}
//bbb_mmio_set_output(pin);
//// Bump up process priority and change scheduler to try to try to make process more 'real time'.
//set_max_priority();
//// Set pin high for ~500 milliseconds.
//bbb_mmio_set_high(pin);
//sleep_milliseconds(500);
//// The next calls are timing critical and care should be taken
//// to ensure no unnecssary work is done below.
//// Set pin low for ~20 milliseconds.
//bbb_mmio_set_low(pin);
//busy_wait_milliseconds(20);
//
//// Set pin as input.
//bbb_mmio_set_input(pin);
//// Wait for DHT to pull pin low.
//uint32_t count = 0;
//while (bbb_mmio_input(pin)) {
//if (++count >= DHT_MAXCOUNT) {
//// Timeout waiting for response.
//set_default_priority();
//return DHT_ERROR_TIMEOUT;
//}
//}
//// Record pulse widths for the expected result bits.
//int i= 0 ;
//for (i=0; i < DHT_PULSES*2; i+=2) {
//// Count how long pin is low and store in pulseCounts[i]
//while (!bbb_mmio_input(pin)) {
//if (++pulseCounts[i] >= DHT_MAXCOUNT) {
//// Timeout waiting for response.
//set_default_priority();
//return DHT_ERROR_TIMEOUT;
//}
//}
//// Count how long pin is high and store in pulseCounts[i+1]
//while (bbb_mmio_input(pin)) {
//if (++pulseCounts[i+1] >= DHT_MAXCOUNT) {
//// Timeout waiting for response.
//set_default_priority();
//return DHT_ERROR_TIMEOUT;
//}
//}
//}
//// Done with timing critical code, now interpret the results.
//// Drop back to normal priority.
//set_default_priority();
//// Compute the average low pulse width to use as a 50 microsecond reference threshold.
//// Ignore the first two readings because they are a constant 80 microsecond pulse.
//uint32_t threshold = 0;
//for (i=2; i < DHT_PULSES*2; i+=2) {
//threshold += pulseCounts[i];
//}
//threshold /= DHT_PULSES-1;
//// Interpret each high pulse as a 0 or 1 by comparing it to the 50us reference.
//// If the count is less than 50us it must be a ~28us 0 pulse, and if it's higher
//// then it must be a ~70us 1 pulse.
//uint8_t data[5] = {0};
//
//for (i=3; i < DHT_PULSES*2; i+=2) {
//int index = (i-3)/16;
//data[index] <<= 1;
//if (pulseCounts[i] >= threshold) {
//// One bit for long pulse.
//data[index] |= 1;
//}
//// Else zero bit for short pulse.
//}
//// Useful debug info:
////printf("Data: 0x%x 0x%x 0x%x 0x%x 0x%x\n", data[0], data[1], data[2], data[3], data[4]);
//// Verify checksum of received data.
//if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
//if (type == DHT11) {
//// Get humidity and temp for DHT11 sensor.
//*humidity = (float)data[0];
//*temperature = (float)data[2];
//}
//else if (type == DHT22) {
//// Calculate humidity and temp for DHT22 sensor.
//*humidity = (data[0] * 256 + data[1]) / 10.0f;
//*temperature = ((data[2] & 0x7F) * 256 + data[3]) / 10.0f;
//if (data[2] & 0x80) {
//*temperature *= -1.0f;
//}
//}
//return DHT_SUCCESS;
//}
//else {
//return DHT_ERROR_CHECKSUM;
//}
//}
//
//
//void busy_wait_milliseconds(uint32_t millis) {
//// Set delay time period.
//struct timeval deltatime;
//deltatime.tv_sec = millis / 1000;
//deltatime.tv_usec = (millis % 1000) * 1000;
//struct timeval walltime;
//// Get current time and add delay to find end time.
//gettimeofday(&walltime, NULL);
//struct timeval endtime;
//timeradd(&walltime, &deltatime, &endtime);
//// Tight loop to waste time (and CPU) until enough time as elapsed.
//while (timercmp(&walltime, &endtime, <)) {
//gettimeofday(&walltime, NULL);
//}
//}
//void sleep_milliseconds(uint32_t millis) {
//struct timespec sleep;
//sleep.tv_sec = millis / 1000;
//sleep.tv_nsec = (millis % 1000) * 1000000L;
//while (clock_nanosleep(CLOCK_MONOTONIC, 0, &sleep, &sleep) && errno == EINTR);
//}
//void set_max_priority(void) {
//struct sched_param sched;
//memset(&sched, 0, sizeof(sched));
//// Use FIFO scheduler with highest priority for the lowest chance of the kernel context switching.
//sched.sched_priority = sched_get_priority_max(SCHED_FIFO);
//sched_setscheduler(0, SCHED_FIFO, &sched);
//}
//void set_default_priority(void) {
//struct sched_param sched;
//memset(&sched, 0, sizeof(sched));
//// Go back to default scheduler with default 0 priority.
//sched.sched_priority = 0;
//sched_setscheduler(0, SCHED_OTHER, &sched);
//}
//
