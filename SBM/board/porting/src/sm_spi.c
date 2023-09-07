/*
 * spi.c
 *
 *  Created on: Jul 10, 2023
 *      Author: Admin
 */

/*include mcu platform*/

/*end*/

#include "sm_bsp_spi.h"
#include "stm32g0xx_hal.h"

#define _impl(x) ((SPI_HandleTypeDef*)(x))

static    int32_t spi_read(sm_bsp_spi_t *_this,uint8_t *buff,uint16_t len);
static    int32_t spi_write(sm_bsp_spi_t *_this,uint8_t *buff,uint16_t len);
static    int32_t spi_write_read(sm_bsp_spi_t *_this,uint8_t *src,uint8_t *dest,uint16_t len);
static    int32_t spi_open(sm_bsp_spi_t *_this);
static    int32_t spi_close(sm_bsp_spi_t *_this);


sm_bsp_spi_proc_t spi_func = {.close = spi_close,.open = spi_open,.write = spi_write,.read = spi_read,.write_read = spi_write_read};

static    int32_t spi_read(sm_bsp_spi_t *_this,uint8_t *buff,uint16_t len){
    /*User code*/
    return HAL_SPI_Receive(_impl(_this->handle),buff,len,1000);;
}
static    int32_t spi_write(sm_bsp_spi_t *_this,uint8_t *buff,uint16_t len){
    /*User code*/
    return HAL_SPI_Transmit(_impl(_this->handle),buff,len,1000);
}
static    int32_t spi_write_read(sm_bsp_spi_t *_this,uint8_t *src,uint8_t *dest,uint16_t len){
    /*User code*/
    return HAL_SPI_TransmitReceive(_impl(_this->handle),src,dest,len,1000);
}
static    int32_t spi_open(sm_bsp_spi_t *_this){
    /*User code*/
    return 0;
}
static    int32_t spi_close(sm_bsp_spi_t *_this){
    /*User code*/
    return 0;
}
