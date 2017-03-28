#include <node.h>
#include <node_buffer.h>
#include "NodeOpenALStream.h"

string ErrorCheck(ALenum error) {
  if(error == AL_INVALID_NAME) {
    return "Invalid name";

  } else if(error == AL_INVALID_ENUM) {
    return " Invalid enum ";

  } else if(error == AL_INVALID_VALUE) {
    return " Invalid value ";

  } else if(error == AL_INVALID_OPERATION) {
    return " Invalid operation ";

  } else if(error == AL_OUT_OF_MEMORY) {
    return " Out of memory like! ";

  }

  return " Don't know ";
}

using namespace v8;
using namespace std;

Nan::Persistent<v8::Function> NodeOpenALStream::constructor;

// --------------------------------------------------------
void NodeOpenALStream::Init(Handle<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Stream").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("Ready").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(Ready));
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("Buffer").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(Buffer));
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("SetPosition").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(SetPosition));
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("GetPosition").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(GetPosition));
  tpl->PrototypeTemplate()->Set(Nan::New<v8::String>("SetGain").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(SetGain));

  constructor.Reset(tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "Stream"), tpl->GetFunction());
}


// --------------------------------------------------------
void NodeOpenALStream::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Isolate* isolate = info.GetIsolate();

  if (info.Length() < 3) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
    info.GetReturnValue().Set(v8::Undefined(isolate));
    return;
  }

  if ( !info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsNumber()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
    info.GetReturnValue().Set(v8::Undefined(isolate));
    return;
  }

  double channels = info[0]->NumberValue();
  double bps = info[1]->NumberValue();
  double frequency = info[2]->NumberValue();

  NodeOpenALStream* stream = new NodeOpenALStream(channels, bps, frequency);
  stream->Wrap( info.This() );

  info.GetReturnValue().Set(info.This());
}


// --------------------------------------------------------
void NodeOpenALStream::Buffer(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Isolate* isolate = info.GetIsolate();

  NodeOpenALStream* obj = ObjectWrap::Unwrap<NodeOpenALStream>(info.This());

  if (info.Length() < 1) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
    info.GetReturnValue().Set(v8::Undefined(isolate));
    return;
  }

  Local<Value> buffer = info[0];
  size_t size = node::Buffer::Length( buffer->ToObject() );
  char* bufferdata = node::Buffer::Data( buffer->ToObject() );

  obj->buffer(size, bufferdata);

  info.GetReturnValue().Set(v8::Undefined(isolate));
}


// --------------------------------------------------------
void NodeOpenALStream::Ready(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Isolate* isolate = info.GetIsolate();

  NodeOpenALStream* obj = ObjectWrap::Unwrap<NodeOpenALStream>(info.This());

  info.GetReturnValue().Set(Boolean::New(isolate, obj->ready() ));
}

// --------------------------------------------------------
void NodeOpenALStream::SetPosition(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Isolate* isolate = info.GetIsolate();

  NodeOpenALStream* obj = ObjectWrap::Unwrap<NodeOpenALStream>(info.This());

  if (info.Length() < 3) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
    info.GetReturnValue().Set(v8::Undefined(isolate));
    return;
  }

  if ( !info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsNumber()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
    info.GetReturnValue().Set(v8::Undefined(isolate));
    return;
  }

  double x = info[0]->NumberValue();
  double y = info[1]->NumberValue();
  double z = info[2]->NumberValue();
  obj->setPosition(x, y, z);

  info.GetReturnValue().Set(v8::Undefined(isolate));
}

// --------------------------------------------------------
void NodeOpenALStream::GetPosition(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Isolate* isolate = info.GetIsolate();

  NodeOpenALStream* obj = ObjectWrap::Unwrap<NodeOpenALStream>(info.This());

  ALfloat x;
  ALfloat y;
  ALfloat z;
  alGetSource3f(obj->sourceid, AL_POSITION, &x, &y, &z);

  Local<Object> position = Object::New(isolate);
  position->Set(String::NewFromUtf8(isolate, "x", String::kInternalizedString),  Number::New(isolate, x));
  position->Set(String::NewFromUtf8(isolate, "y", String::kInternalizedString),  Number::New(isolate, y));
  position->Set(String::NewFromUtf8(isolate, "z", String::kInternalizedString),  Number::New(isolate, z));

  info.GetReturnValue().Set(position);
}


