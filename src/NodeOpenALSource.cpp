#include <node.h>
#include "NodeOpenALSource.h"

using namespace v8;

// --------------------------------------------------------
NodeOpenALSource::NodeOpenALSource(NodeWavData* data) {

  if(data->channel==1) {
    if(data->bps==8) {
      format=AL_FORMAT_MONO8;
    } else {
      format=AL_FORMAT_MONO16;
    }
  } else {
    if(data->bps==8) {
      format=AL_FORMAT_STEREO8;
    } else {
      format=AL_FORMAT_STEREO16;
    }
  }

  alGenBuffers(1, &bufferid);
  alBufferData(bufferid, format, data->data, data->size, data->samplerate);
  alGenSources(1, &sourceid);
  alSourcei(sourceid, AL_BUFFER, bufferid);
  alSource3f(sourceid, AL_POSITION, 0, 0, 0);
};

// --------------------------------------------------------
NodeOpenALSource::~NodeOpenALSource() {
  alDeleteSources(1, &sourceid);
  alDeleteBuffers(1, &bufferid);
};

Nan::Persistent<v8::Function> NodeOpenALSource::constructor;

// --------------------------------------------------------
void NodeOpenALSource::Init(Handle<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Source").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("Play").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(Play));
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("SetPosition").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(SetPosition));
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("SetLoop").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(SetLoop));

  constructor.Reset(tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "Source"), tpl->GetFunction());
}

// --------------------------------------------------------
void NodeOpenALSource::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Isolate* isolate = info.GetIsolate();

  if (info.Length() < 1) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
    info.GetReturnValue().Set(v8::Undefined(isolate));
    return;
  }

  if ( !info[0]->IsObject() ) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
    info.GetReturnValue().Set(v8::Undefined(isolate));
    return;
  }

  NodeWavData* data = Nan::ObjectWrap::Unwrap<NodeWavData>(info[0]->ToObject());

  NodeOpenALSource* source = new NodeOpenALSource(data);
  source->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}


// --------------------------------------------------------
void NodeOpenALSource::play() {
  alSourcePlay(sourceid);
}

// --------------------------------------------------------
void NodeOpenALSource::setPosition(double x, double y, double z) {
  alSource3f(sourceid, AL_POSITION, x, y, z);
}

// --------------------------------------------------------
void NodeOpenALSource::setLoop(bool loop) {
  if(loop)
    alSourcei(sourceid, AL_LOOPING, AL_TRUE);
  else
    alSourcei(sourceid, AL_LOOPING, AL_FALSE);
}

// --------------------------------------------------------
void NodeOpenALSource::Play(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Isolate* isolate = info.GetIsolate();

  NodeOpenALSource* obj = ObjectWrap::Unwrap<NodeOpenALSource>(info.This());
  obj->play();

  info.GetReturnValue().Set(v8::Undefined(isolate));
}

// --------------------------------------------------------
void NodeOpenALSource::SetPosition(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Isolate* isolate = info.GetIsolate();

  NodeOpenALSource* obj = ObjectWrap::Unwrap<NodeOpenALSource>(info.This());

  double x = info[0]->NumberValue();
  double y = info[1]->NumberValue();
  double z = info[2]->NumberValue();
  obj->setPosition(x, y, z);

  info.GetReturnValue().Set(v8::Undefined(isolate));
}

// --------------------------------------------------------
void NodeOpenALSource::SetLoop(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Isolate* isolate = info.GetIsolate();

  NodeOpenALSource* obj = ObjectWrap::Unwrap<NodeOpenALSource>(info.This());
  bool loop = info[0]->BooleanValue();
  obj->setLoop( loop );

  info.GetReturnValue().Set(v8::Undefined(isolate));
}


