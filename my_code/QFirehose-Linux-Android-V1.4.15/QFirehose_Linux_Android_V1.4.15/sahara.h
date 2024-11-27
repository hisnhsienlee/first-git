/******************************************************************************
  @file    sahara.h
  @brief   sahara protocol.

  DESCRIPTION
  QFirehoe Tool for USB and PCIE of Quectel wireless cellular modules.

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  None.

  ---------------------------------------------------------------------------
  Copyright (c) 2016 - 2023 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
  ---------------------------------------------------------------------------
******************************************************************************/

#ifndef SAHARA_H
#define SAHARA_H

#define Q_SAHARA_VER 2
#define Q_SAHARA_VER_SUP 4

#define Q_SAHARA_RAW_BUF_SZ (4*1024)

typedef enum
{
  Q_SAHARA_ZERO          = 0x00,
  Q_SAHARA_ONE           = 0x01,
  Q_SAHARA_TWO           = 0x02,
  Q_SAHARA_THREE         = 0x03,
  Q_SAHARA_FOUR          = 0x04,
  Q_SAHARA_FIVE          = 0x05,
  Q_SAHARA_SIX           = 0x06,
  Q_SAHARA_SEVEN         = 0x07,
  Q_SAHARA_EIGTH         = 0x08,
  Q_SAHARA_NINE          = 0x09,
  Q_SAHARA_TEN           = 0x0A,
  Q_SAHARA_ELEVEN        = 0x0B,
  Q_SAHARA_TWELEVE       = 0x0C,
  Q_SAHARA_THIRTEEN      = 0x0D,
  Q_SAHARA_FOURTEEN      = 0x0E,
  Q_SAHARA_FIFTEEN       = 0x0F,
  Q_SAHARA_SIXTEEN	     = 0x10,
  Q_SAHARA_SEVENTEEN     = 0x11,
  Q_SAHARA_EIGHTEEN		 = 0x12,
  Q_SAHARA_NINETEEN,
  Q_SAHARA_TWENTY        = 0x7FFFFFFF
} q_sahara_cmd;

typedef enum {
    Q_SAHARA_IMAGE_ZERO = 0,
    Q_SAHARA_IMAGE_ONE,
    Q_SAHARA_IMAGE_TWO = 0x7FFFFFFF
} q_sahara_image;

typedef enum
{
  Q_SAHARA_STATUS_ZERO =          0x00,
  Q_SAHARA_NAK_ONE =              0x01,
  Q_SAHARA_NAK_TWO =              0x02,
  Q_SAHARA_NAK_THREE =            0x03,
  Q_SAHARA_NAK_FOUR =             0x04,
  Q_SAHARA_NAK_FIVE =             0x05,
  Q_SAHARA_NAK_SIX =              0x06,
  Q_SAHARA_NAK_SEVEN =            0x07,
  Q_SAHARA_NAK_EIGHT =            0x08,
  Q_SAHARA_NAK_NINE =             0x09,
  Q_SAHARA_NAK_TEN =              0x0A,
  Q_SAHARA_NAK_ELEVEN =           0x0B,
  Q_SAHARA_NAK_TWELEVE =          0x0C,
  Q_SAHARA_NAK_THIRTEEN =         0x0D,
  Q_SAHARA_NAK_FOURTEEN =         0x0E,
  Q_SAHARA_NAK_FIFTEEN =          0x0F,
  Q_SAHARA_NAK_SIXTEEN =          0x10,
  Q_SAHARA_NAK_SEVENTEEN =        0x11,
  Q_SAHARA_NAK_EIGHTEEN =         0x12,
  Q_SAHARA_NAK_NINETEEN =         0x13,
  Q_SAHARA_NAK_TWENTY =           0x14,
  Q_SAHARA_NAK_TWENTY_ONE =       0x15,
  Q_SAHARA_NAK_TWENTY_TWO =       0x16,
  Q_SAHARA_NAK_TWENTY_THREE =     0x17,
  Q_SAHARA_NAK_TWENTY_FOUR =      0x18,
  Q_SAHARA_NAK_TWENTY_FIVE =      0x19,
  Q_SAHARA_NAK_TWENTY_SIX =       0x1A,
  Q_SAHARA_NAK_TWENTY_SEVEN =     0x1B,
  Q_SAHARA_NAK_TWENTY_EIGHT =     0x1C,
  Q_SAHARA_NAK_TWENTY_NINE =      0x1D,
  Q_SAHARA_NAK_THIRTY =           0x1E,
  Q_SAHARA_NAK_THIRTY_ONE =       0x1F,
  Q_SAHARA_NAK_THIRTY_TWO =       0x20,
  Q_SAHARA_NAK_THIRTY_THREE =     0x21,
  Q_SAHARA_NAK_THIRTY_FOUR =      0x22,
  Q_SAHARA_NAK_THIRTY_FIVE =      0x23,
  Q_SAHARA_NAK_THIRTY_SIX,
  Q_SAHARA_NAK_THIRTY_SEVEN = 0x7FFFFFFF
} q_sahara_status;

