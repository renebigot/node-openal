#pragma once
#include <iostream>
#include <node.h>
#include <v8.h>

#ifdef __linux__
#include <AL/al.h>
#include <AL/alc.h>
#endif

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

#if defined (_WIN32) || defined (_WIN64)
#include <AL/al.h>
#include <AL/alc.h>
#endif

#include "NodeWavData.h"

using namespace v8;
using namespace std;

class NodeOpenALSource : public Nan::ObjectWrap {
  public:
  static void Init(Handle<v8::Object> exports);

  private:
  NodeOpenALSource( NodeWavData* data );
  ~NodeOpenALSource();

  static Nan::Persistent<v8::Function> constructor;
  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Play(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void SetPosition(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void SetLoop(const Nan::FunctionCallbackInfo<v8::Value>& info);

  void play();
  void setPosition(double x, double y, double z);
  void setLoop(bool loop);
  unsigned int bufferid;
  unsigned int format;
  unsigned int sourceid;
};
