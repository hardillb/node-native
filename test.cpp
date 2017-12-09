#include <test.h>

namespace test {

    Nan::Persistent<v8::Function> Test::constructor;

    //basic C++ constructor
    Test::Test()  {
    }

    //basic C++ descructor
    Test::~Test() {
    }

    //sets up the javascript bindings
    void Test::Init(v8::Local<v8::Object> exports) {
      Nan::HandleScope scope;

      // Prepare constructor template
      v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
      tpl->SetClassName(Nan::New("Test").ToLocalChecked());
      tpl->InstanceTemplate()->SetInternalFieldCount(1);

      // Prototype
      Nan::SetPrototypeMethod(tpl, "start", Start);
      Nan::SetPrototypeMethod(tpl, "stop", Stop);

      constructor.Reset(tpl->GetFunction());
      exports->Set(Nan::New("Test").ToLocalChecked(),tpl->GetFunction());

    }

    //handles creating new objects
    void Test::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
      if (info.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        Test* obj = new Test();
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());

        v8::Local<v8::Object> context = v8::Local<v8::Object>::Cast(info[0]);
        obj->context.Reset(context);

        uv_loop_t* loop = uv_default_loop();
        uv_async_init(loop, &obj->async, asyncmsg);
      } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 2;
        v8::Local<v8::Value> argv[argc] = { info[0], info[1] };
        v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
        info.GetReturnValue().Set(Nan::NewInstance(cons,argc,argv).ToLocalChecked());
      }
    }

    //starts the background thread
    void Test::Start(const Nan::FunctionCallbackInfo<v8::Value>& args) {
        Nan::HandleScope scope;

        Test* obj = ObjectWrap::Unwrap<Test>(args.Holder());
        obj->run_ = true;
        obj->cb = new Nan::Callback(args[0].As<v8::Function>());

        uv_thread_create(&obj->event_loop, Test::loop, (void*)obj);
    }

    //stops the background thread
    void Test::Stop(const Nan::FunctionCallbackInfo<v8::Value>& args) {
        Test* obj = ObjectWrap::Unwrap<Test>(args.Holder());
        obj->run_ = false;
        uv_close((uv_handle_t*) &obj->async, NULL);
    }

    //the function to run on the background thread
    void Test::loop(void *arg) {
        Test* obj = (Test*)arg;
        int counter = 0;

        while(obj->run_) {
            usleep(1 * 1000 * 1000);
            callbackData *cbd = (callbackData *)malloc(sizeof(callbackData));
            cbd->cb = obj->cb;
            cbd->context = obj;
            cbd->counter = counter++;
            obj->async.data = cbd;
            uv_async_send(&obj->async);
        }
    }

    //makes async callback to Javascript side
    void Test::asyncmsg(uv_async_t* handle) {
        Nan::HandleScope scope;

        Test* obj = (Test*)((callbackData*)handle->data)->context;
        v8::Local<v8::Object> context = Nan::New(obj->context);

        //create object to pass back to javascript with result
        v8::Local<v8::Object> response = Nan::New<v8::Object>();
        response->Set(Nan::New<v8::String>("counter").ToLocalChecked(), Nan::New(((callbackData*)handle->data)->counter));

        v8::Local<v8::Value> argv[] = { response };
        ((Nan::Callback*)((callbackData*)handle->data)->cb)->Call(context,1,argv);
        free(handle->data);
    }
}
