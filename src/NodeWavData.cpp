#include <node.h>
#include "NodeWavData.h"

bool isBigEndian() {
  int a=1;
  return !((char*)&a)[0];
}

int convertToInt(char* buffer,int len) {
  int a=0;
  if(!isBigEndian())
    for(int i=0;i<len;i++)
      ((char*)&a)[i]=buffer[i];
  else
    for(int i=0;i<len;i++)
      ((char*)&a)[3-i]=buffer[i];
  return a;
}

char* loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size) {
  char buffer[4];
  ifstream in(fn, ios::binary);
  in.read(buffer,4);
  if(strncmp(buffer, "RIFF", 4)!=0) {
    cout << fn << " is not a valid WAVE file"  << endl;
    return NULL;
  }
  in.read(buffer,4);
  in.read(buffer,4);      //WAVE
  in.read(buffer,4);      //fmt
  in.read(buffer,4);      //16
  in.read(buffer,2);      //1
  in.read(buffer,2);
  chan=convertToInt(buffer,2);
  in.read(buffer,4);
  samplerate=convertToInt(buffer,4);
  in.read(buffer,4);
  in.read(buffer,2);
  in.read(buffer,2);
  bps=convertToInt(buffer,2);
  in.read(buffer,4);      //data
  in.read(buffer,4);
  size=convertToInt(buffer,4);
  char* data=new char[size];
  in.read(data,size);
  return data;
}

// ---------------------------------------------------
NodeWavData::NodeWavData(const char* path) {
  data = loadWAV(path, channel, samplerate, bps, size);
};

// ---------------------------------------------------
NodeWavData::~NodeWavData() {
  delete[] data;
};

Nan::Persistent<v8::Function> NodeWavData::constructor;

// ---------------------------------------------------
void NodeWavData::Init(Handle<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName( String::NewFromUtf8(isolate, "WavData", String::kInternalizedString) );
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor.Reset(tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "WavData"), tpl->GetFunction());
}

// ---------------------------------------------------
void NodeWavData::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Isolate* isolate = info.GetIsolate();
  v8::EscapableHandleScope scope(isolate);

  if (info.Length() == 0 || !info[0]->IsString()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "First argument must be a string")));
    scope.Escape(v8::Undefined(isolate));
    return;
  }

  char* path = *v8::String::Utf8Value(info[0]);

  ifstream my_file(path);
  if(!my_file.good()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "File not found")));
    scope.Escape(v8::Undefined(isolate));
    return;
  }

  NodeWavData* wav = new NodeWavData( path );
  wav->Wrap( info.This() );

  info.GetReturnValue().Set(info.This());
}


