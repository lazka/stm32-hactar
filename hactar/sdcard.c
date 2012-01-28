// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <hactar/sdcard.h>
#include <hactar/hactar.h>

#include <stdio.h>
#include <stddef.h>
#include <string.h>

/* Table for CRC-7 (polynomial x^7 + x^3 + 1) */
const uint8_t crc7_syndrome_table[256] = {
    0x00, 0x09, 0x12, 0x1b, 0x24, 0x2d, 0x36, 0x3f,
    0x48, 0x41, 0x5a, 0x53, 0x6c, 0x65, 0x7e, 0x77,
    0x19, 0x10, 0x0b, 0x02, 0x3d, 0x34, 0x2f, 0x26,
    0x51, 0x58, 0x43, 0x4a, 0x75, 0x7c, 0x67, 0x6e,
    0x32, 0x3b, 0x20, 0x29, 0x16, 0x1f, 0x04, 0x0d,
    0x7a, 0x73, 0x68, 0x61, 0x5e, 0x57, 0x4c, 0x45,
    0x2b, 0x22, 0x39, 0x30, 0x0f, 0x06, 0x1d, 0x14,
    0x63, 0x6a, 0x71, 0x78, 0x47, 0x4e, 0x55, 0x5c,
    0x64, 0x6d, 0x76, 0x7f, 0x40, 0x49, 0x52, 0x5b,
    0x2c, 0x25, 0x3e, 0x37, 0x08, 0x01, 0x1a, 0x13,
    0x7d, 0x74, 0x6f, 0x66, 0x59, 0x50, 0x4b, 0x42,
    0x35, 0x3c, 0x27, 0x2e, 0x11, 0x18, 0x03, 0x0a,
    0x56, 0x5f, 0x44, 0x4d, 0x72, 0x7b, 0x60, 0x69,
    0x1e, 0x17, 0x0c, 0x05, 0x3a, 0x33, 0x28, 0x21,
    0x4f, 0x46, 0x5d, 0x54, 0x6b, 0x62, 0x79, 0x70,
    0x07, 0x0e, 0x15, 0x1c, 0x23, 0x2a, 0x31, 0x38,
    0x41, 0x48, 0x53, 0x5a, 0x65, 0x6c, 0x77, 0x7e,
    0x09, 0x00, 0x1b, 0x12, 0x2d, 0x24, 0x3f, 0x36,
    0x58, 0x51, 0x4a, 0x43, 0x7c, 0x75, 0x6e, 0x67,
    0x10, 0x19, 0x02, 0x0b, 0x34, 0x3d, 0x26, 0x2f,
    0x73, 0x7a, 0x61, 0x68, 0x57, 0x5e, 0x45, 0x4c,
    0x3b, 0x32, 0x29, 0x20, 0x1f, 0x16, 0x0d, 0x04,
    0x6a, 0x63, 0x78, 0x71, 0x4e, 0x47, 0x5c, 0x55,
    0x22, 0x2b, 0x30, 0x39, 0x06, 0x0f, 0x14, 0x1d,
    0x25, 0x2c, 0x37, 0x3e, 0x01, 0x08, 0x13, 0x1a,
    0x6d, 0x64, 0x7f, 0x76, 0x49, 0x40, 0x5b, 0x52,
    0x3c, 0x35, 0x2e, 0x27, 0x18, 0x11, 0x0a, 0x03,
    0x74, 0x7d, 0x66, 0x6f, 0x50, 0x59, 0x42, 0x4b,
    0x17, 0x1e, 0x05, 0x0c, 0x33, 0x3a, 0x21, 0x28,
    0x5f, 0x56, 0x4d, 0x44, 0x7b, 0x72, 0x69, 0x60,
    0x0e, 0x07, 0x1c, 0x15, 0x2a, 0x23, 0x38, 0x31,
    0x46, 0x4f, 0x54, 0x5d, 0x62, 0x6b, 0x70, 0x79
};

// taken from the linux kernel
static uint8_t crc7(uint8_t crc, uint8_t *buffer, size_t len)
{
    while (len--)
        crc = crc7_syndrome_table[(crc << 1) ^ *buffer++];
    return crc;
}

