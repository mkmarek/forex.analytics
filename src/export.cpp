#include <nan.h>
#include <node.h>

NAN_METHOD(convertOHLC);
NAN_METHOD(getMarketStatus);
NAN_METHOD(getTrades);
NAN_METHOD(findStrategy);
NAN_METHOD(getIndicatorData);

NAN_MODULE_INIT(Init) {
	NAN_EXPORT(target, convertOHLC);
	NAN_EXPORT(target, getMarketStatus);
	NAN_EXPORT(target, getTrades);
	NAN_EXPORT(target, findStrategy);
	NAN_EXPORT(target, getIndicatorData);
}

NODE_MODULE(analytics, Init);