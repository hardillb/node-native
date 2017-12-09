#include <node.h>
#include "test.h"

namespace test {

using v8::Local;
using v8::Object;

void InitAll(Local<Object> exports) {
  Test::Init(exports);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, InitAll)

}
