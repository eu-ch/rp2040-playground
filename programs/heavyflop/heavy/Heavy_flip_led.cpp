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

#include "Heavy_flip_led.hpp"

#include <new>

#define Context(_c) static_cast<Heavy_flip_led *>(_c)


/*
 * C Functions
 */

extern "C" {
  HV_EXPORT HeavyContextInterface *hv_flip_led_new(double sampleRate) {
    // allocate aligned memory
    void *ptr = hv_malloc(sizeof(Heavy_flip_led));
    // ensure non-null
    if (!ptr) return nullptr;
    // call constructor
    new(ptr) Heavy_flip_led(sampleRate);
    return Context(ptr);
  }

  HV_EXPORT HeavyContextInterface *hv_flip_led_new_with_options(double sampleRate,
      int poolKb, int inQueueKb, int outQueueKb) {
    // allocate aligned memory
    void *ptr = hv_malloc(sizeof(Heavy_flip_led));
    // ensure non-null
    if (!ptr) return nullptr;
    // call constructor
    new(ptr) Heavy_flip_led(sampleRate, poolKb, inQueueKb, outQueueKb);
    return Context(ptr);
  }

  HV_EXPORT void hv_flip_led_free(HeavyContextInterface *instance) {
    // call destructor
    Context(instance)->~Heavy_flip_led();
    // free memory
    hv_free(instance);
  }
} // extern "C"







/*
 * Class Functions
 */

Heavy_flip_led::Heavy_flip_led(double sampleRate, int poolKb, int inQueueKb, int outQueueKb)
    : HeavyContext(sampleRate, poolKb, inQueueKb, outQueueKb) {
  numBytes += cRandom_init(&cRandom_hd0qMJl8, 474441297);
  numBytes += cSlice_init(&cSlice_RJRGGWmA, 1, 1);
  numBytes += cDelay_init(this, &cDelay_LF3zWerQ, 0.0f);
  
  // schedule a message to trigger all loadbangs via the __hv_init receiver
  scheduleMessageForReceiver(0xCE5CC65B, msg_initWithBang(HV_MESSAGE_ON_STACK(1), 0));
}

Heavy_flip_led::~Heavy_flip_led() {
  // nothing to free
}

HvTable *Heavy_flip_led::getTableForHash(hv_uint32_t tableHash) {
  return nullptr;
}

void Heavy_flip_led::scheduleMessageForReceiver(hv_uint32_t receiverHash, HvMessage *m) {
  switch (receiverHash) {
    case 0xCE5CC65B: { // __hv_init
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_q8yNQbcg_sendMessage);
      break;
    }
    default: return;
  }
}

int Heavy_flip_led::getParameterInfo(int index, HvParameterInfo *info) {
  if (info != nullptr) {
    switch (index) {
      default: {
        info->name = "invalid parameter index";
        info->hash = 0;
        info->type = HvParameterType::HV_PARAM_TYPE_PARAMETER_IN;
        info->minVal = 0.0f;
        info->maxVal = 0.0f;
        info->defaultVal = 0.0f;
        break;
      }
    }
  }
  return 0;
}



/*
 * Send Function Implementations
 */


void Heavy_flip_led::cSwitchcase_ZN3r94Sb_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_RJRGGWmA, 0, m, &cSlice_RJRGGWmA_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_hd0qMJl8, 0, m, &cRandom_hd0qMJl8_sendMessage);
      break;
    }
  }
}

void Heavy_flip_led::cBinop_L0xFL0L0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_0YoG8qJT_sendMessage);
}

void Heavy_flip_led::cUnop_0YoG8qJT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 100.0f, 0, m, &cBinop_UWWssdXu_sendMessage);
}

void Heavy_flip_led::cRandom_hd0qMJl8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 800.0f, 0, m, &cBinop_L0xFL0L0_sendMessage);
}

