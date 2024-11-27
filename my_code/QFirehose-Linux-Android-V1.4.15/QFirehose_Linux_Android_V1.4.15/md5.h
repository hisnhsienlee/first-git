/******************************************************************************
  @file    md5.h
  @brief   md5 checksum.

  DESCRIPTION
  QFirehoe Tool for USB and PCIE of Quectel wireless cellular modules.

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  None.

  ---------------------------------------------------------------------------
  Copyright (c) 2016 - 2023 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
  ---------------------------------------------------------------------------
******************************************************************************/

#ifndef _QUECTEL_MD5_H
#define _QUECTEL_MD5_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include "usb_linux.h"

typedef struct md5_ctx {
    uint32_t lo, hi;
    uint32_t a, b, c, d;
    unsigned char buffer[64];
} md5_ctx_t;


// void dbg_time (const char *fmt, ...);
extern int md5_check(const char *);


#endif /* _QUECTEL_MD5_H */
