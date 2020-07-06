/**
 * @file 	filesystem.c
 *
 * @brief 	Library to manage the filesystem
 *
 * @date 	July, 2020
 *
 * @author 	James Zhang (jameszha@andrew.cmu.edu)
 */

#include "filesystem.h"
#include <fs/fs.h>
#include <fs/littlefs.h>
#include <storage/flash_map.h>

FS_LITTLEFS_DECLARE_DEFAULT_CONFIG(storage);

/** @brief  Filesystem Configuration */
static struct fs_mount_t lfs_storage_mnt = {
	.type = FS_LITTLEFS,
	.fs_data = &storage,
	.storage_dev = (void *)FLASH_AREA_ID(storage),
	.mnt_point = MNT_POINT,
};

/**
 * @brief Initializes LittleFS filesystem
 */
int filesystem_init(void)
{
	struct fs_mount_t *mp = &lfs_storage_mnt;
	unsigned int id = (uintptr_t)mp->storage_dev;
	struct fs_statvfs sbuf;
	const struct flash_area *pfa;
	int rc;

	// Check if the flash area can be found and opened
	rc = flash_area_open(id, &pfa);
	if (rc < 0) {
		printk("FAIL: unable to find flash area %u: %d\n",
		       id, rc);
		return -1;
	}

	// Optional wipe flash contents
	if (IS_ENABLED(CONFIG_APP_WIPE_STORAGE)) {
		printk("Erasing flash area ... ");
		rc = flash_area_erase(pfa, 0, pfa->fa_size);
		printk("%d\n", rc);
	}

	flash_area_close(pfa);

	// Mount the filesystem
	rc = fs_mount(mp);
	if (rc < 0) {
		printk("FAIL: mount id %u at %s: %d\n",
		       (unsigned int)mp->storage_dev, mp->mnt_point,
		       rc);
		return -1;
	}

	// Check block size, num blocks, and num free blocks
	rc = fs_statvfs(mp->mnt_point, &sbuf);
	if (rc < 0) {
		printk("FAIL: statvfs: %d\n", rc);
		rc = fs_unmount(mp);
		printk("%s unmount: %d\n", mp->mnt_point, rc);
		return -1;
	}

	printk("%s: bsize = %lu ; frsize = %lu ; blocks = %lu ; bfree = %lu\n",
	       mp->mnt_point,
	       sbuf.f_bsize, sbuf.f_frsize,
	       sbuf.f_blocks, sbuf.f_bfree);

	return 0;
}

/**
 * @brief Takes down LittleFS filesystem
 */
int filesystem_destroy(void)
{
	struct fs_mount_t *mp = &lfs_storage_mnt;
	int rc = fs_unmount(mp);
}