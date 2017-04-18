#pragma once
#include <iostream>
#include <vector>
#include <node.h>
#include <nan.h>

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

#include "NodeOpenALDevice.h"

using namespace v8;
using namespace std;

class NodeOpenALContext : public Nan::ObjectWrap {
  public:
  static void Init(Handle<Object> exports);
  ALCcontext* context;
  //static vector<NodeOpenALContext*> contexts;

  private:
  NodeOpenALContext(NodeOpenALDevice* dev);
  ~NodeOpenALContext();

  static Nan::Persistent<v8::Function> constructor;
  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
};