void Heavy_flip_led::cSlice_RJRGGWmA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_hd0qMJl8, 1, m, &cRandom_hd0qMJl8_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_flip_led::cDelay_LF3zWerQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_LF3zWerQ, m);
  cSwitchcase_ZN3r94Sb_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_flip_led::cSwitchcase_XI30jpfz_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7A5B032D: { // "stop"
      cMsg_NO8arBiA_sendMessage(_c, 0, m);
      break;
    }
    default: {
      cMsg_NO8arBiA_sendMessage(_c, 0, m);
      cDelay_onMessage(_c, &Context(_c)->cDelay_LF3zWerQ, 1, m, &cDelay_LF3zWerQ_sendMessage);
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_Ytt38cbs_sendMessage);
      break;
    }
  }
}

void Heavy_flip_led::cMsg_NO8arBiA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "clear");
  cDelay_onMessage(_c, &Context(_c)->cDelay_LF3zWerQ, 0, m, &cDelay_LF3zWerQ_sendMessage);
}

void Heavy_flip_led::cCast_Ytt38cbs_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_LF3zWerQ, 0, m, &cDelay_LF3zWerQ_sendMessage);
}

void Heavy_flip_led::cBinop_UWWssdXu_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_AL6eWprJ_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_yjckTQok_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_AEYjLGZV_sendMessage);
}

void Heavy_flip_led::cSend_WoJkpmVO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  if (_c->getSendHook() != nullptr) _c->getSendHook()(_c, "flip_led_state", 0xA2A904DC, m);
}

void Heavy_flip_led::cCast_yjckTQok_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_XI30jpfz_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_flip_led::cCast_AL6eWprJ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_LF3zWerQ, 1, m, &cDelay_LF3zWerQ_sendMessage);
}

void Heavy_flip_led::cCast_AEYjLGZV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSend_WoJkpmVO_sendMessage(_c, 0, m);
}

void Heavy_flip_led::cReceive_q8yNQbcg_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_ZN3r94Sb_onMessage(_c, NULL, 0, m, NULL);
}




/*
 * Context Process Implementation
 */

int Heavy_flip_led::process(float **inputBuffers, float **outputBuffers, int n) {
  while (hLp_hasData(&inQueue)) {
    hv_uint32_t numBytes = 0;
    ReceiverMessagePair *p = reinterpret_cast<ReceiverMessagePair *>(hLp_getReadBuffer(&inQueue, &numBytes));
    hv_assert(numBytes >= sizeof(ReceiverMessagePair));
    scheduleMessageForReceiver(p->receiverHash, &p->msg);
    hLp_consume(&inQueue);
  }
  
  hv_uint32_t nextBlock = blockStartTimestamp + n;
  while (mq_hasMessageBefore(&mq, nextBlock)) {
    MessageNode *const node = mq_peek(&mq);
    node->sendMessage(this, node->let, node->m);
    mq_pop(&mq);
  }

  blockStartTimestamp = nextBlock;
  return n;
}

int Heavy_flip_led::processInline(float *inputBuffers, float *outputBuffers, int n4) {
  hv_assert(!(n4 & HV_N_SIMD_MASK)); // ensure that n4 is a multiple of HV_N_SIMD

  // define the heavy input buffer for 0 channel(s)
  float **const bIn = NULL;

  // define the heavy output buffer for 0 channel(s)
  float **const bOut = NULL;

  int n = process(bIn, bOut, n4);
  return n;
}

int Heavy_flip_led::processInlineInterleaved(float *inputBuffers, float *outputBuffers, int n4) {
  hv_assert(n4 & ~HV_N_SIMD_MASK); // ensure that n4 is a multiple of HV_N_SIMD

  // define the heavy input buffer for 0 channel(s), uninterleave
  float *const bIn = NULL;

  // define the heavy output buffer for 0 channel(s)
  float *const bOut = NULL;

  int n = processInline(bIn, bOut, n4);

  

  return n;
}
