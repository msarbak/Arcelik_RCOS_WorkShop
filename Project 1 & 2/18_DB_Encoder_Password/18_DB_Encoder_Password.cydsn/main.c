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
//#include "stdio.h"
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
 //   printf("hello babe");
    rcosMainLoop();
}

/* [] END OF FILE */
