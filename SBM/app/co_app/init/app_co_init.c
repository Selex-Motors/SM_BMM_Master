/*
 * app_co_init.c
 *
 *  Created on: Jun 17, 2022
 *      Author: Dakaka
 */

#include "app_co_init.h"
#include "od.h"
#include <stdbool.h>
//#include "sbp_app/sbp_app.h"
#include "sm_bsp_can.h"
//#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
//#include "assign_app.h"
#include "sm_porting.h"
//#include "hal_data.h"
extern sm_bsp_can_t *can;
CO sbp_co;
void can_receive_task(sm_bsp_can_t *_this, sm_bsp_can_mgs_t *_mgs);

void app_co_set_can_baudrate_impl(uint32_t br_khz);
void app_co_flash_error_handle_impl(void);
int8_t app_co_erase_and_write_data_to_flash_impl(uint32_t addr, uint32_t buff, uint32_t len);
/********************************************************************************************
 * 									MANDATORY: Edit carefully !!!
 * *******************************************************************************************/
/* Declare for build TPDO message function when using "tpdo_build_data_manually" mode*/
static void tpdo1_build_data_impl(uint8_t* buffer);
static void tpdo2_build_data_impl(uint8_t* buffer);
static void tpdo3_build_data_impl(uint8_t* buffer);
static void tpdo4_build_data_impl(uint8_t* buffer);
static void tpdo5_build_data_impl(uint8_t *buffer);
static void tpdo6_build_data_impl(uint8_t *buffer);
static void tpdo7_build_data_impl(uint8_t *buffer);
void *tpdo_build_data_impl[TPDO_NUMBER] =
{ tpdo1_build_data_impl,
  tpdo2_build_data_impl,
  tpdo3_build_data_impl,
  tpdo4_build_data_impl,
  tpdo5_build_data_impl,
  tpdo6_build_data_impl,
  tpdo7_build_data_impl, };

static void tpdo1_build_data_impl(uint8_t* buffer)
{
/*    CO_setUint16 (buffer, (uint16_t) (sm_sbp_get_voltage (sm_sbp) / 10));
    CO_setUint16 (buffer + 2, (uint16_t) ((int16_t) (sm_sbp_get_current (sm_sbp))));
    buffer[4] = (uint8_t) sm_sbp_get_soc (sm_sbp);
    buffer[5] = sm_sbp_get_state (sm_sbp);
    CO_setUint16 (buffer + 6, (uint16_t) (0sm_sbp_get_bat_status (sm_sbp)));*/
}

static void tpdo2_build_data_impl(uint8_t* buffer)
{
/*    uint8_t cell_id = 0;
    uint8_t index = 0;
    while (cell_id < 8)
    {
        if (sm_sbp->_afe_sv->cell_array->cells[index].is_short == 0)
        {
            buffer[cell_id] = (uint8_t) (sm_sbp->_afe_sv->cell_array->cells[index].voltage);
            buffer[cell_id + 1] = (uint8_t) ((sm_sbp->_afe_sv->cell_array->cells)[index].voltage >> 8);
            cell_id = cell_id + 2;
        }
        index++;
    }*/
}

static void tpdo3_build_data_impl(uint8_t* buffer)
{
/*    uint8_t cell_id = 0;
    uint8_t index = 0;
    while (cell_id < 8)
    {
        if ((sm_sbp->_afe_sv->cell_array->cells + 4)[index].is_short == 0)
        {
            buffer[cell_id] = (uint8_t) ((sm_sbp->_afe_sv->cell_array->cells + 4)[index].voltage);
            buffer[cell_id + 1] = (uint8_t) ((sm_sbp->_afe_sv->cell_array->cells + 4)[index].voltage >> 8);
            cell_id = cell_id + 2;
        }
        index++;
    }*/
}

static void tpdo4_build_data_impl(uint8_t* buffer)
{
/*    for (uint16_t i = 0; i < 6; i++)
    {
        buffer[i] = (uint8_t) sm_sbp_get_temp (sm_sbp, i);
    }
    buffer[6] = (uint8_t) sm_sbp_get_fet_status (sm_sbp);*/
}
static void tpdo5_build_data_impl(uint8_t *buffer)
{
/*    uint8_t cell_id = 0;
    uint8_t index = 0;
    while (cell_id < 8)
    {
        if ((sm_sbp->_afe_sv->cell_array->cells + 8)[index].is_short == 0)
        {
            buffer[cell_id] = (uint8_t) ((sm_sbp->_afe_sv->cell_array->cells + 8)[index].voltage);
            buffer[cell_id + 1] = (uint8_t) ((sm_sbp->_afe_sv->cell_array->cells + 8)[index].voltage >> 8);
            cell_id = cell_id + 2;
        }
        index++;
    }*/
}
//voltage from cell 13 to cell 16
static void tpdo6_build_data_impl(uint8_t *buffer)
{
/*    uint8_t cell_id = 0;
    uint8_t index = 0;
    while (cell_id < 8)
    {
        if ((sm_sbp->_afe_sv->cell_array->cells + 12)[index].is_short == 0)
        {
            buffer[cell_id] = (uint8_t) ((sm_sbp->_afe_sv->cell_array->cells + 12)[index].voltage);
            buffer[cell_id + 1] = (uint8_t) ((sm_sbp->_afe_sv->cell_array->cells + 12)[index].voltage >> 8);
            cell_id = cell_id + 2;
        }
        index++;
    }*/
}

