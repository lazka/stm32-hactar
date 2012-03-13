// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <hactar/misc.h>

/*
 * Example:
 *   GPIO_InitTypeDef GPIO_InitStructure;
 *   GPIO_GetPinConfig(GPIOA, GPIO_Pin_6, &GPIO_InitStructure);
 */
void GPIO_GetPinConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,
        GPIO_InitTypeDef* GPIO_InitStruct)
{
    uint32_t port = 0, cnf = 0, port_config = 0;

    for(port = 0; port < 16; port++) {
        if(GPIO_Pin == (1 << port))
            break;
    }

    if(port >= 8)
        port_config = ((GPIOx->CRH) >> ((port - 8) * 4)) & 0xF;
    else
        port_config = ((GPIOx->CRL) >> ((port) * 4)) & 0xF;

    GPIO_InitStruct->GPIO_Speed = port_config & 0x3;
    cnf = (port_config >> 2) & 0x3;

    if(GPIO_InitStruct->GPIO_Speed == 0)
    {
        if(GPIOx->ODR & GPIO_Pin)
            GPIO_InitStruct->GPIO_Mode = (cnf << 2) | 0x40;
        else
            GPIO_InitStruct->GPIO_Mode = (cnf << 2) | 0x20;
    }
    else
        GPIO_InitStruct->GPIO_Mode = (cnf << 2) | 0x10;

    GPIO_InitStruct->GPIO_Pin = GPIO_Pin;
}
