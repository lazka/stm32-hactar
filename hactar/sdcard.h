// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_SDCARD_H__
#define HACTAR_SDCARD_H__

#include <stdint.h>
#include "stm32f10x.h"

// Public config

#define SD_SPI              SPI3
#define SD_SPI_CLK          RCC_APB1Periph_SPI3
#define SD_SPI_CLK_CMD      RCC_APB1PeriphClockCmd

#define SD_GPIO_Port        GPIOC
#define SD_GPIO_SCK_Pin     GPIO_Pin_10
#define SD_GPIO_MISO_Pin    GPIO_Pin_11
#define SD_GPIO_MOSI_Pin    GPIO_Pin_12
#define SD_GPIO_CLK         RCC_APB2Periph_GPIOC
#define SD_GPIO_CLK_CMD     RCC_APB2PeriphClockCmd

#define SD_GPIO_CS_Port     GPIOA
#define SD_GPIO_CS_Pin      GPIO_Pin_4
#define SD_GPIO_CS_CLK      RCC_APB2Periph_GPIOA
#define SD_GPIO_CS_CLK_CMD  RCC_APB2PeriphClockCmd

#define SD_VOLTAGE_SUPPLY   3300 // in mV
#define SD_HAS_CARD_DETECT  1

#if SD_HAS_CARD_DETECT
#define SD_GPIO_CD_Port     GPIOE
#define SD_GPIO_CD_Pin      GPIO_Pin_0
#define SD_GPIO_CD_CLK      RCC_APB2Periph_GPIOE
#define SD_GPIO_CD_CLK_CMD  RCC_APB2PeriphClockCmd
#endif

// Implementation config

#define SD_HAS_HC_SUPPORT   0

// Helpers
#define SD_GET_BIT(value, bit)          (!!(value & (1 << bit)))
#define SD_OCRV_TO_BIT(value)           ((value - 2700) / 100 + 15)

// Card Types
typedef enum x{SD_NOCARD, SD_ERROR,
               SD_VER_1,
               SD_VER_2, SD_VER_2_HC} SDInitState;

// Commands and arguments
#define SD_CMD0                 (0)

#define SD_CMD8                 (8)
#define SD_CMD8_CHECK_PATTER    (0xAA)  // recommended pattern
#define SD_CMD8_VOLTAGE_27_36   (0x1)   // only one voltage range in 2.0

#define SD_ACMD41               (41)
#define SD_ACMD41_HCS_YES       (0x1)   // Host Capacity Support
#define SD_ACMD41_HCS_NO        (0x0)
#define SD_CMD55                (55)
#define SD_CMD58                (58)

// OCR register

#define SD_OCR_CCS(ocr)         ((ocr >> 30) & 0x1)
#define SD_OCR_BUSY(ocr)        (!((ocr >> 31) & 0x1))

// Responses
#define SD_MAX_RESPONSE_LENGTH          5

#define SD_R1_LENGTH                    1
#define SD_R1_IDLE_STATE(resp)          SD_GET_BIT(resp[0], 0)
#define SD_R1_ERASE_RESET(resp)         SD_GET_BIT(resp[0], 1)
#define SD_R1_ILLEGAL_COMMAND(resp)     SD_GET_BIT(resp[0], 2)
#define SD_R1_COMM_CRC_ERROR(resp)      SD_GET_BIT(resp[0], 3)
#define SD_R1_ERASE_SEQ_ERROR(resp)     SD_GET_BIT(resp[0], 4)
#define SD_R1_ADRRESS_ERROR(resp)       SD_GET_BIT(resp[0], 5)
#define SD_R1_PARAMETER_ERROR(resp)     SD_GET_BIT(resp[0], 6)
#define SD_R1_IS_ERROR(resp)            (resp[0] >> 1)

#define SD_R3_LENGTH                    5
#define SD_R3_OCR(resp)                 (((uint32_t)resp[1] << 24) | (resp[2] << 16) | (resp[3] << 8) | resp[4])

#define SD_R7_LENGTH                    5
#define SD_R7_COMMAND_VERSION(resp)     (resp[1] >> 4)
#define SD_R7_VOLTAGE_ACCEPTED(resp)    (resp[3] & 0x7)
#define SD_R7_CHECK_PATTERN(resp)       (resp[4])

#define SD_IS_RESPONSE_START(byte)      (!SD_GET_BIT(byte, 7))

// Public interface

SDInitState hactarSDInit(void);

#endif
