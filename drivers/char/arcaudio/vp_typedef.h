#ifndef __VP_TYPEDEF_H__
#define __VP_TYPEDEF_H__

#include <linux/wait.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include "vp_ioctl.h"

#define MAX_FRNUM	 3
#define MIN_FRSIZE	160	/* 80 is MIN if 8KHz, 160 if 16KHz */
#define DTMF_NPOINTS 210

typedef struct dtmfd_struct {
	int enable;				/* dtmf is enabled */
	int treshold;				/* above this is dtmf (square of) */
	int size;				/* number of bytes in buffer */
	signed short buffer[DTMF_NPOINTS];	/* buffers one full dtmf frame */
	u8 lastwhat, lastdigit;
	int count;
	u8 digits[16];				/* dtmf result */
} dtmfd_t;

typedef struct frame_energy_struct {
	unsigned int frame_energy;
	unsigned int frame_energy_min;
	unsigned int frame_energy_max;
	unsigned int frame_energy_average;
	unsigned int frame_energy_frames;
} fr_energy_t;

typedef u16 frame_t[MAX_FRNUM * MIN_FRSIZE];

/** DMA TCD Structure **/
typedef struct mcf_tcd {
	volatile u32 saddr;	/* source address */
	volatile u16 attr;	/* transfer attributes */
	volatile u16 soff;	/* signed source address offset */
	volatile u32 nbytes;	/* minor byte count */
	volatile u32 slast;	/* last source address adjustment */
	volatile u32 daddr;	/* destination address */
	volatile u16 citer;	/* current minor loop link, major loop count */
	volatile u16 doff;	/* signed destination address offset */
	volatile u32 dlast_sga;	/* last destination address adjustment */
	volatile u16 biter;	/* beginning minor loop link, major loop count */
	volatile u16 csr;	/* control and status */
} mcf_tcd_t;

/** Driver Private Structure **/
typedef struct vp_pcm_dev {
	u32 Id;
	u32 flags;
	u32 frsize;
	u32 ptime;		/* 1 - 10ms | 2 - 20ms | 3 - 30ms */
	u32 num_tcd;
	u32 timeout;
	u32 buf_size;		/* buffer size in words */
	u32 dma_running;	/* simulator run control */

	u32 tx_event;
	volatile u32 tx_indx;		/* current index for write function */
	u32 tx_dma_count;	/* current count dma HW */
	u32 tx_dma_words;	/* max count dma HW */

	u32 rx_event;
	volatile u32 dma_indx;	/* current index for rx tcd for dma HW */
	volatile u32 dma_indx_prev;	/* prev index for tx tcd for dma HW */
	volatile u32 dma_indx_next;	/* next index for tx tcd for dma HW */
	volatile u32 rx_indx;		/* current index for read function     */
	u32 rx_dma_count;	/* current count dma HW */
	u32 rx_dma_words;	/* max count dma HW */

	volatile u32 rx_dma_indx;	/* current index for rx tcd for dma HW */
	volatile u32 rx_dma_indx_prev;	/* prev index for tx tcd for dma HW */
	volatile u32 rx_dma_indx_next;	/* next index for tx tcd for dma HW */

	volatile u32 tx_dma_indx;	/* current index for rx tcd for dma HW */
	volatile u32 tx_dma_indx_prev;	/* prev index for tx tcd for dma HW */
	volatile u32 tx_dma_indx_next;	/* next index for tx tcd for dma HW */

	wait_queue_head_t read_wait;
	wait_queue_head_t write_wait;

	int first_run;		/* have we been setup yet */

	u32 codec;		/* 0 - PCM, X - codec number */
	int (*codec2pcm) (int Id, u16 * pcm, u8 * codec, int ptime);
	int (*pcm2codec) (int Id, u8 * codec, u16 * pcm, int ptime);

	u32 aec;		/* 0 | 1 : AEC off | on  */
	u32 aec_indx;		/* 0, 1, 2 */
	int (*aec_run) (int Id, u16 * rin, u16 * sin, u16 * sout, int pfrsz);

	u32 conference;		/* 0 | 1 : 3-way conference off | on  */
	void (*mixer) (int Id, u16 * out, u16 * first, u16 * second, int p_frsz);


	u32 tone;		/* 0 - disabled, X - tone number for play */
	u32 tone_remote;	/* 0 - disabled, X - enabled */
	int (*tone2pcm) (int Id, u16 * pcm, int tone, int ptime);
	int toneRampSwitch;

	u32 tone_detection;	/* 0 - disabled, 1 - enabled */
	char (*pcm2tone)(int Id, dtmfd_t *dtmf, u16 *sample, int len, int sfreq);

	u32 write_waiting;
	wait_queue_head_t dma_wait;
	frame_t *wBuf;
	unsigned long last_write;
	unsigned long last_read;

	u32 dma_sync_waiting;
	wait_queue_head_t dma_sync_wait;

	u32 read_waiting;
	frame_t *rBuf;

	spinlock_t tx_lock;
	spinlock_t rx_lock;

	mcf_tcd_t *mcf_tx_tcd;	/* bases of tx tcd (uncached)    */
	mcf_tcd_t *mcf_rx_tcd;	/* bases of rx tcd (uncached)    */

	frame_t *tx_buf;	/* bases of tx buffers (uncached)    */
	frame_t *rx_buf;	/* bases of rx buffers (uncached)    */
	frame_t *silence_buf;	/* base  of silence buffer (uncached)  */
	frame_t *aec_buf;	/* base  of AEC output buffer */

	u32 frameEnergyCnt;
	u32 frameEnergyCntNext;
	u32 frameEnergyCntMax;
	u32 frameEnergySum;
	u32 frameEnergyAverage;
#define FRAME_ENERGY_CNT_MAX	1024
	u32 frameEnergy[FRAME_ENERGY_CNT_MAX + 1];
	set_codec_t scodecdata;

	int sfreq;
	u32 cn_payload;		/* comfort noise payload */

	dtmfd_t dtmfd;
	struct tasklet_struct events_tasklet;
	unsigned int events;
	char dtmf_digit[4];
	wait_queue_head_t event_wait;
	CPToneGConfig tg;
	struct i2c_client *vp_i2c_client[4];
} vp_dev_t;

#endif /* __VP_TYPEDEF_H__ */
