// hello.cc
#include <node.h>

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

void Method(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "nono"));
}




// This is the implementation of the "add" method
// Input arguments are passed using the
// const FunctionCallbackInfo<Value>& args struct
void GetDip(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // Check the number of arguments passed.

  // return the entire DIP switch state
  if (args.Length() == 0) {

  	// Return the a dummy number for now
	Local<Number> num = Integer::New(isolate, 12345678 );
	args.GetReturnValue().Set( num );

  }else if (args.Length() == 1) {	// return a boolean for a single DIP switch state

	  // Check the argument types
	  if (!args[0]->IsInt32() || ( (int)args[0]->Int32Value()< 0 || (int)args[0]->Int32Value() > 7) ) {
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Argument needs to be an Integer between 0 and 7.")));
	    return;
	  }

	  // Return the a dummy number for now
	  Local<Boolean> val = Boolean::New(isolate, args[0]->Int32Value()%2==0 );
	  args.GetReturnValue().Set(val);


  }else{
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }


}


void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "say", Method);
  NODE_SET_METHOD(exports, "getDip", GetDip);
}

NODE_MODULE(addon, init)

}  // namespace nono