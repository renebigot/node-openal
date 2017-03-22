#pragma once
#include <node.h>
#include <nan.h>
#include <fstream>
#include <cstring>
#include <iostream>

using namespace v8;
using namespace std;

class NodeWavData : public Nan::ObjectWrap {
  public:
  static void Init(Handle<Object> exports);

  char* data;
  int channel;
  int samplerate;
  int bps;
  int size;

  private:
  NodeWavData(const char* path);
  ~NodeWavData();

  static Nan::Persistent<v8::Function> constructor;
  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
};
