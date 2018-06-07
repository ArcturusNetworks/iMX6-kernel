/*
 * Arcturus Voice PCM (vp) kernel driver IOCTLs calls
 *
 * Copyright (c) 2008-2016 by Arcturus Networks Inc.
 *		by Oleksandr G Zhadan
 *
 * All rights reserved.
 *
 * This material is proprietary to Arcturus Networks Inc. and, in
 * addition to the above mentioned Copyright, may be subject to
 * protection under other intellectual property regimes, including
 * patents, trade secrets, designs and/or trademarks.
 *
 * Any use of this material for any purpose, except with an express
 * license from Arcturus Networks Inc. is strictly prohibited.
 *
 */
#ifndef __VP_IOCTL_H__
#define __VP_IOCTL_H__

#define VP_IOC_MAGIC  	'Z'
#define VP_IOC_MAXNR  	34
#define VP_IOC_TXBUFF	1

#define VP_IOCEXIT	_IO(VP_IOC_MAGIC, 0)	/* shutdown pcm device */
#define VP_IOCRESET	_IO(VP_IOC_MAGIC, 1)	/* reset pcm */

#define VP_IOCGCODEC	_IOR(VP_IOC_MAGIC, 2, int)	/* get curent codec */
#define VP_IOCSCODEC	_IOW(VP_IOC_MAGIC, 3, int)	/* set curent codec */
#define VP_IOCGCODEC_EF	_IOR(VP_IOC_MAGIC, 4, int)	/* get codec encode entry */
#define VP_IOCGCODEC_DF	_IOR(VP_IOC_MAGIC, 5, int)	/* get codec decode entry */
#define VP_IOCSCODEC_EF	_IOR(VP_IOC_MAGIC, 6, int)	/* set codec encode entry */
#define VP_IOCSCODEC_DF	_IOR(VP_IOC_MAGIC, 7, int)	/* set codec decode entry */

#define VP_IOCGAEC	_IOR(VP_IOC_MAGIC, 8, int)	/* get curent AEC state */
#define VP_IOCSAEC	_IOW(VP_IOC_MAGIC, 9, int)	/* set curent AEC state */
#define VP_IOCGAECF	_IOR(VP_IOC_MAGIC, 10, int)	/* get AEC function entry */
#define VP_IOCSAECF	_IOW(VP_IOC_MAGIC, 11, int)	/* set AEC function entry */

#define VP_IOCGNUMTCD	_IOR(VP_IOC_MAGIC, 12, int)	/* get num_tcd */
#define VP_IOCSNUMTCD	_IOW(VP_IOC_MAGIC, 13, int)	/* set num_tcd */

#define VP_IOCSPTIME	_IOW(VP_IOC_MAGIC, 14, int)	/* set ptime */
#define VP_IOCGPTIME	_IOR(VP_IOC_MAGIC, 15, int)	/* get ptime */

#define VP_IOCSTONE	_IOW(VP_IOC_MAGIC, 16, int)	/* set local TONE_G state */
#define VP_IOCGTONE	_IOR(VP_IOC_MAGIC, 17, int)	/* get local TONE_G state */
#define VP_IOCSTONEF	_IOW(VP_IOC_MAGIC, 18, int)	/* set TONE_G function entry */
#define VP_IOCSCALL	_IOW(VP_IOC_MAGIC, 19, int)	/* magic call */
#define VP_IOCGTONEF	_IOR(VP_IOC_MAGIC, 20, int)	/* get TONE_G function entry */
#define VP_IOCSTONEREMOTE _IOW(VP_IOC_MAGIC, 21, int)	/* set remote TONE_G state */
#define VP_IOCGTONEREMOTE _IOR(VP_IOC_MAGIC, 22, int)	/* get remote TONE_G state */

#define VP_IOCSAECOFF 	_IOW(VP_IOC_MAGIC, 23, int)	/* set AEC tx buffer offset */

#define VP_IOCSFREQ 	_IOW(VP_IOC_MAGIC, 24, int)	/* set PCM Freq in KHz (8 or 16) */

#define VP_IOCSCOMPLEX 	_IOW(VP_IOC_MAGIC, 25, unsigned int)	/* set CODEC&FREQ&PTIME */

#define VP_IOCSFRENERGY _IOW(VP_IOC_MAGIC, 26, unsigned int)	/* set frame energy time */
#define VP_IOCGFRENERGY _IOW(VP_IOC_MAGIC, 27, unsigned int)	/* get frame energy */
#define VP_IOCGCOMPLEX 	_IOR(VP_IOC_MAGIC, 28, unsigned int)	/* get CODEC&FREQ&PTIME */

#define VP_IOCGFLAGS	_IOR(VP_IOC_MAGIC, 29, int)	/* get curent flags */
#define VP_IOCSFLAGS	_IOW(VP_IOC_MAGIC, 30, int)	/* set curent flags */

#define VP_IOCSSYNC 	_IO(VP_IOC_MAGIC, 31)		/* sync device read&write indexes with codec DMA indexes */
#define VP_IOCSSYNCALL 	_IO(VP_IOC_MAGIC, 32)		/* sync all vp devices read&write indexes with codec DMA indexes */
#define VP_IOCGSYNC 	_IO(VP_IOC_MAGIC, 33)		/* get sync from codec DMA channel */

typedef struct set_codec_struct {
	unsigned int codec;
	unsigned int pcm2codec;
	unsigned int codec2pcm;
	unsigned short freq;
	unsigned short ptime;
	unsigned int private;
} set_codec_t;

typedef struct {
	void  *ChannelBase;		/* Pointer to the channel data */
	short ToneType;
	short CadenceBuf[4];
	short ToneFreq1;
	short ToneFreq2;
	short ToneAMFreq;
	short ToneLevel;
} CPToneGConfig;

#define TONE_LOCAL_BIT_MAP		0x80000000
#define TONE_REMOTE_BIT_MAP		0x40000000
#define AEC_BIT_MAP			0x20000000
#define TONE_LOCAL_BIT_MAP_CUSTOM	0x10000000

#define CN_PAYLOAD		4

#endif /* __VP_IOCTL_H__ */
