/*
 * sm_config.c
 *
 *  Created on: Aug 24, 2023
 *      Author: Admin
 */


#include <sm_config_sv.h>


int32_t sm_config_init()
{
    FILE *config = fopen (file_config, "r");

    if (config == NULL)
    {
        config = fopen (file_config, "w");

        if (config == NULL)
            return -1;
    }

    fclose (config);

    return 0;
}
