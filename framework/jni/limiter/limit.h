/*
 * Postfish Limiter Code
 *
 * Adapted by Auphonic, 2013, to be usable as limiter on Android devices.
 * by Georg Holzmann, grh _at_ auphonic _dot_ com
 *
 * For more info see http://auphonic.com
 *
 *
 * Original comment:
 *
 *  postfish
 *
 *      Copyright (C) 2002-2005 Monty and Xiph.Org
 *
 *  Postfish is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  Postfish is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Postfish; see the file COPYING.  If not, write to the
 *  Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 */

#ifndef _POSTFISH_LIMIT_H_
#define _POSTFISH_LIMIT_H_


#include "bessel.h"

typedef struct{
  sig_atomic_t thresh;
  sig_atomic_t depth;
  sig_atomic_t decay;
} limit_settings;

// Auphonic NOTE: moved limit_state from .c to .h
typedef struct{
  time_linkage out;

  // Auphonic NOTE: feedback is deactivated
  // feedback_generic_pool feedpool;

  iir_state *iir;
  iir_filter decay;
  iir_filter limit;

  int prev_active;
  int initted;

  float pthresh;
  float pdepth;
} limit_state;

// Auphonic NOTE: feedback is deactivated
// extern int pull_limit_feedback(float *peak, float *att, limit_state *limitstate);

extern int limit_load(int ch, int buffersize, limit_state *limitstate, float *window);
extern int limit_reset(limit_state *limitstate);
extern time_linkage *limit_read(time_linkage *in, int buffersize, int samplerate,
                                limit_settings limitset, limit_state *limitstate,
                                float *window, float *peakfeed, float *attfeed);

// Auphonic NOTE: let's disable all global variables!
// extern sig_atomic_t limit_active;
// extern sig_atomic_t limit_visible;
// extern limit_settings limitset;


#endif // _POSTFISH_LIMIT_H_
