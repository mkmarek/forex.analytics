#include <nan.h>
#include <node.h>

NAN_METHOD(getMarketStatus);
NAN_METHOD(getTrades);
NAN_METHOD(findStrategy);

NAN_MODULE_INIT(Init) {
	NAN_EXPORT(target, getMarketStatus);
	NAN_EXPORT(target, getTrades);
	NAN_EXPORT(target, findStrategy);
}

NODE_MODULE(analytics, Init);