static void tpdo7_build_data_impl(uint8_t *buffer)
{
/*    CO_setUint16 (buffer, (uint16_t) (sm_sbp_get_cycles (sm_sbp)));
    buffer[2] = (uint8_t) sm_sbp_get_soh (sm_sbp);*/
}

static sm_bsp_can_mgs_t mgs;
/* Define can_send message function */
static void app_co_can_send_impl(CO_CAN_Msg* p_msg)
{
    mgs.id = p_msg->id.cob_id;
    mgs.length = p_msg->data_len;
    memcpy (mgs.data, p_msg->data, 8);
    sm_bsp_can_write (can, &mgs);
}



/* Call in receive can interrupt */
void app_co_can_receive(const uint32_t can_id, uint8_t* data)
{

//    uint32_t cob_id = can_id;
//    if (sm_sbp_get_state (sm_sbp) == SBP_ST_SOFTSTART)
//        return;
    if (CO_can_receive_basic_handle (&CO_DEVICE, can_id, data))
    {
    }
	/* USER CODE BEGIN */
/*    if (cob_id == CAN_NODE_ID_ASSIGN_COBID)
    {
        switch (sm_assign_get_state (&assign_app))
        {
            case ASSIGN_ST_WAIT_CONFIRM:
                assign_app_set_confirm ();
            break;
            case ASSIGN_ST_WAIT_SLAVE_SELECT:
                mgs.id = CAN_NODE_ID_ASSIGN_COBID;
                mgs.data[0] = data[0];
                mgs.length = 1;
                sm_bsp_can_write (can, &mgs);
                CO_set_node_id (&CO_DEVICE, data[0]);
                assign_app_set_confirm ();
            break;
            default:
            break;
        }
    }
    if (cob_id == 0x60)
    {
        mgs.id = 0x061;
        memset (mgs.data, 1, 8);
        mgs.length = 8;
        sm_bsp_can_write (can, &mgs);
    }*/
	/* USER CODE END */
}

void can_receive_task(sm_bsp_can_t *_this, sm_bsp_can_mgs_t *_mgs)
{
    app_co_can_receive (_mgs->id, _mgs->data);
}

/* Define erase_and_write_data_to_flash function
 * Return: success(0), fail(-1)
 */
int8_t app_co_erase_and_write_data_to_flash_impl(uint32_t addr, uint32_t buff, uint32_t len)
{
    return -1;
}

/* Define flash_error_handle function */
void app_co_flash_error_handle_impl(void)
{

}

/* Define set CAN baud-rate function */
void app_co_set_can_baudrate_impl(uint32_t br_khz)
{

}

void app_co_init(void)
{
	/* [Mandatory] Set CO_CAN_send interface */
	CO_CAN_set_can_send_interface(app_co_can_send_impl);

	/* [Mandatory] Init CO object */
#if CO_FLASH__USE
	CO_FLASH_init(&CO_DEVICE.flash,
			app_co_erase_and_write_data_to_flash_impl,
			app_co_flash_error_handle_impl);
#endif
#if APP_OD_STORAGE__USE
#if CO_FLASH__USE & (SW_VERSION != 0)
	CO_OD_STORAGE_init(&CO_DEVICE.storage,
			APP_OD_STORAGE__STORAGE_REGION__ADDR,
			APP_OD_STORAGE__STORAGE_REGION__SIZE,
			&od_temp_comm_profile_para,
			&od_temp_manu_profile_para,
			sizeof(OD_Temp_Manufacturer_Profile_Para_t),
			&CO_DEVICE.flash,
			p_co_od,
			SW_VERSION,
			IS_MAIN_APP);
#else
	while(1); /* Code fail */
#endif
#endif

	CO_init_basic(&CO_DEVICE,
			od_temp_comm_profile_para.x1000_device_type,
			&od_temp_comm_profile_para.x1018_identity,
			p_co_od);

	CO_SYNC_init(&CO_DEVICE.sync, &od_temp_comm_profile_para);

	for(uint8_t i = 0; i < TPDO_NUMBER; i++)
	{
		CO_TPDO_init(&CO_DEVICE.tpdos[i],
				&CO_DEVICE.sync,
				&od_temp_comm_profile_para.x1A0x_tpdo_map_para[i],
				CO_DEVICE.p_od,
				&od_temp_comm_profile_para.x180x_tpdo_comm_para[i],
				tpdo_build_data_impl[i]);
//        &CO_DEVICE.tpdos[i].comm_parameter.build_data_option = CO_TPDO_build_data_map;
	}
	CO_SDOserver_init(&CO_DEVICE.sdo_server,
			&od_temp_comm_profile_para,
			&CO_DEVICE.sync,
			CO_DEVICE.p_od);
	CO_SDOclient_init(&CO_DEVICE.sdo_client,
			&od_temp_comm_profile_para,
			&CO_DEVICE.sync);

	CO_set_node_id(&CO_DEVICE, od_temp_manu_profile_para.x500300_node_ID);
	app_co_set_can_baudrate_impl(od_temp_manu_profile_para.x500301_baud_rate_kHz);


	/* USER CODE BEGIN */
//    can = sm_bsp_can_init (&can_func, (void*) &g_can0);
    sm_bsp_can_open (can);
    sm_bsp_can_set_rx_callback (can, can_receive_task);
	/* USER CODE END */
}


/********************************************************************************************
 * 									USER CODE
 * *******************************************************************************************/

