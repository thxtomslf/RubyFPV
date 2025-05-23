/* $FreeBSD: src/sys/net80211/ieee80211_radiotap.h,v 1.5 2005/01/22 20:12:05 sam Exp $ */
/* $NetBSD: ieee80211_radiotap.h,v 1.11 2005/06/22 06:16:02 dyoung Exp $ */

/*-
 * Copyright (c) 2003, 2004 David Young.  All rights reserved.
 *
 * Redistribution and/or use in source and/or binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions and/or use of the source code (partially or complete) must retain
 *    the above copyright notice, this list of conditions and the following disclaimer
        in the documentation and/or other materials provided with the distribution.
 * 2. Redistributions in binary form (partially or complete) must reproduce
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of David Young may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY DAVID YOUNG ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL DAVID
 * YOUNG BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 */

/*
 * Modifications to fit into the linux IEEE 802.11 stack,
 * Mike Kershaw (dragorn@kismetwireless.net)
 */

#pragma once

#include <linux/if_ether.h>
#include <linux/kernel.h>
#include "../base/base.h"

/* Radiotap header version (from official NetBSD feed) */
#define IEEE80211RADIOTAP_VERSION	"1.5"
/* Base version of the radiotap packet header data */
#define PKTHDR_RADIOTAP_VERSION		0

/* A generic radio capture format is desirable. There is one for
 * Linux, but it is neither rigidly defined (there were not even
 * units given for some fields) nor easily extensible.
 *
 * I suggest the following extensible radio capture format. It is
 * based on a bitmap indicating which fields are present.
 *
 * I am trying to describe precisely what the application programmer
 * should expect in the following, and for that reason I tell the
 * units and origin of each measurement (where it applies), or else I
 * use sufficiently weaselly language ("is a monotonically nondecreasing
 * function of...") that I cannot set false expectations for lawyerly
 * readers.
 */

/* XXX tcpdump/libpcap do not tolerate variable-length headers,
 * yet, so we pad every radiotap header to 64 bytes. Ugh.
 */
#define IEEE80211_RADIOTAP_HDRLEN	64

#define IEEE80211_RADIOTAP_PRESENT_EXTEND_MASK 0x80000000

/* Name                                 Data type    Units
 * ----                                 ---------    -----
 *
 * IEEE80211_RADIOTAP_TSFT              __le64       microseconds
 *
 *      Value in microseconds of the MAC's 64-bit 802.11 Time
 *      Synchronization Function timer when the first bit of the
 *      MPDU arrived at the MAC. For received frames, only.
 *
 * IEEE80211_RADIOTAP_CHANNEL           2 x __le16   MHz, bitmap
 *
 *      Tx/Rx frequency in MHz, followed by flags (see below).
 *
 * IEEE80211_RADIOTAP_FHSS              __le16       see below
 *
 *      For frequency-hopping radios, the hop set (first byte)
 *      and pattern (second byte).
 *
 * IEEE80211_RADIOTAP_RATE              u8           500kb/s
 *
 *      Tx/Rx data rate
 *
 * IEEE80211_RADIOTAP_DBM_ANTSIGNAL     s8           decibels from
 *                                                   one milliwatt (dBm)
 *
 *      RF signal power at the antenna, decibel difference from
 *      one milliwatt.
 *
 * IEEE80211_RADIOTAP_DBM_ANTNOISE      s8           decibels from
 *                                                   one milliwatt (dBm)
 *
 *      RF noise power at the antenna, decibel difference from one
 *      milliwatt.
 *
 * IEEE80211_RADIOTAP_DB_ANTSIGNAL      u8           decibel (dB)
 *
 *      RF signal power at the antenna, decibel difference from an
 *      arbitrary, fixed reference.
 *
 * IEEE80211_RADIOTAP_DB_ANTNOISE       u8           decibel (dB)
 *
 *      RF noise power at the antenna, decibel difference from an
 *      arbitrary, fixed reference point.
 *
 * IEEE80211_RADIOTAP_LOCK_QUALITY      __le16       unitless
 *
 *      Quality of Barker code lock. Unitless. Monotonically
 *      nondecreasing with "better" lock strength. Called "Signal
 *      Quality" in datasheets.  (Is there a standard way to measure
 *      this?)
 *
 * IEEE80211_RADIOTAP_TX_ATTENUATION    __le16       unitless
 *
 *      Transmit power expressed as unitless distance from max
 *      power set at factory calibration.  0 is max power.
 *      Monotonically nondecreasing with lower power levels.
 *
 * IEEE80211_RADIOTAP_DB_TX_ATTENUATION __le16       decibels (dB)
 *
 *      Transmit power expressed as decibel distance from max power
 *      set at factory calibration.  0 is max power.  Monotonically
 *      nondecreasing with lower power levels.
 *
 * IEEE80211_RADIOTAP_DBM_TX_POWER      s8           decibels from
 *                                                   one milliwatt (dBm)
 *
 *      Transmit power expressed as dBm (decibels from a 1 milliwatt
 *      reference). This is the absolute power level measured at
 *      the antenna port.
 *
 * IEEE80211_RADIOTAP_FLAGS             u8           bitmap
 *
 *      Properties of transmitted and received frames. See flags
 *      defined below.
 *
 * IEEE80211_RADIOTAP_ANTENNA           u8           antenna index
 *
 *      Unitless indication of the Rx/Tx antenna for this packet.
 *      The first antenna is antenna 0.
 *
 * IEEE80211_RADIOTAP_RX_FLAGS          __le16       bitmap
 *
 *     Properties of received frames. See flags defined below.
 *
 * IEEE80211_RADIOTAP_TX_FLAGS          __le16       bitmap
 *
 *     Properties of transmitted frames. See flags defined below.
 *
 * IEEE80211_RADIOTAP_RTS_RETRIES       u8           data
 *
 *     Number of rts retries a transmitted frame used.
 *
 * IEEE80211_RADIOTAP_DATA_RETRIES      u8           data
 *
 *     Number of unicast retries a transmitted frame used.
 *
 */
