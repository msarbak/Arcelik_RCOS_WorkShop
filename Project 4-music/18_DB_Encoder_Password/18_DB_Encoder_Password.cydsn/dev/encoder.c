/** @file       encoder.c
 *  @brief      C file of ENCODER device
 *  @copyright  (c) 2021-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Ali Ergun
 *  @date       2.06.2021
 */
#define FILE_ENCODER_C

#include "encoder.h"

/**
 *  @addtogroup ENCODER
 *  @{
 */

static DEV_IO_FUNC_INIT(encoderInit);
static DEV_IO_FUNC_DEINIT(encoderDeinit);
static DEV_IO_FUNC_GET(encoderGet);
static DEV_IO_FUNC_PUT(encoderPut);

static const uint8_t encoderLT[] = {2,1,5,6,13,14,10,9,3,0,4,7,12,15,11,8};
                                    
/// @brief  Structure that defines the functions for encoder
const tsDevIoFuncs devIoEncoderFuncs =
{
    encoderInit,
    encoderDeinit,
    encoderGet,
    encoderPut,
};

/// @brief  Init function for a encoder
static DEV_IO_FUNC_INIT(encoderInit)
{
    tsEncoderParams *params       = device->parameters;
    const tsEncoderConsts *consts = device->constants;
    
    if (config)
    {
        const tsEncoderConfig *cfg = config;

        UNUSED(cfg);    // REMOVE IF USED
    }

    UNUSED(params); // REMOVE IF USED
    devIoInit(consts->COM1,NULL);
    devIoInit(consts->COM2,NULL);
    devIoInit(consts->COM3,NULL);
    devIoInit(consts->COM4,NULL);
    
    device->sys->initialized = 1;

    return EXIT_SUCCESS;
}

/// @brief  Deinit function for a encoder
static DEV_IO_FUNC_DEINIT(encoderDeinit)
{
    tsEncoderParams *params       = device->parameters;
    const tsEncoderConsts *consts = device->constants;

    device->sys->initialized = 0;
    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED

    return EXIT_SUCCESS;
}

/// @brief  Get function for a encoder
static DEV_IO_FUNC_GET(encoderGet)
{
    tsEncoderParams *params       = device->parameters;
    const tsEncoderConsts *consts = device->constants;

    
    params->value = devIoGet(consts->COM1);
    params->value += devIoGet(consts->COM2)<<1;
    params->value += devIoGet(consts->COM3)<<2;
    params->value += devIoGet(consts->COM4)<<3;
    
    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED

    return encoderLT[params->value];
}

/// @brief  Put function for a encoder
static DEV_IO_FUNC_PUT(encoderPut)
{
    tsEncoderParams *params       = device->parameters;
    const tsEncoderConsts *consts = device->constants;

    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED
    UNUSED(data);   // REMOVE IF USED

    return EXIT_SUCCESS;
}

/** @} */
