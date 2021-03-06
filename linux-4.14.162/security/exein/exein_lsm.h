/*
 * exein Linux Security Module
 *
 * Authors: Alessandro Carminati <alessandro@exein.io>,
 *          Gianluigi Spagnuolo <gianluigi@exein.io>,
 *          Alan Vivona <alan@exein.io>
 *
 * Copyright (C) 2020 Exein, SpA.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3, as
 * published by the Free Software Foundation.
 *
 */
#include <linux/types.h>
#include <linux/hashtable.h>
#define EXEIN_REG_DURATION		2500
#define EXEIN_PROT_REGISTRATION_ID	1
#define EXEIN_PROT_KEEPALIVE_ID		2
#define EXEIN_PROT_FEED_ID		3
#define EXEIN_PROT_BLOCK_ID		4
#define EXEIN_PROT_DATA_REQ		5
#define EXEIN_PROT_NEW_PID		6
#define EXEIN_PROT_DEL_PID		7
#define EXEIN_PID_POS			1
#define EXEIN_NN_MAX_SIZE		50
#define EXEIN_RINGBUFFER_SIZE		1<<5 //32 pos
#define EXEIN_FEATURE_NUM_MAX		35

#define EXEIN_ONREQUEST			0x80
#define EXEIN_LIVE			0x81

#ifdef EXEIN_PRINT_DEBUG
#define DODEBUG( ... ) printk( __VA_ARGS__ );
#else
#define DODEBUG( ... ) do { } while(0)
#endif



typedef struct {
        u32		key;
        u8		message_id;
        u8		padding;
        u16		tag;
        pid_t		pid;
} exein_prot_req_t;

typedef struct {
	u16		tag;
	u64		timestamp;
	pid_t		pid;
	uint16_t	processing;
	uint16_t	pending_request;
	u16		seqn;
	struct		hlist_node next;
} exein_reg_data;

void exein_delete_expired_regs(void);

typedef struct {
	u16		features[EXEIN_FEATURE_NUM_MAX];
} exein_pid_data_cell;

typedef struct {
	pid_t			pid;
	u16			tag;
	exein_pid_data_cell	*hookdata[EXEIN_RINGBUFFER_SIZE];
	int			index;
	struct			hlist_node next;
} exein_pid_data;

typedef struct {
	u16		msg_type;
	u32		seed;
	u16		seq;
	pid_t		pid;
	u16		payload[EXEIN_RINGBUFFER_SIZE];
} exein_prot_reply;



