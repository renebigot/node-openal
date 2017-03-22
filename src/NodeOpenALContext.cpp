#include <node.h>
#include "NodeOpenALContext.h"

using namespace v8;

//vector<NodeOpenALContext*> NodeOpenALContext::contexts;


NodeOpenALContext::NodeOpenALContext(NodeOpenALDevice* dev) {
  context = alcCreateContext(dev->device, NULL);
  if(context==NULL) {
    cout << "cannot open context" << endl;
    return;
  }
};


NodeOpenALContext::~NodeOpenALContext() {
  if(context) {
    cout << "destroying context" << endl;
    alcDestroyContext(context);
  }
};

Nan::Persistent<v8::Function> NodeOpenALContext::constructor;

void NodeOpenALContext::Init(Handle<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Context").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor.Reset(tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "Context"), tpl->GetFunction());
}

void NodeOpenALContext::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Isolate* isolate = info.GetIsolate();
  v8::EscapableHandleScope scope(isolate);

  if (info.Length() < 1) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
    scope.Escape(v8::Undefined(isolate));
    return;
  }

  if ( !info[0]->IsObject() ) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
    scope.Escape(v8::Undefined(isolate));
    return;
  }

  NodeOpenALDevice* dev = Nan::ObjectWrap::Unwrap<NodeOpenALDevice>(info[0]->ToObject());

  NodeOpenALContext* ctx = new NodeOpenALContext( dev );
  //contexts.push_back( ctx );

  ctx->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}