enum ieee80211_radiotap_type {
   IEEE80211_RADIOTAP_TSFT = 0,
   IEEE80211_RADIOTAP_FLAGS = 1,
   IEEE80211_RADIOTAP_RATE = 2,
   IEEE80211_RADIOTAP_CHANNEL = 3,
   IEEE80211_RADIOTAP_FHSS = 4,
   IEEE80211_RADIOTAP_DBM_ANTSIGNAL = 5,
   IEEE80211_RADIOTAP_DBM_ANTNOISE = 6,
   IEEE80211_RADIOTAP_LOCK_QUALITY = 7,
   IEEE80211_RADIOTAP_TX_ATTENUATION = 8,
   IEEE80211_RADIOTAP_DB_TX_ATTENUATION = 9,
   IEEE80211_RADIOTAP_DBM_TX_POWER = 10,
   IEEE80211_RADIOTAP_ANTENNA = 11,
   IEEE80211_RADIOTAP_DB_ANTSIGNAL = 12,
   IEEE80211_RADIOTAP_DB_ANTNOISE = 13,
   IEEE80211_RADIOTAP_RX_FLAGS = 14,
   IEEE80211_RADIOTAP_TX_FLAGS = 15,
   IEEE80211_RADIOTAP_RTS_RETRIES = 16,
   IEEE80211_RADIOTAP_DATA_RETRIES = 17,

   IEEE80211_RADIOTAP_MCS = 19,
   IEEE80211_RADIOTAP_AMPDU_STATUS = 20,
   IEEE80211_RADIOTAP_VHT = 21,
   IEEE80211_RADIOTAP_TIMESTAMP = 22, 

   IEEE80211_RADIOTAP_RADIOTAP_NAMESPACE = 29,
   IEEE80211_RADIOTAP_VENDOR_NAMESPACE = 30,
   IEEE80211_RADIOTAP_EXT = 31
};

/* Channel flags. */
#define	IEEE80211_CHAN_TURBO	0x0010	/* Turbo channel */
#define	IEEE80211_CHAN_CCK	0x0020	/* CCK channel */
#define	IEEE80211_CHAN_OFDM	0x0040	/* OFDM channel */
#define	IEEE80211_CHAN_2GHZ	0x0080	/* 2 GHz spectrum channel. */
#define	IEEE80211_CHAN_5GHZ	0x0100	/* 5 GHz spectrum channel */
#define	IEEE80211_CHAN_PASSIVE	0x0200	/* Only passive scan allowed */
#define	IEEE80211_CHAN_DYN	0x0400	/* Dynamic CCK-OFDM channel */
#define	IEEE80211_CHAN_GFSK	0x0800	/* GFSK channel (FHSS PHY) */

/* For IEEE80211_RADIOTAP_FLAGS */
#define	IEEE80211_RADIOTAP_F_CFP	0x01	/* sent/received
						 * during CFP
						 */
#define	IEEE80211_RADIOTAP_F_SHORTPRE	0x02	/* sent/received
						 * with short
						 * preamble
						 */
