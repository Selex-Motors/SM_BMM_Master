/*
 * sm_assign.h
 *
 *  Created on: Aug 10, 2023
 *      Author: Admin
 */

#ifndef SERVICES_ASSIGN_SM_ASSIGN_H_
#define SERVICES_ASSIGN_SM_ASSIGN_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct SM_ASSIGN SM_ASSIGN_t;

struct SM_ASSIGN{
	enum assign_state{
		ASSIGN_ST_START = 0,
		ASSIGN_ST_WAIT_CONFIRM,
		ASSIGN_ST_CONFIRMED,
		ASSIGN_ST_WAIT_SLAVE_SELECT,
		ASSIGN_ST_START_AUTHENTICATE,
		ASSIGN_ST_AUTHENTICATING,
		ASSIGN_ST_SUCCESS,
		ASSIGN_ST_FALSE
	}_state;

	struct assign_if{
		bool (*node_id_selected)();
		void (*send_cob_id)(uint16_t _cod_id);
		bool (*recv_confirm)();
		bool (*authen_success)();
	} *_if;
	struct assign_config{
		uint32_t assign_timeout_ms;
		uint16_t cob_id;

	} *_config;

	uint32_t _assign_time_ms;
};

SM_ASSIGN_t *sm_assign_new();
void sm_assign_init(SM_ASSIGN_t *_this,struct assign_if *_if, struct assign_config *_config);
void sm_assign_process(SM_ASSIGN_t *_this);
void sm_assign_set_state(SM_ASSIGN_t *_this,enum assign_state _state);
static inline enum assign_state sm_assign_get_state(SM_ASSIGN_t *_this){
	return _this->_state;
}

static inline bool sm_assign_id_selected(SM_ASSIGN_t *_this){
	return _this->_if->node_id_selected();
}

static inline void sm_assign_send_cob_id(SM_ASSIGN_t *_this){
	_this->_if->send_cob_id(_this->_config->cob_id);
}

static inline bool sm_assign_recv_confirm(SM_ASSIGN_t *_this){
	return _this->_if->recv_confirm();
}

static inline bool sm_assign_authen_success(SM_ASSIGN_t *_this){
	return _this->_if->authen_success();
}

#endif /* SERVICES_ASSIGN_SM_ASSIGN_H_ */
