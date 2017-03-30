#pragma once
#define NUM_BUFFERS 1


#include <nan.h>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

#if defined (_WIN32) || defined (_WIN64)
#include <AL/al.h>
#include <AL/alc.h>
#endif

using namespace v8;
using namespace std;



// http://kcat.strangesoft.net/openal-tutorial.html
class NodeOpenALStream : public Nan::ObjectWrap {
  public:
  static void Init(Handle<v8::Object> exports);

  void buffer(size_t size, char* bufferdata);
  void setPosition(double x, double y, double z);
  void play();
  bool ready();
  void setGain(float g);

  /* These are what we'll use for OpenAL playback */
  ALuint sourceid, buffers[NUM_BUFFERS];
  ALuint frequency;
  ALenum format;

  int n;

  private:
  static Nan::Persistent<v8::Function> constructor;
  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Buffer(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Ready(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void SetPosition(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void GetPosition(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void SetGain(const Nan::FunctionCallbackInfo<v8::Value>& info);

  NodeOpenALStream(int channels, int bps, int _frequency);
  ~NodeOpenALStream();
};
