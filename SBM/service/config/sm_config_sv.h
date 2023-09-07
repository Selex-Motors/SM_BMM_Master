/*
 * config_sv.h
 *
 *  Created on: Aug 24, 2023
 *      Author: Admin
 */

#ifndef SERVICE_CONFIG_SM_CONFIG_SV_H_
#define SERVICE_CONFIG_SM_CONFIG_SV_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <sm_fileconfig.h>


#define file_config "config.json"

typedef struct sm_config_sv sm_config_sv_t;

struct sm_config_sv
{
    struct sm_config_func
    {
        int32_t (*read)(sm_config_sv_t *_this, void *arg);
        int32_t (*write)(sm_config_sv_t *_this, void *arg);
    } _if;
};

int32_t sm_config_init();

static inline int32_t sm_config_read(sm_config_sv_t *_this, void *arg)
{
    return _this->_if.read (_this, arg);
}
static inline int32_t sm_config_write(sm_config_sv_t *_this, void *arg)
{
    return _this->_if.write (_this, arg);
}

#endif /* SERVICE_CONFIG_SM_CONFIG_SV_H_ */