typedef enum
{
  Q_SAHARA_MODE_ZERO   = 0x0,
  Q_SAHARA_MODE_ONE    = 0x1,
  Q_SAHARA_MODE_TWO    = 0x2,
  Q_SAHARA_MODE_THREE  = 0x3,
  Q_SAHARA_MODE_FOUR,
  Q_SAHARA_MODE_FIVE = 0x7FFFFFFF
} q_sahara_mode;

typedef enum
{
  Q_SAHARA_EXEC_ZERO     = 0x00,
  Q_SAHARA_EXEC_ONE      = 0x01,
  Q_SAHARA_EXEC_TWO      = 0x02,
  Q_SAHARA_EXEC_THREE    = 0x03,
  Q_SAHARA_EXEC_FOUR     = 0x04,
  Q_SAHARA_EXEC_FIVE     = 0x05,
  Q_SAHARA_EXEC_SIX      = 0x06,
  Q_SAHARA_EXEC_SEVEN,
  Q_SAHARA_EXEC_EIGHT = 0x7FFFFFFF
} q_sahara_exec;

typedef enum {
    Q_SAHARA_WAIT_ONE,
    Q_SAHARA_WAIT_TWO,
    Q_SAHARA_WAIT_THREE,
    Q_SAHARA_WAIT_FOUR,
    Q_SAHARA_WAIT_FIVE,
    Q_SAHARA_WAIT_SIX,
    Q_SAHARA_WAIT_SEVEN,
    Q_SAHARA_WAIT_EIGHT,
} q_sahara_state;

typedef struct
{
  uint32_t q_cmd;
  uint32_t q_len;
} q_sahara_packet_h;

typedef struct
{
  q_sahara_packet_h q_header;
  uint32_t q_ver;
  uint32_t q_ver_sup;
  uint32_t q_cmd_packet_len;
  uint32_t q_mode;
  uint32_t q_reserve1;
  uint32_t q_reserve2;
  uint32_t q_reserve3;
  uint32_t q_reserve4;
  uint32_t q_reserve5;
  uint32_t q_reserve6;
} q_sahara_hello_packet;

typedef struct
{
  q_sahara_packet_h q_header;
  uint32_t q_ver;
  uint32_t q_ver_sup;
  uint32_t q_status;
  uint32_t q_mode;
  uint32_t q_reserve1;
  uint32_t q_reserve2;
  uint32_t q_reserve3;
  uint32_t q_reserve4;
  uint32_t q_reserve5;
  uint32_t q_reserve6;
} q_sahara_hello_packet_response;

typedef struct
{
  q_sahara_packet_h q_header;
  uint32_t q_image_id;
  uint32_t q_data_offset;
  uint32_t q_data_length;
} q_sahara_read_packet_data;

typedef struct
{
  q_sahara_packet_h q_header;
  uint64_t q_image_id;
  uint64_t q_data_offset;
  uint64_t q_data_length;
} q_sahara_read_packet_data_64bit;

typedef struct
{
  q_sahara_packet_h q_header;
  uint32_t q_image_id;
  uint32_t q_status;
} q_sahara_end_packet_image_tx;

typedef struct
{
  q_sahara_packet_h q_header;
} q_sahara_done_packet;

typedef struct
{
  q_sahara_packet_h q_header;
  uint32_t q_image_tx_status;
} q_sahara_done_packet_response;

typedef struct
{
  q_sahara_packet_h q_header;
} q_sahara_reset_packet;

typedef struct
{
  q_sahara_packet_h q_header;
} q_sahara_reset_packet_response;
#endif
