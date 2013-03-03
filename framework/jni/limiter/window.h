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

#ifndef _POSTFISH_WINDOW_H_
#define _POSTFISH_WINDOW_H_


extern float *window_get(int type,int n);
extern void window_apply(float *data, float *window, float scale, int halfn);


#endif  // _POSTFISH_WINDOW_H_