static int32_t getCardDetected(void)
{
#if SD_HAS_CARD_DETECT
    return !GPIO_ReadInputDataBit(SD_GPIO_CD_Port, SD_GPIO_CD_Pin);
#else
    return 1;
#endif
}

static void select()
{
    GPIO_ResetBits(SD_GPIO_CS_Port, SD_GPIO_CS_Pin);
}

static void deselect()
{
    GPIO_SetBits(SD_GPIO_CS_Port, SD_GPIO_CS_Pin);
}

static void SPIInit(void)
{
    SD_SPI_CLK_CMD(SD_SPI_CLK, ENABLE);

    SPI_InitTypeDef  spi_init = {
        .SPI_Direction          = SPI_Direction_2Lines_FullDuplex,
        .SPI_Mode               = SPI_Mode_Master,
        .SPI_DataSize           = SPI_DataSize_8b,
        .SPI_CPOL               = SPI_CPOL_Low,
        .SPI_CPHA               = SPI_CPHA_1Edge,
        .SPI_NSS                = SPI_NSS_Soft,
        .SPI_BaudRatePrescaler  = SPI_BaudRatePrescaler_2,
        .SPI_FirstBit           = SPI_FirstBit_MSB,
        .SPI_CRCPolynomial      = 7,
    };

    SPI_Init(SD_SPI, &spi_init);
    SPI_CalculateCRC(SD_SPI, DISABLE);
    SPI_Cmd(SD_SPI, ENABLE);
}

static void portsInit(void)
{
    // Chip Select
    SD_GPIO_CS_CLK_CMD(SD_GPIO_CS_CLK, ENABLE);

    GPIO_InitTypeDef sd_gpio_cs = {
        .GPIO_Pin   = SD_GPIO_CS_Pin,
        .GPIO_Mode  = GPIO_Mode_Out_PP,
        .GPIO_Speed = GPIO_Speed_50MHz,
    };
    GPIO_Init(SD_GPIO_CS_Port, &sd_gpio_cs);

#if SD_HAS_CARD_DETECT
    // Card Detection
    SD_GPIO_CD_CLK_CMD(SD_GPIO_CD_CLK, ENABLE);

    GPIO_InitTypeDef sd_gpio_cd = {
        .GPIO_Pin   = SD_GPIO_CD_Pin,
        .GPIO_Mode  = GPIO_Mode_IN_FLOATING,
        .GPIO_Speed = GPIO_Speed_50MHz,
    };
    GPIO_Init(SD_GPIO_CD_Port, &sd_gpio_cd);
#endif

    // SCK, MOSI, MISO
    SD_GPIO_CLK_CMD(SD_GPIO_CLK, ENABLE);

    GPIO_InitTypeDef sd_gpio_sck = {
        .GPIO_Pin   = SD_GPIO_SCK_Pin,
        .GPIO_Mode  = GPIO_Mode_AF_PP,
        .GPIO_Speed = GPIO_Speed_50MHz,
    };
    GPIO_Init(SD_GPIO_Port, &sd_gpio_sck);

    GPIO_InitTypeDef sd_gpio_miso = {
        .GPIO_Pin   = SD_GPIO_MISO_Pin,
        .GPIO_Mode  = GPIO_Mode_IPU,
        .GPIO_Speed = GPIO_Speed_50MHz,
    };
    GPIO_Init(SD_GPIO_Port, &sd_gpio_miso);

    GPIO_InitTypeDef sd_gpio_mosi = {
        .GPIO_Pin   = SD_GPIO_MOSI_Pin,
        .GPIO_Mode  = GPIO_Mode_AF_PP,
        .GPIO_Speed = GPIO_Speed_50MHz,
    };
    GPIO_Init(SD_GPIO_Port, &sd_gpio_mosi);
}

static uint8_t sendByte(uint8_t data)
{
    while(SPI_I2S_GetFlagStatus(SD_SPI, SPI_I2S_FLAG_TXE) != SET);
    SPI_I2S_SendData(SD_SPI, data);

    while(SPI_I2S_GetFlagStatus(SD_SPI, SPI_I2S_FLAG_RXNE) != SET);
    return SPI_I2S_ReceiveData(SD_SPI);
}