// --------------------------------------------------------
void NodeOpenALStream::SetGain(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Isolate* isolate = info.GetIsolate();

  NodeOpenALStream* obj = ObjectWrap::Unwrap<NodeOpenALStream>(info.This());

  if (info.Length() < 1) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
    info.GetReturnValue().Set(v8::Undefined(isolate));
    return;
  }

  if ( !info[0]->IsNumber() ) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
    info.GetReturnValue().Set(v8::Undefined(isolate));
    return;
  }


  double x = info[0]->NumberValue();
  obj->setGain(x);

  info.GetReturnValue().Set(v8::Undefined(isolate));
}

// -----------------------------------------------------
NodeOpenALStream::NodeOpenALStream(int channels, int bps, int _frequency) {
  if(channels==1) {
    if(bps==8) {
      format=AL_FORMAT_MONO8;
    } else {
      format=AL_FORMAT_MONO16;
    }
  } else {
    if(bps==8) {
      format=AL_FORMAT_STEREO8;
    } else {
      format=AL_FORMAT_STEREO16;
    }
  }

  /* Generate the buffers and sources */
  alGenBuffers(NUM_BUFFERS, buffers);
  alGenSources(1, &sourceid);
  ALenum error = alGetError();
  if(error != AL_NO_ERROR) {
    cout << "Error generating :( " << ErrorCheck(error) << endl;;
    return;
  }

  n = 0;
  frequency = _frequency;
  alSource3f(sourceid, AL_POSITION, 0, 0, 0);


  ALfloat source0Pos[]={ 2.0, 0.0, 2.0};	// Front left of the listener
  ALfloat source0Vel[]={ 0.0, 0.0, 0.0};

  alSourcef(sourceid, AL_PITCH, 1.0f);
  alSourcef(sourceid, AL_GAIN, 1.0f);
  alSourcefv(sourceid, AL_POSITION, source0Pos);
  alSourcefv(sourceid, AL_VELOCITY, source0Vel);
  alSourcei(sourceid, AL_LOOPING, AL_FALSE);
  alSourcef(sourceid, AL_ROLLOFF_FACTOR, 0.5f);

}

// -----------------------------------------------------
NodeOpenALStream::~NodeOpenALStream() {
  ALint val;
  /* Although mplayer is done giving us data, OpenAL may still be
	 * playing the remaining buffers. Wait until it stops. */
  do {
    alGetSourcei(sourceid, AL_SOURCE_STATE, &val);
  } while(val == AL_PLAYING);

  cout << "deleting source and buffers" << endl;
  alDeleteSources(1, &sourceid);
  alDeleteBuffers(NUM_BUFFERS, buffers);
}

// -----------------------------------------------------
void NodeOpenALStream::buffer(size_t size, char* data) {
//  cout << "received " << size << " bytes" << endl;
  ALenum error;

  // Prefill all of the buffers
  if(n < NUM_BUFFERS-1) {
    alBufferData(buffers[n], format, data, size, frequency);
    error = alGetError();
    if(error != AL_NO_ERROR) {
      cout << "Error loading :( " << ErrorCheck(error) << endl;
      return;
    }
    n++;

    if(n==NUM_BUFFERS-1) {
      alSourceQueueBuffers(sourceid, NUM_BUFFERS, buffers);
      alSourcePlay(sourceid);
    }
  } else {

    ALuint buffer;
    ALint val;

    /* Check if OpenAL is done with any of the queued buffers */
    alGetSourcei(sourceid, AL_BUFFERS_PROCESSED, &val);
    if(val <= 0)
      return;


    /* Pop the oldest queued buffer from the source, fill it
		 * with the new data, then requeue it */
    alSourceUnqueueBuffers(sourceid, 1, &buffer);
    alBufferData(buffer, format, data, size, frequency);
    alSourceQueueBuffers(sourceid, 1, &buffer);
    error = alGetError();
    if(error != AL_NO_ERROR) {
      cout << "Error buffering :( " << ErrorCheck(error) << endl;
      return;
    }

    /* Make sure the source is still playing, and restart it if needed. */
    alGetSourcei(sourceid, AL_SOURCE_STATE, &val);
    if(val != AL_PLAYING)
      alSourcePlay(sourceid);
  }
}

// -----------------------------------------------------
bool NodeOpenALStream::ready() {
  if(n < NUM_BUFFERS-1) {
    return true;
  }

  ALint val;
  alGetSourcei(sourceid, AL_BUFFERS_PROCESSED, &val);

  return val > 0;
}

// -----------------------------------------------------
void NodeOpenALStream::setGain(float g) {
  cout << "SETTING: gain=" << g << endl;
  alSourcef(sourceid, AL_GAIN, g);
}

// --------------------------------------------------------
void NodeOpenALStream::setPosition(double x, double y, double z) {
  cout << "SETTING: x=" << x << " y=" << y << " z=" << z << endl;
  alSource3f(sourceid, AL_POSITION, x, y, z);
}

