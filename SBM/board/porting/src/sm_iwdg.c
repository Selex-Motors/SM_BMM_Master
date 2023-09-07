#include "sm_bsp_iwdg.h"
#include "stm32g0xx_hal.h"

#define _impl(x) ((IWDG_HandleTypeDef*)(x))

static int32_t iwdg_open(sm_bsp_iwdg_t *_this);
static int32_t iwdg_close(sm_bsp_iwdg_t *_this);
static int32_t iwdg_reset(sm_bsp_iwdg_t *_this);

sm_bsp_iwdg_proc_t iwdg_fun = {.open = iwdg_open,.close = iwdg_close,.reset = iwdg_reset};


static int32_t iwdg_open(sm_bsp_iwdg_t *_this)
{
    return HAL_IWDG_Init(_impl(_this->handle));
}
static int32_t iwdg_close(sm_bsp_iwdg_t *_this)
{
    return -1;
}
static int32_t iwdg_reset(sm_bsp_iwdg_t *_this)
{
    return HAL_IWDG_Refresh(_impl(_this->handle));
}
