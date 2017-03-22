#ifndef NODE_OPEN_AL_DEVICE_H
#define NODE_OPEN_AL_DEVICE_H

#include <iostream>
#include <vector>
#include <node.h>
#include <nan.h>
#include <v8.h>

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

class NodeOpenALDevice : public Nan::ObjectWrap {
  public:
  static void Init(Handle<Object> exports);
  static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> arg);
  ALCdevice* device;

  private:
  NodeOpenALDevice();
  ~NodeOpenALDevice();

  static Nan::Persistent<v8::Function> constructor;
  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
};

#endif

