#ifndef SBIONative_H
#define SBIONative_H

#include <iostream>
#include <unistd.h> 

#include <nan.h>

namespace test {
    class Test : public node::ObjectWrap {
    public:
        static void Init(v8::Local<v8::Object> exports);

    private:
        explicit Test();
        ~Test();

        static void New(const Nan::FunctionCallbackInfo<v8::Value>& args);
        static Nan::Persistent<v8::Function> constructor;
        static void Start(const Nan::FunctionCallbackInfo<v8::Value>& args);
        static void Stop(const Nan::FunctionCallbackInfo<v8::Value>& args);

        static void loop(void *arg);
        static void asyncmsg(uv_async_t* handle);

        //store the context for the callback
        Nan::Persistent<v8::Object> context;
        Nan::Callback *cb;
        //control variable for the loop
        bool run_;
        uv_async_t async;
        uv_thread_t event_loop;
    };

    //structure to pass data from the loop
    struct callbackData {
        Nan::Callback *cb;
        Test *context;
        int counter;
    };
}
#endif