#define	IEEE80211_RADIOTAP_F_WEP	0x04	/* sent/received
						 * with WEP encryption
						 */
#define	IEEE80211_RADIOTAP_F_FRAG	0x08	/* sent/received
						 * with fragmentation
						 */
#define	IEEE80211_RADIOTAP_F_FCS	0x10	/* frame includes FCS */
#define	IEEE80211_RADIOTAP_F_DATAPAD	0x20	/* frame has padding between
						 * 802.11 header and payload
						 * (to 32-bit boundary)
						 */
/* For IEEE80211_RADIOTAP_RX_FLAGS */
#define IEEE80211_RADIOTAP_F_RX_BADFCS	0x0001	/* frame failed crc check */

/* For IEEE80211_RADIOTAP_TX_FLAGS */
#define IEEE80211_RADIOTAP_F_TX_FAIL	0x0001	/* failed due to excessive
						 * retries */
#define IEEE80211_RADIOTAP_F_TX_CTS	0x0002	/* used cts 'protection' */
#define IEEE80211_RADIOTAP_F_TX_RTS	0x0004	/* used rts/cts handshake */
#define IEEE80211_RADIOTAP_F_TX_NOACK 0x0008

/* For IEEE80211_RADIOTAP_MCS */
#define IEEE80211_RADIOTAP_MCS_HAVE_BW          0x01
#define IEEE80211_RADIOTAP_MCS_HAVE_MCS         0x02
#define IEEE80211_RADIOTAP_MCS_HAVE_GI          0x04
#define IEEE80211_RADIOTAP_MCS_HAVE_FMT         0x08
#define IEEE80211_RADIOTAP_MCS_HAVE_FEC         0x10
#define IEEE80211_RADIOTAP_MCS_HAVE_STBC        0x20

#define IEEE80211_RADIOTAP_MCS_BW_MASK          0x03
#define         IEEE80211_RADIOTAP_MCS_BW_20    0
#define         IEEE80211_RADIOTAP_MCS_BW_40    1
#define         IEEE80211_RADIOTAP_MCS_BW_20L   2
#define         IEEE80211_RADIOTAP_MCS_BW_20U   3
#define IEEE80211_RADIOTAP_MCS_SGI              0x04
#define IEEE80211_RADIOTAP_MCS_FMT_GF           0x08
#define IEEE80211_RADIOTAP_MCS_FEC_LDPC         0x10
#define IEEE80211_RADIOTAP_MCS_STBC_MASK        0x60
#define         IEEE80211_RADIOTAP_MCS_STBC_1   1
#define         IEEE80211_RADIOTAP_MCS_STBC_2   2
#define         IEEE80211_RADIOTAP_MCS_STBC_3   3

#define IEEE80211_RADIOTAP_MCS_STBC_SHIFT       5

/* The radio capture header precedes the 802.11 header.
 * All data in the header is little endian on all platforms.
 */
struct ieee80211_radiotap_header
{
   u8 it_version;  /* Version 0. Only increases
       * for drastic changes,
       * introduction of compatible
       * new fields does not count.
       */
   u8 it_pad;
   __le16 it_len;  /* length of the whole
       * header in bytes, including
       * it_version, it_pad,
       * it_len, and data fields.
       */
   __le32 it_present; /* A bitmap telling which
       * fields are present. Set bit 31
       * (0x80000000) to extend the
       * bitmap by another 32 bits.
       * Additional extensions are made
       * by setting bit 31.
       */
};

struct radiotap_align_size
{
   uint8_t align:4, size:4;
};

struct ieee80211_radiotap_namespace
{
   const struct radiotap_align_size *align_size;
   int n_bits;
   uint32_t oui;
   uint8_t subns;
};

struct ieee80211_radiotap_vendor_namespaces
{
   const struct ieee80211_radiotap_namespace *ns;
   int n_ns;
};

static inline u16 __get_unaligned_memmove16(const void *p)
{
   u16 tmp;
   memmove(&tmp, p, 2);
   return tmp;
}

static inline u32 __get_unaligned_memmove32(const void *p)
{
   u32 tmp;
   memmove(&tmp, p, 4);
   return tmp;
} 

static inline u16 get_unaligned_le16(const void *p)
{
   u16 tmp = __get_unaligned_memmove16((const u8 *)p);
   return le16_to_cpu(tmp);
}

static inline u32 get_unaligned_le32(const void *p)
{
   u32 tmp = __get_unaligned_memmove32((const u8 *)p);
   return le32_to_cpu(tmp);
} 
