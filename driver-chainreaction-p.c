/*
 * Copyright 2015 Con Kolivas <kernel@kolivas.org>
 * Copyright 2019 Chainreaction 
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  See COPYING for more details.
 */

/*
 This file holds functions needed for reactiongate packet parsing/creation
*/

#include "driver-chainreaction-p.h"
#include "assert.h"

#ifndef passert
#define passert assert
#endif

reactiongate_req_packet *allocate_reactiongate_packet_req(uint8_t requester_id,
                                                        uint8_t request_id) {
  reactiongate_req_packet *p = cgmalloc(sizeof(reactiongate_req_packet));
  p->requester_id = requester_id;
  p->req_count = 0;
  p->protocol_version = REACTIONGATE_PROTOCOL_VERSION;
  p->request_id = request_id;
  p->magic = REACTIONGATE_PROTOCOL_MAGIC_REQ;
  p->mask = 0;
  return p;
}

reactiongate_rsp_packet *allocate_reactiongate_packet_rsp(uint8_t requester_id,
                                                    uint8_t request_id) {
  reactiongate_rsp_packet *p = cgmalloc(sizeof(reactiongate_rsp_packet));
  p->requester_id = requester_id;
  p->rsp_count = 0;
  p->protocol_version = REACTIONGATE_PROTOCOL_VERSION;
  p->request_id = request_id;
  p->magic = REACTIONGATE_PROTOCOL_MAGIC_RSP;
  p->gh_div_50_rate= 0;
  return p;
}
