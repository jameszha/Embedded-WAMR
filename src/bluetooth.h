/**
 * @file 	bluetooth.h
 *
 * @brief 	Library to interact with the HC-05 bluetooth module
 *
 * @date 	July, 2020
 *
 * @author 	James Zhang (jameszha@andrew.cmu.edu)
 */

#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include <drivers/uart.h>

int bluetooth_init(void);

char bluetooth_get_byte(void);

char bluetooth_polling_get_byte(void);

#endif /* _BLUETOOTH_H_ */