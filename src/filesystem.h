/**
 * @file 	filesystem.h
 *
 * @brief 	Library to manage the filesystem
 *
 * @date 	July, 2020
 *
 * @author 	James Zhang (jameszha@andrew.cmu.edu)
 */

#ifndef _FS_H_
#define _FS_H_

#include <drivers/uart.h>

/* Matches LFS_NAME_MAX */
#define FILESYSTEM_MAX_PATH_LEN 255

#define MNT_POINT "/lfs"

int filesystem_init(void);

int filesystem_destroy(void);

#endif /* _FS_H_ */