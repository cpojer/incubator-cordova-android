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

#include "postfish.h"
#include "limit.h"
#include "window.h"

// Auphonic NOTE: feedback is deactivated
// #include "feedback.h"

// Auphonic NOTE:
// - all global variables are disabled
// - they are replaced here with samplerate and buffersize
// extern int input_size;
// extern int input_rate;

// Auphonic NOTE: we always set it to active, so we don't need these vars!
// sig_atomic_t limit_active;
// sig_atomic_t limit_visible;

// Auphonic NOTE: these three global variables are also removed
// limit_settings limitset;
// limit_state limitstate;
// static float *window;

// Auphonic NOTE: feedback is deactivated
/* feedback! */
// typedef struct limit_feedback{
//   feedback_generic parent_class;
//   float *peak;
//   float *att;
// } limit_feedback;

// static feedback_generic *new_limit_feedback(void){
//   limit_feedback *ret=calloc(1,sizeof(*ret));
//   return (feedback_generic *)ret;
// }

// // Auphonic NOTE: feedback is deactivated
// int pull_limit_feedback(float *peak, float *att, limit_state *limitstate){
//   limit_feedback *f=(limit_feedback *)feedback_pull(&limitstate->feedpool);

//   if(!f)return 0;

//   if(peak)
//     memcpy(peak,f->peak,sizeof(*peak)*limitstate->out.channels);
//   if(att)
//     memcpy(att,f->att,sizeof(*att)*limitstate->out.channels);
//   feedback_old(&limitstate->feedpool,(feedback_generic *)f);
//   return 1;
// }

/* called only by initial setup */
int limit_load(int ch, int buffersize, limit_state *limitstate, float *window){
  int i;
  memset(limitstate,0,sizeof(*limitstate));

  limitstate->iir=calloc(ch,sizeof(*limitstate->iir));
  limitstate->out.channels=ch;
  limitstate->out.data=malloc(ch*sizeof(*limitstate->out.data));
  for(i=0;i<ch;i++)
    limitstate->out.data[i]=malloc(buffersize*sizeof(**limitstate->out.data));

  // Auphonic NOTE: we don't free windows, they are global + static!
  window=window_get(1,buffersize);

  return(0);
}

static void filter_set(float msec, int order, iir_filter *filter, int samplerate){
  float alpha;
  float corner_freq= 500./msec;

  alpha=corner_freq/samplerate;
  filter->g=mkbessel(alpha,order,filter->c);
  filter->alpha=alpha;
  filter->Hz=alpha*samplerate;
  filter->ms=msec;
}

/* called only in playback thread */
int limit_reset(limit_state *limitstate){
  /* reset cached pipe state */
  // while(pull_limit_feedback(NULL,NULL));
  memset(limitstate->iir,0,limitstate->out.channels*sizeof(*limitstate->iir));
  limitstate->initted=0;
  return 0;
}

static inline float limit_knee(float x,float d){
  return (sqrtf(x*x+d)-x)*-.5f;
}


