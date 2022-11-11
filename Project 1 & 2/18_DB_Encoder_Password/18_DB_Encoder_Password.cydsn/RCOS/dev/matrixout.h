/** @file       matrixout.h
 *  @brief      Matrix Output Driver
 *  @copyright  (c) 2018-Arcelik - All Rights Reserved
 *              Permission to use, reproduce, copy, prepare derivative works,
 *              modify, distribute, perform, display or sell this software and/or
 *              its documentation for any purpose is prohibited without the express
 *              written consent of Arcelik A.S.
 *  @author     Deniz Fer
 *  @date       9.11.2018
 *  @date       5.7.2019
 *  @details    A solution is included to place an absolute encoder with pins connected to columns and common to another gpio
 */
#ifndef FILE_MATRIXOUT_H
#define FILE_MATRIXOUT_H

#include "rcos.h"
#include "dev/iolink.h"

/**
 *  @defgroup   MATRIXOUT MATRIXOUT
 *  @ingroup    DEV_CPX
 *  @brief      Matrix Output Driver
 *  @details    With 1ms periods one of the columns is energised and related data is output to rows. Columns assumed to be active low
 *              A solution is included to place an absolute encoder with pins connected to columns and common to another gpio
    @verbatim
           C0  C1  C2  C3  .   .   .
        R0 L00 L01 L02 L03 .   .   .
        R1 L10 L11 L12 L13 .   .   .
        R2 L20 L21 L22 L23 .   .   .
        R3 L30 L31 L32 L33 .   .   .
        ...
           8   4   2   1
        EC 0   0   1   0   => position 2
    @endverbatim
 *  @{
 */

#ifndef FILE_MATRIXOUT_C
#define INTERFACE extern
#else
#define INTERFACE
#endif

/// @brief  Functions for MATRIXOUT devices
INTERFACE const tsDevCpxFuncs devCpxMatrixoutFuncs;

/// @brief  Device specific parameters
typedef struct
{
    tsTimerCallBack timerCb; ///< Timer callback to scan each column
    uint32_t encBuffer;      ///< Encoder buffer
    uint32_t encValue;       ///< Encoder finalised value
    uint8_t brightness;      ///< Value of brightness, 0 is max, as the number increases count of imaginary columns increases
    uint8_t colIdx;          ///< Current column index displayed
} tsMatrixoutParams;

/// @brief  Device specific constants
typedef struct
{
    uint32_t *raw;            ///< Raw bitmap of output(an array of values each corresponds to a column)
    const tsDevIo *rows;      ///< Target devIo for rows value
    const tsDevIo *columns;   ///< Target devIo for columns value
    const tsDevIo *encCommon; ///< Target devIo for absolute encoder common signal
    uint16_t deadTime;        ///< Cycle count of dead zone between each column
    uint8_t colCount;         ///< Total number of columns
} tsMatrixoutConsts;

/** @brief  Create a MATRIXOUT devCpx
 *  @param  _name       Name of devCpx object
 *  @param  _rowPort    Target devIo for rows value
 *  @param  _colPort    Target devIo for columns value
 *  @param  _colCount   Total number of columns
 *  @param  _deadTime   Cycle count of dead zone between each column
 *  @param  _encCommon  Common signal of absolute encoder
 */
