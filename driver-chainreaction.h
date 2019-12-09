/*
 * Copyright 2014 Con Kolivas <kernel@kolivas.org>
 * Copyright 2014 Zvi Shteingart - Chainreactionoolies-tech.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  See COPYING for more details.
 */

#ifndef CHAINREACTIONA_HFILE
#define CHAINREACTIONA_HFILE

#include "miner.h"
#include "driver-chainreaction-p.h"



typedef enum adapter_state {
	ADAPTER_STATE_INIT,
	ADAPTER_STATE_OPERATIONAL,
} ADAPTER_STATE;

typedef enum chainreaction_work_state {
	CHAINREACTIONWORK_STATE_EMPTY,
	CHAINREACTIONWORK_STATE_IN_BUSY,
} CHAINREACTIONWORK_STATE;

#define MAX_JOBS_PENDING_IN_REACTIONGATE 30
#define MAX_NROLES 60 


typedef struct {
	struct work      *cgminer_work;
	CHAINREACTIONWORK_STATE  state;
	uint32_t         merkle_root;
	time_t           start_time;
	int              job_id;
} chainreaction_driver_work;



struct react_adapter {
	pthread_mutex_t lock;
	ADAPTER_STATE adapter_state;
	void *cgpu;

	// Statistics
	int wins;
	int good;
	int empty;
	int bad;
	int overflow;
	// state
	int works_in_driver;
	int works_in_reactiongate_and_pending_tx;
	int works_pending_tx;
	int socket_fd;
	int reset_mg_queue;  // 3=reset, 2=fast send 1 job, 1=fast send 10 jobs, 0=nada
	int current_job_id;
	int parse_resp;
	reactiongate_req_packet* mp_next_req;
	reactiongate_rsp_packet* mp_last_rsp;
	chainreaction_driver_work my_jobs[MAX_JOBS_PENDING_IN_REACTIONGATE];

	// Temperature statistics
	int temp_rate;
	int front_temp;
	int rear_temp_top;
	int rear_temp_bot;

	// Last second we polled stats
	time_t last_stats;
};

// returns non-zero if needs to change ASICs.
int chainreaction_one_sec_timer_scaling(struct react_adapter *a, int t);
int chainreaction_do_scaling(struct react_adapter *a);

extern void one_sec_chainreactionoolies_watchdog(int uptime);

#define REQUEST_PERIOD (100000)  //  times per second - in usec
#define REQUEST_SIZE   10      //  jobs per request

#endif
