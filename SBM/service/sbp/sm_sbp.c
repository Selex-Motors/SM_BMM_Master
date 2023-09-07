/*
 * sm_sbm.c
 *
 *  Created on: Aug 10, 2023
 *      Author: Admin
 */

#include <sm_sbp.h>
#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "stdio.h"
#include <sm_logger.h>

//const char *serial = "101BE1N000000";

static const char *TAG = "SBP_SERVICE";

extern char serial[13];

#define FAULT_RECOVER_TIMEOUT_mS 7000
#define MAX_TEMP_NUM 6
typedef struct SBP_private
{
    SBP_t base;

    struct
    {
        char *serial;
        uint32_t sw_version;
        uint32_t hw_version;
    } info;

	float soc;
	float soh;
	uint32_t cycles;
	uint32_t voltage;
	int32_t current;
	int32_t temp[MAX_TEMP_NUM];

	uint32_t status;
	uint32_t fet_status;
    SBP_state state;
    uint32_t LD_vol;
	uint32_t time_balancing;
	uint32_t time_inrrush_limit;
	uint32_t time_fault_recover;
    uint32_t time_shutdown;
} SBP_private_t;

SBP_private_t *p_sbp;

#define _impl(x)  ((SBP_private_t*)(x))

SBP_t* sm_sbp_new()
{
    SBP_private_t *_this = malloc (sizeof(SBP_private_t));
	assert(_this != NULL);
    _this->time_balancing = 0;
    _this->time_inrrush_limit = 0;
    _this->time_fault_recover = 0;
    _this->soc = 60;
    _this->soh = 100;
    _this->cycles = 10;
    _this->current = 0;
    _this->voltage = 67000;
    _this->time_shutdown = 0;
    _this->state = SBP_ST_IDLE;
    for (uint16_t i = 0; i < MAX_TEMP_NUM; i++)
    {
        _this->temp[i] = 30;
    }
    _this->info.serial = serial;
//    _this->base.serial_number = serial;
    p_sbp = _this;
	return &_this->base;
}
void sm_sbp_process(SBP_t *_this)
{
    SBP_state state = sm_sbp_get_state (_this);
    SBP_private_t *parents = (SBP_private_t*) _this;
	switch (state) {
        case SBP_ST_IDLE:
            if (sm_sbp_key_hold_on (_this))
            {
                if (sm_sbp_id_selected (_this) != true)
                {
                    sm_sbp_set_state (_this, SBP_ST_SOFTSTART);
				}
				else{
                    sm_sbp_set_state (_this, SBP_ST_ASSIGN);
				}
			}
            if (sm_sbp_id_selected (_this) == true)
            {
                sm_sbp_set_state (_this, SBP_ST_ASSIGN);
			}
            if (parents->time_balancing <= _this->_config->max_time_start_balancing)
                parents->time_balancing++;

            if (parents->time_balancing == _this->_config->max_time_start_balancing)
            {
                sm_sbp_start_balancing (_this);
			}
            parents->time_shutdown++;
            if (parents->time_shutdown > _this->_config->max_time_shutdown)
            {
                sm_sbp_set_state (_this, SBP_ST_SHUTDOWN);
            }
			break;
        case SBP_ST_FAULT:
			parents->time_fault_recover++;
			if(parents->time_fault_recover > FAULT_RECOVER_TIMEOUT_mS){
                sm_sbp_set_state (_this, SBP_ST_SHUTDOWN);
			}
			break;
        case SBP_ST_STANDBY:
//            printf ("Standby mode\n");
            if ((sm_sbp_id_selected (_this) != true))
            {
                sm_sbp_set_state (_this, SBP_ST_IDLE);
            }
            if ((sm_sbp_key_hold_on (_this) == true))
            {
                sm_sbp_set_state (_this, SBP_ST_IDLE);
            }
            if (parents->time_balancing <= _this->_config->max_time_start_balancing)
                parents->time_balancing++;
            if (parents->time_balancing == _this->_config->max_time_start_balancing)
            {
                sm_sbp_start_balancing (_this);
			}
			break;
        case SBP_ST_CHARGING:
        case SBP_ST_DISCHARGING:
            if ((sm_sbp_id_selected (_this) != true) || (sm_sbp_key_hold_on (_this) == true))
            {
                sm_sbp_set_state (_this, SBP_ST_IDLE);
			}
			break;
        case SBP_ST_SOFTSTART:
			parents->time_inrrush_limit++;
            if (parents->time_inrrush_limit > _this->_config->max_inrush_limit_time)
            {
				parents->time_inrrush_limit = 0;
                sm_sbp_set_state (_this, SBP_ST_SYSTEM_BOOST_UP);
			}
			break;
        case SBP_ST_SYSTEM_BOOST_UP:
            if ((sm_sbp_key_hold_on (_this) == true))
            {
                sm_sbp_set_state (_this, SBP_ST_IDLE);
			}

            if (sm_assign_get_state (_this->assign_sv) == ASSIGN_ST_SUCCESS)
            {
                sm_sbp_set_state (_this, SBP_ST_STANDBY);
			}
            if (sm_sbp_get_current (_this) < -100)
            {
                sm_sbp_set_state (_this, SBP_ST_IDLE);
            }
			break;
        case SBP_ST_SHUTDOWN:
            LOG_INF(TAG, "SBP Shutdown\n");
            sm_sbp_shutdown (_this);
			break;
        case SBP_ST_ASSIGN:
            if (_this->assign_sv != NULL)
            {
                sm_assign_process (_this->assign_sv);
                if (sm_assign_get_state (_this->assign_sv) == ASSIGN_ST_SUCCESS)
                {
                    state = SBP_ST_STANDBY;
                    _this->assign_sv->_state = ASSIGN_ST_START;
                    sm_sbp_set_state (_this, SBP_ST_STANDBY);
				}
                else if (sm_assign_get_state (_this->assign_sv) == ASSIGN_ST_FALSE)
                {
                    sm_sbp_set_state (_this, SBP_ST_IDLE);
				}
			}
			break;
		default:
			break;
	}
}
SBP_state sm_sbp_get_state(SBP_t *_this)
{
    SBP_private_t *parent = (SBP_private_t*) _this;
	return parent->state;
}
void sm_sbp_set_state(SBP_t *_this, SBP_state _state)
{
    SBP_private_t *parent = (SBP_private_t*) _this;
    parent->time_balancing = 0;
    parent->time_inrrush_limit = 0;
    parent->time_fault_recover = 0;
    parent->time_shutdown = 0;
	switch (_state) {
        case SBP_ST_IDLE:
            CO_set_node_id (_this->_co_sv, 4);
            sm_sbp_turn_off (_this);
			break;
        case SBP_ST_FAULT:
            sm_sbp_turn_off (_this);
            sm_sbp_stop_balancing (_this);
			parent->time_fault_recover = 0;
			break;
        case SBP_ST_STANDBY:
            sm_sbp_turn_off (_this);
            sm_assign_set_state (_this->assign_sv, ASSIGN_ST_SUCCESS);
			break;
        case SBP_ST_CHARGING:
            sm_sbp_on_charge (_this);
			break;
        case SBP_ST_DISCHARGING:
            sm_sbp_on_discharge (_this);
			break;
        case SBP_ST_SOFTSTART:
            sm_sbp_turn_on (_this);
//            sm_sbp_on_discharge (_this);
			break;
        case SBP_ST_SYSTEM_BOOST_UP:
            sm_sbp_stop_balancing (_this);
			break;
        case SBP_ST_SHUTDOWN:
            sm_sbp_turn_off (_this);
			break;
        case SBP_ST_ASSIGN:
            sm_assign_set_state (_this->assign_sv, ASSIGN_ST_START);
			break;
		default:
			break;
	}
	parent->state = _state;
}
void sm_sbp_set_if(SBP_t *_this, struct sbp_interface *_if)
{
	_this->_if = _if;
}
uint32_t sm_sbp_get_hw_version(SBP_t *_this)
{
    SBP_private_t *parent = (SBP_private_t*) _this;

	return parent->info.hw_version;
}
uint32_t sm_sbp_get_sw_version(SBP_t *_this)
{
    SBP_private_t *parent = (SBP_private_t*) _this;

	return parent->info.sw_version;
}
char* sm_sbp_get_serial_number(SBP_t *_this)
{
    SBP_private_t *parent = (SBP_private_t*) _this;

	return parent->info.serial;
}
void sm_sbp_set_serial_number(SBP_t *_this, char *_serial)
{
    SBP_private_t *parent = (SBP_private_t*) _this;
	strcpy(parent->info.serial,_serial);
}
uint32_t sm_sbp_get_voltage(SBP_t *_this)
{
    SBP_private_t *parent = (SBP_private_t*) _this;
	return parent->voltage;
}
int32_t sm_sbp_get_current(SBP_t *_this)
{
    SBP_private_t *parent = (SBP_private_t*) _this;
	return parent->current;
}
float sm_sbp_get_soc(SBP_t *_this)
{
    SBP_private_t *parent = (SBP_private_t*) _this;
	return parent->soc;
}
void sm_sbp_set_soc(SBP_t *_this, float _soc)
{
    SBP_private_t *parent = (SBP_private_t*) _this;
	parent->soc = _soc;
}
float sm_sbp_get_soh(SBP_t *_this)
{
    SBP_private_t *parent = (SBP_private_t*) _this;
	return parent->soh;
}
void sm_sbp_set_soh(SBP_t *_this, float _soh)
{
    SBP_private_t *parent = (SBP_private_t*) _this;
	parent->soh = _soh;
}
uint32_t sm_sbp_get_cycles(SBP_t *_this)
{
    SBP_private_t *parent = (SBP_private_t*) _this;
	return parent->cycles;
}
void sm_sbp_set_cycles(SBP_t *_this, uint32_t cycles)
{
    SBP_private_t *parent = (SBP_private_t*) _this;
	parent->cycles = cycles;
}
int32_t sm_sbp_get_temp(SBP_t *_this, uint16_t _index)
{
	if(_index > MAX_TEMP_NUM) return 0;
    SBP_private_t *parent = (SBP_private_t*) _this;
	return parent->temp[_index];
}
void sm_sbp_set_temp(SBP_t *_this, int32_t _temp, uint16_t _index)
{
	if(_index > MAX_TEMP_NUM) return;
    SBP_private_t *parent = (SBP_private_t*) _this;
	parent->temp[_index] = _temp;
}

void sm_sbp_set_voltage(SBP_t *_this, uint32_t _vol)
{
    SBP_private_t *parent = (SBP_private_t*) _this;
	parent->voltage = _vol;
}
void sm_sbp_set_current(SBP_t *_this, int32_t _cur)
{
    SBP_private_t *parent = (SBP_private_t*) _this;
	parent->current = _cur;
}
void sm_sbp_set_bat_status(SBP_t *_this, uint32_t status)
{
    _impl(_this)->status = status;
}
uint32_t sm_sbp_get_bat_status(SBP_t *_this)
{
    return _impl(_this)->status;
}

void sm_sbp_set_fet_status(SBP_t *_this, uint32_t fet_status)
{
    _impl(_this)->fet_status = fet_status;
}
uint32_t sm_sbp_get_fet_status(SBP_t *_this)
{
    return _impl(_this)->fet_status;
}
void sm_sbp_set_LD_vol(SBP_t *_this, uint32_t LD_vol)
{
    _impl(_this)->LD_vol = LD_vol;
}
uint32_t sm_sbp_get_LD_vol(SBP_t *_this)
{
    return _impl(_this)->LD_vol;
}