#define DEV_CPX_MATRIXOUT_CREATE(_name, _rowPort, _colPort, _colCount, _deadTime, _encCommon) \
    extern TIMER_CALLBACK_FUNC(matrixoutIsr);                                                 \
    const tsDevCpx _name;                                                                     \
    uint32_t _name##Raw[_colCount];                                                           \
    tsMatrixoutParams _name##Params =                                                         \
        {                                                                                     \
            .timerCb    = TIMER_CALLBACK_INIT(matrixoutIsr, &_name),                          \
            .encBuffer  = 0,                                                                  \
            .encValue   = 0,                                                                  \
            .brightness = 0,                                                                  \
            .colIdx     = 0,                                                                  \
    };                                                                                        \
    const tsMatrixoutConsts _name##Consts =                                                   \
        {                                                                                     \
            .raw       = _name##Raw,                                                          \
            .rows      = &_rowPort,                                                           \
            .columns   = &_colPort,                                                           \
            .encCommon = (_encCommon),                                                        \
            .deadTime  = _deadTime,                                                           \
            .colCount  = (_colCount),                                                         \
    };                                                                                        \
    DEV_CPX_CREATE(_name, devCpxMatrixoutFuncs, &_name##Params, &_name##Consts)

/** @brief  Get the value of a single bit from MATRIXOUT device
 *  @param  device  pointer of target devCpx object
 *  @param  row     row index of target bit
 *  @param  col     column index of target bit
 */
INTERFACE uint32_t matrixoutGet(const tsDevCpx *device, uint8_t row, uint8_t col);

/** @brief  Set the value of a single bit from MATRIXOUT device
 *  @param  device  pointer of target devCpx object
 *  @param  row     row index of target bit
 *  @param  col     column index of target bit
 *  @param  data    value to be set
 */
INTERFACE uint8_t matrixoutPut(const tsDevCpx *device, uint8_t row, uint8_t col, uint32_t data);

/** @brief  Get row value from MATRIXOUT device
 *  @param  device  pointer of target devCpx object
 *  @param  row     row index
 */
INTERFACE uint32_t matrixoutGetRow(const tsDevCpx *device, uint8_t row);

/** @brief  Set row value to MATRIXOUT device
 *  @param  device  pointer of target devCpx object
 *  @param  row     row index
 *  @param  data    value to be set
 */
INTERFACE uint8_t matrixoutPutRow(const tsDevCpx *device, uint8_t row, uint32_t data);

/** @brief  Get column value from MATRIXOUT device
 *  @param  device  pointer of target devCpx object
 *  @param  col     column index
 */
INTERFACE uint32_t matrixoutGetCol(const tsDevCpx *device, uint8_t col);

/** @brief  Set column value to MATRIXOUT device
 *  @param  device  pointer of target devCpx object
 *  @param  col     column index
 *  @param  data    value to be set
 */
INTERFACE uint8_t matrixoutPutCol(const tsDevCpx *device, uint8_t col, uint32_t data);

/**
 *  @defgroup   MATRIXOUT_BRIGHTNESS BRIGHTNESS
 *  @ingroup    MATRIXOUT
 *  @brief      Total brightness for the Matrix Output
 *  @{
 */

/** @brief      Create a MATRIXOUT brightness device
 *  @details    0 is max, as the number increases count of imaginary columns increases hence lowers brightness
 *  @param      _name   Name of devIo object
 *  @param      _matrix Target MATRIXOUT devCpx
 */
#define DEV_IO_MATRIXOUT_BRIGHTNESS_CREATE(_name, _matrix) \
    DEV_IO_IOLINK_CREATE(_name, &_matrix##Params.brightness, sizeof(_matrix##Params.brightness))

/** @} */

/** @} */

/**
 *  @defgroup   MATRIXOUT_GPIO GPIO
 *  @brief      Matrix Output Driver GPIO
 *  @ingroup    MATRIXOUT
 *  @{
 */

/// @brief  Functions for MATRIXOUT io type device
INTERFACE const tsDevIoFuncs devIoMatrixoutGpioFuncs;

/// @brief  Device specific constants
typedef struct
{
    const tsDevCpx *matrixout; ///< Target MATRIXOUT devCpx
    uint8_t row;               ///< Target bit row index
    uint8_t col;               ///< Target bit column index
} tsMatrixoutGpioConsts;

/** @brief  Create a MATRIXOUT_GPIO devIo
 *  @param  _name   Name of devIo object
 *  @param  _matrix Target MATRIXOUT devCpx
 *  @param  _row    Row index of bit
 *  @param  _col    Column index of bit
 */
#define DEV_IO_MATRIXOUT_GPIO_CREATE(_name, _matrix, _row, _col) \
    const tsMatrixoutGpioConsts _name##Consts =                  \
        {                                                        \
            &_matrix,                                            \
            (_row),                                              \
            (_col),                                              \
    };                                                           \
    DEV_IO_CREATE(_name, devIoMatrixoutGpioFuncs, NULL, &_name##Consts)

/** @} */

/**
 *  @defgroup   MATRIXOUT_PORT PORT
 *  @brief      Matrix Output Driver PORT
 *  @ingroup    MATRIXOUT
 *  @{
 */

/// @brief  Functions for MATRIXOUT io type device
INTERFACE const tsDevIoFuncs devIoMatrixoutPortFuncs;

/// @brief  MATRIXOUT pin location structure
typedef struct
{
    uint8_t row; ///< Row index of pin
    uint8_t col; ///< Column index of pin
} tsMatrixoutPin;

/// @brief  Device specific Constants
typedef struct
{
    const tsDevCpx *matrixout;  ///< Target MATRIXOUT devCpx
    const tsMatrixoutPin *pins; ///< Array of pins that will be combined to a port
    uint8_t pinCount;           ///< Number of pins in array
} tsMatrixoutPortConsts;

/** @brief  MATRIXOUT_PORT pin information
 *  @param  _r  Row index of pin
 *  @param  _c  Column index of pin
 */
#define DIMP_GPIO(_r, _c) \
    {                     \
        (_r), (_c)        \
    }

/** @brief  Create a MATRIXOUT_PORT devIo
 *  @param  _name   Name of devIo object
 *  @param  _matrix Target MATRIXOUT devCpx
 *  @param  ...     Pin objects defined with DIMP_GPIO
 */
#define DEV_IO_MATRIXOUT_PORT_CREATE(_name, _matrix, ...) \
    const tsMatrixoutPin _name##Pins[] =                  \
        {                                                 \
            __VA_ARGS__,                                  \
    };                                                    \
    const tsMatrixoutPortConsts _name##Consts =           \
        {                                                 \
            &_matrix,                                     \
            _name##Pins,                                  \
            ARRAY_SIZE(_name##Pins),                      \
    };                                                    \
    DEV_IO_CREATE(_name, devIoMatrixoutPortFuncs, NULL, &_name##Consts)

/** @} */

/**
 *  @defgroup   MATRIXOUT_ENCODER ENCODER
 *  @brief      Absolute encoder placed inside a matrixout to collect position data
 *  @warning    Supports maximum of 6-bit encoding of position
 *  @ingroup    MATRIXOUT
 *  @{
 */

/// @brief  Functions for MATRIXOUT encoder io type device
INTERFACE const tsDevIoFuncs devIoMatrixoutEncFuncs;

/// @brief  Device specific constants
typedef struct
{
    const tsDevCpx *matrixout;   ///< Target MATRIXOUT devCpx
    const uint8_t *encDecodeLut; ///< Decoding table for encoder pin positions on columns
    uint8_t encDecodeCnt;        ///< Element count of encDecodeLut
    uint8_t gray;                ///< Gray encoding enabled or disabled
} tsMatrixoutEncConsts;

/** @brief  Create a MATRIXOUT_ENCODER devIo
 *  @param  _name   Name of devIo object
 *  @param  _matrix Target MATRIXOUT devCpx
 *  @param  _gray   Gray encoding is enabled
 *  @param  ...     Define each encoder pin(LSB first) location as column indicies
 */
#define DEV_IO_MATRIXOUT_ENCODER_CREATE(_name, _matrix, _gray, ...) \
    const uint8_t _name##Lut[] =                                    \
        {                                                           \
            __VA_ARGS__,                                            \
    };                                                              \
    const tsMatrixoutEncConsts _name##Consts =                      \
        {                                                           \
            .matrixout    = &_matrix,                               \
            .encDecodeLut = _name##Lut,                             \
            .encDecodeCnt = ARRAY_SIZE(_name##Lut),                 \
            .gray         = _gray,                                  \
    };                                                              \
    DEV_IO_CREATE(_name, devIoMatrixoutEncFuncs, NULL, &_name##Consts)

/** @} */

#undef INTERFACE // Should not let this roam free

#endif // FILE_MATRIXOUT_H
