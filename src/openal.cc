#include <node.h>
#include <v8.h>

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

#if defined (_WIN32) || defined (_WIN64)
#include <AL/al.h>
#include <AL/alc.h>
#endif

#include <iostream>
#include "NodeWavData.h"
#include "NodeOpenALDevice.h"
#include "NodeOpenALContext.h"
#include "NodeOpenALSource.h"
#include "NodeOpenALStream.h"

using namespace v8;
using namespace std;

using v8::HandleScope;

void PrintOpenALInfo()
{
  if (alGetString(AL_VERSION))
    cout << "OpenAL version: "    << alGetString(AL_VERSION)    << endl;
  if (alGetString(AL_RENDERER))
    cout << "OpenAL renderer: "   << alGetString(AL_RENDERER)   << endl;
  if (alGetString(AL_VENDOR))
    cout << "OpenAL vendor: "     << alGetString(AL_VENDOR)     << endl;
  if (alGetString(AL_EXTENSIONS))
    cout << "OpenAL extensions: " << alGetString(AL_EXTENSIONS) << endl;

  // Enumerate OpenAL devices
  if (alcIsExtensionPresent (NULL, (const ALCchar *) "ALC_ENUMERATION_EXT") == AL_TRUE)
  {
    const char *s = (const char *) alcGetString(NULL, ALC_DEVICE_SPECIFIER);
    while (*s != '\0')
    {
      cout << "OpenAL available device: " << s << endl;
      while (*s++ != '\0');
    }
  }
  else
  {
    cout << "OpenAL device enumeration isn't available." << endl;
  }

  // Print default device name
  cout << "OpenAL default device: "
    << (const char *)alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER)
    << endl;
}



// --------------------------------------------------
void SetListenerOrientation(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);
  float x = args[0]->NumberValue();
  float y = args[1]->NumberValue();
  float z = args[2]->NumberValue();
  float o[] = {x, y, z, 0, 1, 0};
  alListenerfv(AL_ORIENTATION, o);
}

// --------------------------------------------------
void SetListenerPosition(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);
  float x = args[0]->NumberValue();
  float y = args[1]->NumberValue();
  float z = args[2]->NumberValue();
  float p[] = {x, y, z, 0, 1, 0};
  alListenerfv(AL_POSITION, p);
}

// --------------------------------------------------
void MakeContextCurrent(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  v8::EscapableHandleScope scope(isolate);

  if (args.Length() < 1) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
    scope.Escape(v8::Undefined(isolate));
    return;
  }

  if ( !args[0]->IsObject() ) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
    scope.Escape(v8::Undefined(isolate));
    return;
  }

  NodeOpenALContext* ctx = Nan::ObjectWrap::Unwrap<NodeOpenALContext>(args[0]->ToObject());
  alcMakeContextCurrent(ctx->context);
}

// --------------------------------------------------
void Init(Handle<Object> exports) {
  PrintOpenALInfo();

  NodeWavData::Init(exports);
  NodeOpenALContext::Init(exports);
  NodeOpenALDevice::Init(exports);
  NodeOpenALSource::Init(exports);
  NodeOpenALStream::Init(exports);

  NODE_SET_METHOD(exports, "MakeContextCurrent", MakeContextCurrent);
  NODE_SET_METHOD(exports, "SetListenerPosition", SetListenerPosition);
  NODE_SET_METHOD(exports, "SetListenerOrientation", SetListenerOrientation);
}

/**
*	Initialize
*/
NODE_MODULE(openal, Init)
