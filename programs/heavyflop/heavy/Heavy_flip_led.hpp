/**
 * Copyright (c) 2024 Enzien Audio, Ltd.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the phrase "powered by heavy",
 *    the heavy logo, and a hyperlink to https://enzienaudio.com, all in a visible
 *    form.
 * 
 *   2.1 If the Application is distributed in a store system (for example,
 *       the Apple "App Store" or "Google Play"), the phrase "powered by heavy"
 *       shall be included in the app description or the copyright text as well as
 *       the in the app itself. The heavy logo will shall be visible in the app
 *       itself as well.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef _HEAVY_CONTEXT_FLIP_LED_HPP_
#define _HEAVY_CONTEXT_FLIP_LED_HPP_

// object includes
#include "HeavyContext.hpp"
#include "HvControlCast.h"
#include "HvControlDelay.h"
#include "HvControlUnop.h"
#include "HvControlSlice.h"
#include "HvControlBinop.h"
#include "HvControlRandom.h"

class Heavy_flip_led : public HeavyContext {

 public:
  Heavy_flip_led(double sampleRate, int poolKb=10, int inQueueKb=2, int outQueueKb=0);
  ~Heavy_flip_led();

  const char *getName() override { return "flip_led"; }
  int getNumInputChannels() override { return 0; }
  int getNumOutputChannels() override { return 0; }

  int process(float **inputBuffers, float **outputBuffer, int n) override;
  int processInline(float *inputBuffers, float *outputBuffer, int n) override;
  int processInlineInterleaved(float *inputBuffers, float *outputBuffer, int n) override;

  int getParameterInfo(int index, HvParameterInfo *info) override;
  struct Event {
    struct Out {
      enum EventOut : hv_uint32_t {
        FLIP_LED_STATE = 0xA2A904DC, // flip_led_state
      };
    };
  };

 private:
  HvTable *getTableForHash(hv_uint32_t tableHash) override;
  void scheduleMessageForReceiver(hv_uint32_t receiverHash, HvMessage *m) override;

  // static sendMessage functions
  static void cSwitchcase_ZN3r94Sb_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_L0xFL0L0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_0YoG8qJT_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_hd0qMJl8_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_RJRGGWmA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cDelay_LF3zWerQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_XI30jpfz_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cMsg_NO8arBiA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_Ytt38cbs_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_UWWssdXu_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSend_WoJkpmVO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_yjckTQok_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_AL6eWprJ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_AEYjLGZV_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_q8yNQbcg_sendMessage(HeavyContextInterface *, int, const HvMessage *);

  // objects
  ControlBinop cBinop_L0xFL0L0;
  ControlRandom cRandom_hd0qMJl8;
  ControlSlice cSlice_RJRGGWmA;
  ControlDelay cDelay_LF3zWerQ;
  ControlBinop cBinop_UWWssdXu;
};

#endif // _HEAVY_CONTEXT_FLIP_LED_HPP_
