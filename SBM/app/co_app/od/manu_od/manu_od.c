/*
 * app_co_manu_od.c
 *
 *  Created on: Jul 15, 2022
 *      Author: Dakaka
 *
 */

#include "manu_od.h"


/*******************************************************************************************
 * Define all application/manufactor-specific profile initializer sub-object
 * *******************************************************************************************/

/*--- Object 0x5000: Basic electric specifications ----------------------------*/

/*--- Object 0x5001: Basic CANopen node parameters ----------------------------*/
CO_Sub_Object obj_5001_basic_co_node_para[] = {
		{(void*)&od_temp_manu_profile_para.x500300_node_ID, ODA_SDO_RW|ODA_VWTE_after_reset, 1, NULL, (void*)&od_temp_manu_profile_para.x500300_node_ID},
		{(void*)&od_temp_manu_profile_para.x500301_baud_rate_kHz, ODA_SDO_RW|ODA_VWTE_after_reset, 2, NULL, (void*)&od_temp_manu_profile_para.x500301_baud_rate_kHz},
};

/* USER CODE 0 begin */

/* USER CODE 0 end */