static int32_t sendCommandNoWait(uint8_t command, uint32_t args,
        uint8_t *response, size_t response_size)
{
    size_t i, j;
    uint8_t data[6];

    // 0x40 is start and transmission bit + command
    data[0] = 0x40 | command;

    // copy over the arguments
    data[1] = args >> 24;
    data[2] = args >> 16;
    data[3] = args >> 8;
    data[4] = args;

    // calc the crc7 and set the end bit
    data[5] = (crc7(0, data, 5) << 1) | 0x1;

    // Send command
    for(i = 0; i < 6; i++)
        sendByte(data[i]);

    uint32_t attempts = 10;
    uint8_t byte;

    for(i = 0; i < attempts; i++)
    {
        // Leave MOSI high while receiving
        byte = sendByte(0xFF);

        // All responses start with an R1 token with MSB low
        if(SD_IS_RESPONSE_START(byte))
        {
            response[0] = byte;
            for(j = 1; j < response_size; j++)
                response[j] = sendByte(0xFF);
            return 0;
        }
    }

    return -1;
}

static void waitReady(void)
{
    // Wait for the card to be ready again
    while(sendByte(0xFF) != 0xFF);
}

static int32_t sendCommand(uint8_t command, uint32_t args, uint8_t *response,
        size_t response_size)
{
    waitReady();
    return sendCommandNoWait(command, args, response, response_size);
}

static uint32_t buildCMD8Args(uint32_t voltage_range, uint32_t check_pattern)
{
    return (voltage_range << 8) | check_pattern;
}

static uint32_t getOCRBitMask(uint32_t voltage)
{
    uint32_t mask = 0;

    mask |= 0x1 << SD_OCRV_TO_BIT(voltage);
    mask |= 0x1 << SD_OCRV_TO_BIT(voltage + 1);
    mask |= 0x1 << SD_OCRV_TO_BIT(voltage - 1);
    mask &= 0xFF8000;

    return mask;
}

static uint8_t waitForData(void)
{
    uint8_t data;

    while(1)
    {
        data = sendByte(0xFF);
        // Wait for the start block token
        if(SD_IS_STD_START_BLOCK(data))
            break;
        if(SD_IS_ERROR_BLOCK(data))
            return data;
    }

    return 0;
}

static uint8_t receiveData(uint8_t *dest, size_t size)
{
    uint8_t data;

    if(!size)
        return 0;

    data = waitForData();
    if(data != 0)
        return data;

    // We get the MSB first so save it backwards
    for(; size; size--)
        *(dest + size - 1) = sendByte(0xFF);

    return 0;
}

static uint8_t receiveDataMSBFirst(uint8_t *dest, size_t size)
{
    uint8_t data;
    size_t i;

    if(!size)
        return 0;

    data = waitForData();
    if(data != 0)
        return data;

    for(i = 0; i < size; i++)
        dest[i] = sendByte(0xFF);

    return 0;
}

static int32_t setBlockSize(CardInfo *info, uint32_t block_size)
{
    uint8_t response[SD_MAX_RESPONSE_LENGTH];
    int32_t status = 0;

    if(info->max_block_size_ < block_size)
        return -1;

    select();

    sendCommand(SD_CMD16, block_size, response, SD_R1_LENGTH);
    if(SD_R1_IS_ERROR(response))
        status = -1;

    deselect();

    info->block_size_ = block_size;

    return status;
}

