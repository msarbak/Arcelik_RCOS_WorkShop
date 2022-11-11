/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "rcos.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    rcosMainLoop();
}

/* [] END OF FILE */
