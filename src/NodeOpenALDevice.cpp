#include <node.h>
#include "NodeOpenALDevice.h"

using namespace v8;

//vector<NodeOpenALDevice*> NodeOpenALDevice::devices;

// ------------------------------------------
NodeOpenALDevice::NodeOpenALDevice() {
  device = alcOpenDevice(NULL);
  if(device==NULL) {
    cout << "cannot open sound card" << endl;
    return;
  }
}

// ------------------------------------------
NodeOpenALDevice::~NodeOpenALDevice() {
  if(device) {
    cout << "destroying device" << endl;
    alcCloseDevice(device);
  }
}

Nan::Persistent<v8::Function> NodeOpenALDevice::constructor;

// ------------------------------------------
void NodeOpenALDevice::Init(Handle<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Device").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor.Reset(tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "Device"), tpl->GetFunction());
}

// ------------------------------------------
void NodeOpenALDevice::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  NodeOpenALDevice* obj = new NodeOpenALDevice();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

//void NodeOpenALDevice::ListDevices(const Nan::FunctionCallbackInfo<v8::Value>& info) {
//  ALboolean enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
//  if (enumeration == AL_FALSE) {
//    cout << "cannot enumerate sound cards" << endl;
//  } else {
//    const ALCchar *devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
//    const ALCchar *device = devices;
//    const ALCchar *next = devices + 1;
//    size_t len = 0;
//
//    cout << "Devices list:" << endl;
//    cout << "----------" << endl;
//
//    while (device && *device != '\0' && next && *next != '\0') {
//      fprintf(stdout, "%s\n", device);
//      len = strlen(device);
//      device += (len + 1);
//      next += (len + 2);
//    }
//    cout << "----------" << endl;
//  }
//};