time_linkage *limit_read(time_linkage *in, int buffersize, int samplerate,
                         limit_settings limitset, limit_state *limitstate,
                         float *window, float *peakfeed, float *attfeed) {
  // Auphonic NOTE:
  // many feedback related things are commented out here!

  int ch=limitstate->out.channels;
  // float peakfeed[ch];
  // float attfeed[ch];

  // int activeC=limit_active;
  // int activeP=limitstate->prev_active;

  // int visible=limit_visible;
  // int bypass;
  int i,k;

  float thresh=limitset.thresh/10.-.01;
  float depth=limitset.depth;

  float localpeak;
  float localatt;

  float decayms=limitset.decay*.1;
  if(decayms!=limitstate->decay.ms){
    filter_set(decayms,2,&limitstate->decay,samplerate);
    filter_set(decayms,1,&limitstate->limit,samplerate);
  }

  if(in->samples==0){
    limitstate->out.samples=0;
    return &limitstate->out;
  }

  depth=depth*.2;
  depth*=depth;

  if(!limitstate->initted){
    limitstate->initted=1;
    // limitstate->prev_active=activeC;

    limitstate->pthresh=thresh;
    limitstate->pdepth=depth;
  }

  for(i=0;i<ch;i++){
    localpeak=0.;
    localatt=0.;

    // Auphonic NOTE: feedback is deactivated
    // bypass=!(activeC || activeP || visible) || mute_channel_muted(in->active,i);

    // if((activeC || activeP) && !mute_channel_muted(in->active,i)){

    float *inx=in->data[i];
    float *x=limitstate->out.data[i];

    float prev_thresh=limitstate->pthresh;
    float prev_depth=limitstate->pdepth;

    float thresh_add=(thresh-prev_thresh)/buffersize;
    float depth_add=(depth-prev_depth)/buffersize;


    /* 'knee' the actual samples, compute attenuation depth */
    for(k=0;k<in->samples;k++){
	    float dB=todB(inx[k]);
	    float knee=limit_knee(dB-prev_thresh,prev_depth)+prev_thresh;
	    float att=dB-knee;

	if(att>localatt)localatt=att;

	    x[k]=att;

	    prev_depth+=depth_add;
	    prev_thresh+=thresh_add;
    }

    compute_iir_decay_limited(x,buffersize,limitstate->iir+i,
				              &limitstate->decay,&limitstate->limit);

    for(k=0;k<in->samples;k++) {
	    x[k]=inx[k]*fromdB(-x[k]);

        // Auphonic NOTE:
        // added method to get max peak feedback
        // TODO: can we disable this when we don't se the plugin?
        if(fabs(x[k])>localpeak)localpeak=fabs(x[k]);
    }

 //    if(activeP && !activeC){
	// /* transition to inactive */
	// for(k=0;k<buffersize;k++){
	//   float w2=1.-window[k];
	//   x[k]= x[k]*w2 + in->data[i][k]*window[k];
	// }
 //      }
 //      if(!activeP && activeC){
	// /* transition to active */
	// for(k=0;k<buffersize;k++){
	//   float w2=1.-window[k];
	//   x[k]= x[k]*window[k] + in->data[i][k]*w2;
	// }
 //      }

    // }else{
    //   float *temp=in->data[i];
    //   in->data[i]=limitstate->out.data[i];
    //   limitstate->out.data[i]=temp;
    // }

 //    if(!bypass){
 //      float *x=limitstate->out.data[i];
 //      /* get peak feedback */
 //      for(k=0;k<in->samples;k++)
	// if(fabs(x[k])>localpeak)localpeak=fabs(x[k]);

 //    }

    peakfeed[i]=todB(localpeak);
    attfeed[i]=localatt;

  }

  limitstate->out.samples=in->samples;

  // Auphonic NOTE: feedback is deactivated
  /* finish up the state feedabck */
  // {
  //   limit_feedback *ff=
  //     (limit_feedback *)feedback_new(&limitstate.feedpool,new_limit_feedback);

  //   if(!ff->peak)
  //     ff->peak=malloc(ch*sizeof(*ff->peak));

  //   if(!ff->att)
  //     ff->att=malloc(ch*sizeof(*ff->att));

  //   memcpy(ff->peak,peakfeed,sizeof(peakfeed));
  //   memcpy(ff->att,attfeed,sizeof(attfeed));

  //   feedback_push(&limitstate.feedpool,(feedback_generic *)ff);
  // }

  {
    int tozero=buffersize-limitstate->out.samples;
    if(tozero)
      for(i=0;i<limitstate->out.channels;i++)
        memset(limitstate->out.data[i]+limitstate->out.samples,0,sizeof(**limitstate->out.data)*tozero);
  }

  limitstate->out.active=in->active;
  // limitstate->prev_active=activeC;
  limitstate->pthresh=thresh;
  limitstate->pdepth=depth;

  return &limitstate->out;
}
