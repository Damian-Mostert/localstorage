#include <string.h>
#include <map>
#include <vector>
using namespace std;
#include <napi.h>
using namespace Napi;
namespace localStorage{
  class localStorageValue{
  public:
    string value="";
    bool exists=false;
  };
  map<string,localStorageValue>localStorage;
  Value setItem(const CallbackInfo& info){
    string key = info[0].As<String>();
    string value = info[1].As<String>();
    localStorageValue newValue;
    newValue.value = value;
    newValue.exists = true;
    localStorage[key] = newValue;
    return Boolean::New(info.Env(),1);
  }
  Value getItem(const CallbackInfo& info){
    string key = info[0].As<String>();
    localStorageValue OldValue = localStorage[key];
    if(OldValue.exists){
      return String::New(info.Env(),OldValue.value);
    }else{
      return Boolean::New(info.Env(),0);
    }
  }
  Value removeItem(const CallbackInfo& info){
    string key = info[0].As<String>();
    localStorageValue OldValue = localStorage[key];
    int response = 0;
    if(OldValue.exists){
      response = 1;
      localStorage[key].exists = false;
      localStorage[key].value = "";
    }
    return Boolean::New(info.Env(),response);
    }
    Value getKeys(const CallbackInfo& info){
      Array response = Array::New(info.Env());
      int amount = 0;
      for (const auto&[key, value]:localStorage) {
        response[amount] = key;
        amount++;
      }
      return response;
    }
}
Object MODULE_EXPORTS(Env env, Object exports) {
  exports.Set(String::New(env, "setItem"), Function::New<localStorage::setItem>(env));
  exports.Set(String::New(env, "getItem"), Function::New<localStorage::getItem>(env));
  exports.Set(String::New(env, "removeItem"), Function::New<localStorage::removeItem>(env));
  exports.Set(String::New(env, "getKeys"), Function::New<localStorage::getKeys>(env));
  return exports;
}
NODE_API_MODULE(NODE_GYP_MODULE_NAME,MODULE_EXPORTS);
