/*
 * Copyright 2015 Con Kolivas <kernel@kolivas.org>
 * Copyright 2014 Zvi (Zvisha) Shteingart - Spondoolies-tech.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  See COPYING for more details.
 *
 * Note that changing this SW will void your miners guaranty
 */


#ifndef ____REACTIONGATE_LIB30_H___
#define ____REACTIONGATE_LIB30_H___

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <netinet/in.h>

#include "miner.h"
#include "util.h"

#define REACTIONGATE_PROTOCOL_MAGIC_REQ 0x7723
#define REACTIONGATE_PROTOCOL_MAGIC_RSP 0x2377

#define REACTIONGATE_PROTOCOL_VERSION 1
#define REACTIONGATE_SOCKET_FILE "/tmp/connection_pipe"

typedef enum {
  REACTIONGATE_DATA_ID_DO_JOB_REQ = 1,
  REACTIONGATE_DATA_ID_DO_JOB_RSP = 2,
} REACTIONGATE_DATA_ID;

typedef struct {
  uint32_t work_id_in_sw;
  uint32_t difficulty;
  uint32_t timestamp;
  uint32_t mrkle_root;
  uint32_t midstate[8];
  uint8_t leading_zeroes;
  uint8_t ntime_limit; // max ntime rolling in secs
  uint8_t resr2;
  uint8_t resr1;
} reactiongate_do_job_req;

#define MAX_REQUESTS 30
#define MAX_RESPONDS 60
#define REACTIONGATE_ADAPTER_QUEUE 40

typedef struct {
  uint32_t work_id_in_sw;
  uint32_t mrkle_root; // to validate
  uint32_t winner_nonce;  
  uint8_t  ntime_offset;
  uint8_t res; // 0 = done, 1 = overflow, 2 = dropped bist
  uint8_t job_complete; 
  uint8_t resrv2; 
} reactiongate_do_job_rsp;

typedef struct {
  uint8_t requester_id;
  uint8_t request_id;
  uint8_t protocol_version;
	uint8_t mask; // 0x01 = first request, 0x2 = drop old work
  uint16_t magic;   // 0xcaf4
  uint16_t req_count;
  reactiongate_do_job_req req[MAX_REQUESTS]; // array of requests
} reactiongate_req_packet;

typedef struct {
  uint8_t requester_id;
  uint8_t request_id;
  uint8_t protocol_version;
  uint8_t gh_div_50_rate;
  uint16_t magic;   // 0xcaf4
  uint16_t rsp_count;
  reactiongate_do_job_rsp rsp[MAX_RESPONDS]; // array of responces
} reactiongate_rsp_packet;

reactiongate_req_packet* allocate_reactiongate_packet_req(uint8_t requester_id,uint8_t request_id);
reactiongate_rsp_packet* allocate_reactiongate_packet_rsp(uint8_t requester_id,uint8_t request_id);

#endif
