// hello.cc
#include <node.h>
#include <uv.h>

#include <iostream>
#include <sstream>

namespace nono {

using namespace v8;

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Integer;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Boolean;
using v8::Persistent;
using v8::Function;
using v8::CopyablePersistentTraits;


static Persistent<Function> callbackDipSwitch;
static Persistent<Integer> tempRandomNumber;


/*
// the 'baton' is the carrier for data between functions
struct DipSwitchBaton {
    // required
    uv_work_t request;                  // libuv
    Persistent<Function> callback;      // javascript callback

    // optional : data goes here.
    // data that doesn't go back to javascript can be any typedef
    // data that goes back to javascript needs to be a supported type
    int         seconds;
    char        greeting[256];
};

*/



void Method(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "nono"));
}

void TriggerOnDipSwitchChange(){

	auto isolate = Isolate::GetCurrent();
	if( callbackDipSwitch.IsEmpty() ){
		std::cout << "ERROR::CALLBACK_NOT_SET No Callback to DipSwitch Changes set yet." << std::endl;
    	return;
	}


    Local<Integer> val = Local<Integer>::New( isolate, tempRandomNumber );

    tempRandomNumber.Reset( isolate, Integer::New( isolate, val->Int32Value() + 1) );


	v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);

	auto context = isolate->GetCurrentContext(); // no longer crashes
	auto global = context->Global();

  	const int argc = 2;
  	Local<Value> argv[argc] = { String::NewFromUtf8(isolate, "hello world"), val };


    Local<Function> local = Local<Function>::New(isolate, callbackDipSwitch);
  	Local<Value> localCB = local;
    if( !callbackDipSwitch.IsEmpty() ){
	    // local->Call( isolate->GetCurrentContext()->Global(), argc, argv );
    }

}



// This is the implementation of the "add" method
// Input arguments are passed using the
// const FunctionCallbackInfo<Value>& args struct
void GetDip(const FunctionCallbackInfo<Value>& args) {

  	Isolate* isolate = args.GetIsolate();
  	TriggerOnDipSwitchChange();
  // Check the number of arguments passed.

  // return the entire DIP switch state
  	if (args.Length() == 0) {
  		// Return the a dummy number for now
		Local<Number> num = Integer::New(isolate, 12345678 );
		args.GetReturnValue().Set( num );

	}else if (args.Length() == 1) {	// return a boolean for a single DIP switch state

		// Check the argument types
		if (!args[0]->IsInt32() || ( (int)args[0]->Int32Value()< 0 || (int)args[0]->Int32Value() > 7) ) {
	    	isolate->ThrowException(Exception::TypeError( String::NewFromUtf8(isolate, "Argument needs to be an Integer between 0 and 7.")));
	    	return;
	  	}

		// Return the a dummy number for now
	  	Local<Boolean> val = Boolean::New(isolate, args[0]->Int32Value()%2==0 );
	  	args.GetReturnValue().Set(val);
	  	return;

  	}else{
	    // Throw an Error that is passed back to JavaScript
    	isolate->ThrowException(Exception::TypeError( String::NewFromUtf8(isolate, "Wrong number of arguments")));
    	return;
  	}

}




void DipSwitchChangeCallback(const FunctionCallbackInfo<Value>& args) {

  	Isolate* isolate = args.GetIsolate();
    Local<Function> callback = Local<Function>::Cast(args[0]);
    callbackDipSwitch.Reset(isolate, callback );
    tempRandomNumber.Reset( isolate,Integer::New( isolate, 77 ) );

}


void init(Local<Object> exports) {


  	NODE_SET_METHOD(exports, "say", Method);
  	NODE_SET_METHOD(exports, "getDip", GetDip);
  	NODE_SET_METHOD(exports, "onDipSwitchChange", DipSwitchChangeCallback);  

}

NODE_MODULE(addon, init)

}  // namespace nono