int32_t hactarSDInit(CardInfo *info, uint32_t block_size)
{
    uint8_t response[SD_MAX_RESPONSE_LENGTH];
    uint32_t cmd8_count = 10;

    // Set up CS, MOSI, MISO
    portsInit();

    // Check if there is a card
    if(!getCardDetected())
        return -1;

    // Init SPI
    SPIInit();

    // Wait 10 ms for card to init
    volatile size_t x = hactarGetSystemClock() / 100;
    while(--x);

    // Wait 80 clocks (74 according to spec) with DO, CS high
    size_t i = 10;
    deselect();
    while(--i)
        sendByte(0xFF);

    // Set CS low and send CMD0 to enter SPI mode
    select();
    sendCommandNoWait(SD_CMD0, 0, response, SD_R1_LENGTH);
    if(SD_R1_IS_ERROR(response))
        goto error;

    retry_CMD8:

    // Send the supported voltage and a "random" check pattern
    sendCommand(SD_CMD8,
                buildCMD8Args(SD_CMD8_VOLTAGE_27_36, SD_CMD8_CHECK_PATTER),
                response, SD_R7_LENGTH);

    // Older cards don't support CMD8 and will return "illegal command"
    if(SD_R1_IS_ERROR(response))
    {
        // In case there was a different error, something else went wrong
        if(!SD_R1_ILLEGAL_COMMAND(response))
            goto error;

        // Ver1.X SD Memory Card / or Not SD Memory Card
        info->type_ = SD_VER_1;
    }
    else
    {
        // Check if the send voltage range is supported
        if(SD_R7_VOLTAGE_ACCEPTED(response) != SD_CMD8_VOLTAGE_27_36)
            goto error;

        // In case the pattern got back wrong, resent CMD8
        if(SD_R7_CHECK_PATTERN(response) != SD_CMD8_CHECK_PATTER)
        {
            if(cmd8_count--)
                goto retry_CMD8;
            goto error;
        }

        // Ver2.00 or later SD Memory Cards
        info->type_ = SD_VER_2;
    }

    // Check the supported voltage range and fail if or voltage doesn't work
    sendCommand(SD_CMD58, 0, response, SD_R3_LENGTH);
    if(SD_R1_IS_ERROR(response) ||
            !(SD_R3_OCR(response) & getOCRBitMask(SD_VOLTAGE_SUPPLY)))
        goto error;

    // Send with ACMD41 if we support HC cards it's a Ver 2 card
    uint32_t acmd41_args = SD_ACMD41_HCS_NO;
    if(SD_HAS_HC_SUPPORT && info->type_ == SD_VER_2)
        acmd41_args = SD_ACMD41_HCS_YES;

    retry_ACMD41:

    // Next command is application specific, so send CMD55 first
    sendCommand(SD_CMD55, 0, response, SD_R1_LENGTH);
    if(SD_R1_IS_ERROR(response))
        goto error;

    sendCommand(SD_ACMD41, acmd41_args, response, SD_R1_LENGTH);
    if(SD_R1_IS_ERROR(response))
        goto error;

    // Repeat ACMD until the card isn't in idle state anymore
    if(SD_R1_IDLE_STATE(response))
        goto retry_ACMD41;

    // In case of a ver. 1 card, we are done here
    if(info->type_ == SD_VER_1)
        goto skip_ver2;

    // Check the CSS bit of OCR to see if it's HC card
    // Fail in case OCR is busy, since this means that OCR and CSS are invalid
    sendCommand(SD_CMD58, 0, response, SD_R3_LENGTH);
    if(SD_R1_IS_ERROR(response) || SD_OCR_BUSY(SD_R3_OCR(response)))
        goto error;

    if(SD_OCR_CCS(SD_R3_OCR(response)))
        info->type_ = SD_VER_2_HC;

    skip_ver2:

    // Read the CSD register
    sendCommand(SD_CMD9, 0, response, SD_R1_LENGTH);
    if(SD_R1_IS_ERROR(response))
        goto error;

    uint8_t csd_data[SD_CSD_LENGTH];
    if(receiveDataMSBFirst(csd_data, SD_CSD_LENGTH) != 0)
        goto error;

    // We don't support HC, so this shouldn't happen
    if(SD_CSD_STRUCTURE(csd_data) != SD_CSD_STRUCTURE_1)
        goto error;

    // Block size is 2^READ_BL_LEN
    info->max_block_size_ = (uint32_t)1 << SD_CSD_READ_BL_LEN(csd_data);


    // Capacity is READ_BL_LEN * (C_SIZE + 1) * 2 ^ (C_SIZE_MULT + 2)
    info->capacity_ = info->max_block_size_ * \
                      (SD_CSD_C_SIZE(csd_data) + 1) * \
                      ((uint32_t)1 << (SD_CSD_C_SIZE_MULT(csd_data) + 2));

    // Force a block size
    if(setBlockSize(info, block_size) != 0)
        goto error;

    // ... and the block count
    info->block_count_ = info->capacity_ / block_size;

    // The size of an erasable sector
    info->erase_sectors_count_ = SD_CSD_SECTOR_SIZE(csd_data) + 1;

    deselect();
    return 0;

    error:
        deselect();
        return -1;
}

