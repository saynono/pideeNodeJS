// hello.cc
#include <node.h>
#include <uv.h>

#include <iostream>
#include <sstream>

namespace nono {

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


// static Persistent<Function, CopyablePersistentTraits<Function>> callbackDipSwitch;
static Persistent<Function> callbackDipSwitch;


uv_loop_t *loop;
uv_async_t async;


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





void Method(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "nono"));
}

void TriggerOnDipSwitchChange(Isolate* isolate){

	// auto isolate = Isolate::GetCurrent();

	v8::Locker locker(isolate);
    v8::HandleScope scope(isolate);

	auto context = isolate->GetCurrentContext(); // no longer crashes
	auto global = context->Global();

  	// const int argc = 1;
  	// Local<Value> argv[argc] = { String::NewFromUtf8(isolate, "hello world") };


  	// const int argc = 0;
  	// Local<Value> argv[argc] = {};

  	// Local<Function> fn = Local<Function>::New(isolate, callbackDipSwitch);
   //  fn->Call(global, argc, argv);


  	std::cout << "callbackDipSwitch : " << &callbackDipSwitch << std::endl;


    Local<Function> local = Local<Function>::New(isolate, callbackDipSwitch);
    local->Call(isolate->GetCurrentContext()->Global(), 0, {});

	// fn->Call(Context::GetCurrent()->Global(), argc, argv);
    // uv_async_send(&async);

    // Local<Function>::New(isolate, callbackDipSwitch)->
    //   Call(isolate->GetCurrentContext()->Global(), argc, argv);

}



// This is the implementation of the "add" method
// Input arguments are passed using the
// const FunctionCallbackInfo<Value>& args struct
void GetDip(const FunctionCallbackInfo<Value>& args) {
  	Isolate* isolate = args.GetIsolate();
  	std::cout << ("NONONO\n");

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


  	TriggerOnDipSwitchChange(isolate);


  // v8::Handle<Function> callback = v8::Handle<Function>::Cast(args[0]);
  // v8::Persistent<Function> cbPers(isolate, arg0);
  // callbackDipSwitch = cbPers;

    Local<Function> callback = Local<Function>::Cast(args[0]);
    // Local<Function> callback = Persistent<Function>::New(Local<Function>::Cast(args[0]));
    // callbackDipSwitch.Reset(isolate, Persistent<Function>::New(callback) );
    // callbackDipSwitch = Persistent<Function>::New(callback);
    callbackDipSwitch = v8::Persistent<v8::Function>::New(args[0]);


  //   // create 'baton' data carrier
  //   DipSwitchBaton *baton = new DipSwitchBaton;
  //   // get callback argument
  //   v8::Handle<Function> cb = v8::Handle<Function>::Cast(args[0]);
  //   // attach baton to uv work request
  //   baton->request.data = baton;
 	// baton->callback = Persistent<Function>::New(cb);
 	// uv_queue_work(uv_default_loop(),&baton->request, DelayAsync, DelayAsyncAfter );



  const unsigned argc = 1;
  Local<Value> argv[argc] = { String::NewFromUtf8(isolate, "hello world") };


  Local<Function> cb = Local<Function>::Cast(args[0]);
  cb->Call(Null(isolate), argc, argv);



	// auto isolateNew = Isolate::GetCurrent();
 //  	v8::HandleScope scope(isolateNew);

 //  	auto fn = Local<Function>::New(isolateNew, callbackDipSwitch);
 //  	fn->Call(isolateNew->GetCurrentContext()->Global(), argc, argv);

}

// /// in Module::Init function 
// uv_async_init(uv_default_loop(), &b.async_request, async_cb); 
// /// in the onEvent function 
// uv_async_send(&b.async_request);


void init(Local<Object> exports) {


  	NODE_SET_METHOD(exports, "say", Method);
  	NODE_SET_METHOD(exports, "getDip", GetDip);
  	NODE_SET_METHOD(exports, "getDip", GetDip);  
  	NODE_SET_METHOD(exports, "onDipSwitchChange", DipSwitchChangeCallback);  

	// loop = uv_default_loop();
    // uv_async_init(loop, &async, TriggerOnDipSwitchChange);
    // uv_run(loop, UV_RUN_DEFAULT);

  // NODE_SET_METHOD(module, "exports", RunCallback);
}

NODE_MODULE(addon, init)

}  // namespace nono