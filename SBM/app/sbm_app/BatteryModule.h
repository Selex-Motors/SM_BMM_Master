/*
 * BatteryModule.h
 *
 *  Created on: Sep 8, 2023
 *      Author: Admin
 */

#ifndef APP_SBM_APP_BATTERYMODULE_H_
#define APP_SBM_APP_BATTERYMODULE_H_

#include "sm_sbp.h"

class BatteryModule {
private:
	SBP_t *sbp;
public:
	BatteryModule();
	virtual ~BatteryModule();
};

#endif /* APP_SBM_APP_BATTERYMODULE_H_ */