int32_t hactarGetSDCardID(CardInfo *info, CardID *id)
{
    uint8_t response[SD_MAX_RESPONSE_LENGTH];

    select();

    // Read the CID register
    sendCommand(SD_CMD10, 0, response, SD_R1_LENGTH);
    if(SD_R1_IS_ERROR(response))
        goto error;

    // Read the response
    uint8_t cid_data[SD_CID_LENGTH];
    if(receiveDataMSBFirst(cid_data, SD_CID_LENGTH) != 0)
        goto error;

    id->manufacturer_id_ = SD_CID_MID(cid_data);
    memcpy(id->application_id_, SD_CID_OID(cid_data), 2);
    memcpy(id->product_name_, SD_CID_PNM(cid_data), 5);
    id->product_revision_major_ = SD_CID_PRV(cid_data) >> 4;
    id->product_revision_minor_ = SD_CID_PRV(cid_data) & 0xF;
    id->product_serial_number_ = SD_CID_PSN(cid_data);
    id->manufacturing_year_ = (SD_CID_MDT(cid_data) >> 4) + 2000;
    id->manufacturing_month_ = SD_CID_MDT(cid_data) & 0xF;

    deselect();
    return 0;

    error:
    deselect();
    return -1;
}

int32_t hactarSDWriteBlocks(CardInfo *info, uint32_t block_number,
        size_t block_count, const uint8_t *src)
{
    uint8_t response[SD_MAX_RESPONSE_LENGTH], command = SD_CMD25, data;
    size_t block;
    uint32_t offset;

    if(!block_count)
        return 0;

    select();

    if(block_count == 1)
        command = SD_CMD24;

    sendCommand(command, block_number * info->block_size_,
                response, SD_R1_LENGTH);

    if(SD_R1_IS_ERROR(response))
    {
        deselect();
        return -1;
    }

    waitReady();

    for(block = 0; block < block_count; block++)
    {
        if(command == SD_CMD24)
            sendByte(SD_STD_START_BLOCK);
        else
        {
            if(block + 1 == block_count)
                sendByte(SD_MULT_STOP_BLOCK);
            else
                sendByte(SD_MULT_START_BLOCK);
        }

        offset = info->block_size_;
        while(offset--)
            sendByte(src[offset - 1]);

        // Dummy CRC block
        sendByte(0xFF);
        sendByte(0xFF);

        src += info->block_size_;
    }

    while(SD_IS_DATA_RESPONSE(data = sendByte(0xFF)));
    if(!SD_IS_DATA_ACCPTED(data))
    {
        sendCommand(SD_CMD12, 0, response, SD_R1_LENGTH);
        // No need to wait for busy, since sendCommand does it anyway

        deselect();
        return -1;
    }

    deselect();

    return 0;
}

int32_t hactarSDReadBlocks(CardInfo *info, uint32_t block_number,
        size_t block_count, uint8_t *dest)
{
    uint8_t response[SD_MAX_RESPONSE_LENGTH], command = SD_CMD18;
    size_t block;

    if(!block_count)
        return 0;

    select();

    if(block_count == 1)
        command = SD_CMD17;

    sendCommand(command, block_number * info->block_size_,
                response, SD_R1_LENGTH);

    if(SD_R1_IS_ERROR(response))
    {
        deselect();
        return -1;
    }

    for(block = 0; block < block_count; block++)
    {
        if(receiveData(dest, info->block_size_) != 0)
        {
            deselect();
            return -1;
        }
        dest += info->block_size_;
    }

    deselect();

    return 0;
}
