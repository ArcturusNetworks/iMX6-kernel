/*
 *   Copyright (c) 2008-2015 Arcturus Networks Inc.
 *                 by Oleksandr Zhadan <www.ArcturusNetworks.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Public License v2 rights and more.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/compiler.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/fcntl.h>
#include <linux/device.h>
#include <linux/major.h>
#include <linux/wait.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/time.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#include "vp_ioctl.h"
#include "vp_typedef.h"

#define MAX_DEV_NUM 1
#define MAX_TCD_NUM 4

#define VP_DRV_NAME		"vp"
#define VP_DRV_VERSION		"v5.0.1547 (c) 2015 Arcturus Networks Inc."

#define VP_NAME			"Voice PCM"
#define VP_MAJOR		120

#define MAX_DEV_NUM	 1

static vp_dev_t vp_dev[MAX_DEV_NUM] __attribute__ ((aligned(32)));
static vp_dev_t vp_dev_saved[MAX_DEV_NUM] __attribute__ ((aligned(32)));

static mcf_tcd_t _mcf_tx_tcd[MAX_TCD_NUM] __attribute__ ((aligned(32)));
static mcf_tcd_t _mcf_rx_tcd[MAX_TCD_NUM] __attribute__ ((aligned(32)));

static frame_t _pcm_tx_buf[MAX_TCD_NUM] __attribute__ ((aligned(32)));
static frame_t _pcm_rx_buf[MAX_TCD_NUM] __attribute__ ((aligned(32)));

static frame_t _pcm_silence __attribute__ ((aligned(32)));
static frame_t _aec_buf __attribute__ ((aligned(32)));

static frame_t _read_buf __attribute__ ((aligned(32)));
static frame_t _write_buf __attribute__ ((aligned(32)));

/******************************************************************************/
/**********************************************   Driver  FOPS   **************/
/******************************************************************************/
static void vp_exit(void);

static int vp_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	vp_dev_t *dev = (vp_dev_t *) file->private_data;
	unsigned int ret = 0;
	unsigned int tmp = 0;
	unsigned int stat = 0;
	unsigned long flags;
	struct timeval ktv;
	static int tv_usec;
	int ptime = 20000;

	pr_debug("....%s: enter (%d)\n", __func__, cmd);

	switch (cmd) {

	case VP_IOCEXIT:
		printk("....%s: VP_IOCEXIT\n", __func__);
		break;

	case VP_IOCGSYNC:
		{
			unsigned int u_wait;
			int tmp_usec;

			do_gettimeofday(&ktv);
			tmp_usec = ktv.tv_usec - tv_usec;
			tv_usec = ktv.tv_usec;
			if (tmp_usec < 0)
				tmp_usec += 1000000;
			if (tmp_usec > ptime)
				break;
			u_wait = ptime - tmp_usec;
			usleep_range(u_wait, u_wait);

			printk("....%s: (%ld usec)\n", __func__, u_wait);

			break;
		}
	case VP_IOCRESET:
		printk("....%s: VP_IOCRESET\n", __func__);
		break;

	case VP_IOCGCODEC:
		printk("....%s: VP_IOCGCODEC\n", __func__);
		break;

	case VP_IOCSCODEC:
		printk("....%s: VP_IOCSCODEC\n", __func__);
		break;

	case VP_IOCGCODEC_EF:
		printk("....%s: VP_IOCGCODEC_EF\n", __func__);
		break;

	case VP_IOCGCODEC_DF:
		printk("....%s: VP_IOCGCODEC_DF\n", __func__);
		break;

	case VP_IOCSCODEC_EF:
		printk("....%s: VP_IOCSCODEC_EF\n", __func__);
		break;

	case VP_IOCSCODEC_DF:
		printk("....%s: VP_IOCSCODEC_DF\n", __func__);
		break;

	case VP_IOCGAEC:
		printk("....%s: VP_IOCGAEC\n", __func__);
		break;

	case VP_IOCSAEC:
		printk("....%s: VP_IOCSAEC\n", __func__);
		break;

	case VP_IOCSCALL:
		printk("....%s: VP_IOCSCALL\n", __func__);
		break;

	case VP_IOCGAECF:
		printk("....%s: VP_IOCGAECF\n", __func__);
		break;

	case VP_IOCSAECF:
		printk("....%s: VP_IOCSAECF\n", __func__);
		break;

	case VP_IOCGTONE:
		printk("....%s: VP_IOCGTONE\n", __func__);
		break;

	case VP_IOCGTONEREMOTE:
		printk("....%s: VP_IOCGTONEREMOTE\n", __func__);
		break;

	case VP_IOCSTONE:
		printk("....%s: VP_IOCSTONE\n", __func__);
		break;

	case VP_IOCSTONEREMOTE:
		printk("....%s: VP_IOCSTONEREMOTE\n", __func__);
		break;

	case VP_IOCGTONEF:
		printk("....%s: VP_IOCGTONEF\n", __func__);
		break;

	case VP_IOCSTONEF:
		printk("....%s: VP_IOCSTONEF\n", __func__);
		break;

	case VP_IOCGNUMTCD:
		printk("....%s: VP_IOCGNUMTCD\n", __func__);
		break;

	case VP_IOCSNUMTCD:
		printk("....%s: VP_IOCSNUMTCD\n", __func__);
		break;

	case VP_IOCGPTIME:
		printk("....%s: VP_IOCGPTIME\n", __func__);
		break;

	case VP_IOCSPTIME:
		printk("....%s: VP_IOCSPTIME\n", __func__);
		break;

	case VP_IOCSAECOFF:
		printk("....%s: VP_IOCSAECOFF\n", __func__);
		break;

	case VP_IOCSFREQ:
		printk("....%s: VP_IOCSFREQ\n", __func__);
		break;

	case VP_IOCSCOMPLEX:
		printk("....%s: VP_IOCSCOMPLEX\n", __func__);
		break;

	case VP_IOCGCOMPLEX:
		printk("....%s: VP_IOCGCOMPLEX\n", __func__);
		break;

	case VP_IOCSFRENERGY:
		printk("....%s: VP_IOCSFRENERGY\n", __func__);
		break;

	case VP_IOCGFRENERGY:
		printk("....%s: VP_IOCGFRENERGY\n", __func__);
		break;

	case VP_IOCGFLAGS:
		printk("....%s: VP_IOCGFLAGS\n", __func__);
		break;

	case VP_IOCSFLAGS:
		printk("....%s: VP_IOCSFLAGS\n", __func__);
		break;

	default:
		pr_err
		    ("%s error: Non exist or obsolete IOCTL call #%d=0x%x, (%0x)\n",
		     __func__, cmd, cmd, (unsigned int)VP_IOCRESET);
		ret = -ENOSYS;
	}

	return ret;
}

