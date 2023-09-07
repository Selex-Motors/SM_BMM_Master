/*
 * sm_sbm.h
 *
 *  Created on: Aug 10, 2023
 *      Author: Admin
 */

#ifndef APPS_SM_SBP_H_
#define APPS_SM_SBP_H_

#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
//#include "bq769x2.h"
#include "CO.h"
#include "sm_assign.h"

typedef enum {
    SBP_ST_INIT = 0,
    SBP_ST_IDLE,
    SBP_ST_SOFTSTART,
    SBP_ST_DISCHARGING,
    SBP_ST_CHARGING,
    SBP_ST_FAULT,
    SBP_ST_SYSTEM_BOOST_UP,
    SBP_ST_STANDBY,
    SBP_ST_SHUTDOWN,
    SBP_ST_ASSIGN
} SBP_state;

typedef struct SBP SBP_t;

struct SBP
{
    struct sbp_interface
    {
		int32_t (*on_charge)();
		int32_t (*on_discharge)();
		int32_t (*off_charge)();
		int32_t (*off_discharge)();
		int32_t (*turn_on)();
		int32_t (*turn_off)();
		int32_t (*shutdown)();
		int32_t (*start_balancing)();
		int32_t (*stop_balancing)();
		bool    (*node_id_selected)();
		bool 	(*key_hold_on)();

        // update
        void (*update)(SBP_t *_this, uint32_t time_driff_mS);
	} *_if;

    struct sbp_config
    {
		uint32_t max_inrush_limit_time;
		uint32_t max_time_start_balancing;
		uint32_t max_time_relax;
        uint32_t max_time_shutdown;
    } *_config;

    
//    BQ769x2 *_afe_sv;
	CO *_co_sv;
    SM_ASSIGN_t *assign_sv;
};

SBP_t* sm_sbp_new();

void sm_sbp_set_if(SBP_t *_this, struct sbp_interface *_if);
SBP_state sm_sbp_get_state(SBP_t *_this);
void sm_sbp_set_state(SBP_t *_this, SBP_state _state);
uint32_t sm_sbp_get_hw_version(SBP_t *_this);
uint32_t sm_sbp_get_sw_version(SBP_t *_this);
char* sm_sbp_get_serial_number(SBP_t *_this);
void sm_sbp_set_serial_number(SBP_t *_this, char *_serial);

float sm_sbp_get_soc(SBP_t *_this);
void sm_sbp_set_soc(SBP_t *_this, float _soc);
float sm_sbp_get_soh(SBP_t *_this);
void sm_sbp_set_soh(SBP_t *_this, float _soh);
uint32_t sm_sbp_get_cycles(SBP_t *_this);
void sm_sbp_set_cycles(SBP_t *_this, uint32_t cycles);
int32_t sm_sbp_get_temp(SBP_t *_this, uint16_t _index);
void sm_sbp_set_temp(SBP_t *_this, int32_t _temp, uint16_t _index);

void sm_sbp_set_voltage(SBP_t *_this, uint32_t _vol);
void sm_sbp_set_current(SBP_t *_this, int32_t _cur);

uint32_t sm_sbp_get_voltage(SBP_t *_this);
int32_t sm_sbp_get_current(SBP_t *_this);

void sm_sbp_set_bat_status(SBP_t *_this, uint32_t status);
uint32_t sm_sbp_get_bat_status(SBP_t *_this);


void sm_sbp_set_fet_status(SBP_t *_this, uint32_t fet_status);
uint32_t sm_sbp_get_fet_status(SBP_t *_this);

void sm_sbp_set_LD_vol(SBP_t *_this, uint32_t LD_vol);
uint32_t sm_sbp_get_LD_vol(SBP_t *_this);

void sm_sbp_process(SBP_t *_this);
static inline void sm_sbp_update(SBP_t *_this, uint32_t time_driff_mS)
{
    _this->_if->update (_this, time_driff_mS);
}


static inline int32_t sm_sbp_on_charge(SBP_t *_this)
{
	if(_this->_if->on_charge == NULL) return -1;
	return _this->_if->on_charge();
}
static inline int32_t sm_sbp_off_charge(SBP_t *_this)
{
	if(_this->_if->off_charge == NULL) return -1;

	return _this->_if->off_charge();
}
static inline int32_t sm_sbp_on_discharge(SBP_t *_this)
{
	if(_this->_if->on_discharge == NULL) return -1;
	return _this->_if->on_discharge();
}
static inline int32_t sm_sbp_off_discharge(SBP_t *_this)
{
	if(_this->_if->off_discharge == NULL) return -1;
	return _this->_if->off_discharge();
}
static inline int32_t sm_sbp_turn_on(SBP_t *_this)
{
	if(_this->_if->turn_on == NULL) return -1;
	return _this->_if->turn_on();
}
static inline int32_t sm_sbp_turn_off(SBP_t *_this)
{
	if(_this->_if->turn_off == NULL) return -1;
	return _this->_if->turn_off();
}
static inline int32_t sm_sbp_start_balancing(SBP_t *_this)
{
	if(_this->_if->start_balancing == NULL) return -1;
	return _this->_if->start_balancing();
}
static inline int32_t sm_sbp_stop_balancing(SBP_t *_this)
{
	if(_this->_if->stop_balancing == NULL) return -1;
	return _this->_if->stop_balancing();
}
static inline int32_t sm_sbp_shutdown(SBP_t *_this)
{
	if(_this->_if->shutdown == NULL) return -1;
	return _this->_if->shutdown();
}
static inline bool sm_sbp_id_selected(SBP_t *_this)
{
	if(_this->_if->node_id_selected == NULL) return false;
	return _this->_if->node_id_selected();
}

static inline bool sm_sbp_key_hold_on(SBP_t *_this)
{
	if(_this->_if->key_hold_on == NULL) return false;
	return _this->_if->key_hold_on();
}
#endif /* APPS_SM_SBP_H_ */