static ssize_t
vp_read(struct file *file, char *buf, size_t cnt, loff_t * offset)
{
	vp_dev_t *dev = (vp_dev_t *) file->private_data;

	pr_debug("%s: enter\n", __func__);

	return 0;
}

static ssize_t
vp_write(struct file *file, const char *buf, size_t cnt, loff_t * offset)
{
	vp_dev_t *dev = (vp_dev_t *) file->private_data;

	pr_debug("....%s: enter\n", __func__);

	return 0;
}

static int vp_open(struct inode *inode, struct file *file)
{
	unsigned int minor = MINOR(inode->i_rdev);
	vp_dev_t *dev = (vp_dev_t *) & vp_dev[minor];

	pr_debug("....%s: enter vp%d\n", __func__, minor);

	file->private_data = &vp_dev[minor];

	return 0;
}

static int vp_release(struct inode *inode, struct file *file)
{
	unsigned int minor = MINOR(inode->i_rdev);
	vp_dev_t *dev = (vp_dev_t *) file->private_data;

	pr_debug("....%s: enter vp%d\n", __func__, minor);

	return 0;
}

static struct file_operations vp_fops = {
	.open = vp_open,
	.unlocked_ioctl = vp_ioctl,
	.read = vp_read,
	.write = vp_write,
	.release = vp_release,
};

static int vp_init(void)
{
	return (int)register_chrdev(VP_MAJOR, VP_NAME, &vp_fops);;
}

static void vp_exit(void)
{
	unregister_chrdev(VP_MAJOR, VP_NAME);
	return;
}

module_init(vp_init);
module_exit(vp_exit);

MODULE_DESCRIPTION(" Voice PCM(vp) API driver ");
MODULE_AUTHOR("Oleksandr Zhadan (www.ArcturusNetworks.com)");
MODULE_VERSION(VP_DRV_VERSION);
MODULE_LICENSE("GPL and additional rights");
