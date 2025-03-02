
import 'package:flutter/services.dart';
import 'dart:ffi';
import 'package:fk_flutter/utils/logcat.dart';

const _mediaEditorMethodChannel =
    MethodChannel("com.alimin.flutter/media_editor");

class MediaEditorChannel {
  List<Function(int, String)> _onInfoListeners = [];
  List<Function(int, String, bool)> _onTipListeners = [];
  MediaEditorChannel() {
    _mediaEditorMethodChannel.setMethodCallHandler((call) {
      Logcat.debug("Flutter setMethodCallHandler ${call.method}");
      if (call.method == "onDeliveryInfo") {
        _onDeliveryInfo(call.arguments["Code"], call.arguments["Msg"]);
      } else if (call.method == "onDeliveryTip") {
        _onDeliveryTip(call.arguments["Code"], call.arguments["Msg"], call.arguments["isError"]);
      }
      return Future(() => 0);
    });
  }

  void _onDeliveryInfo(int code, String msg) {
    for (Function(int, String) l in _onInfoListeners) {
      l(code, msg);
    }
  }

  void addOnInfoListener(Function(int, String) l) {
    _onInfoListeners.add(l);
  }

  void removeOnInfoListener(Function(int, String) l) {
    _onInfoListeners.remove(l);
  }

  void _onDeliveryTip(int code, String msg, bool isError) {
    for (Function(int, String) l in _onInfoListeners) {
      l(code, msg);
    }
  }

  void addOnTipListener(Function(int, String, bool) l) {
    _onTipListeners.add(l);
  }

  void removeOnTipListener(Function(int, String, bool) l) {
    _onTipListeners.remove(l);
  }

  Future<int> savePicture() {
    return _invokeMethod("savePicture", null);
  }

  Future<int> attachWindow(Pointer<Void> handle) {
    return _invokeMethod("attachWindow", handle);
  }

  Future<int> detachWindow(Pointer<Void> handle) {
    return _invokeMethod("detachWindow", handle);
  }

  Future<String> convertImage(String path) async {
    return _invokeMethod("convertImage", null, {"path": path});
  }

  Future<int> openCamera() async {
    return _invokeMethod("openCamera", null);
  }

  Future<int> closeCamera() async {
    return _invokeMethod("closeCamera", null);
  }

  Future<int> switchCamera() async {
    return _invokeMethod("switchCamera", null);
  }

  Future<int> capture() async {
    return _invokeMethod("capture", null);
  }

  Future<int> reqTest(Pointer<Void> handle) async {
    return _invokeMethod("reqTest", handle);
  }

  Future<int> notifyCameraExposure(Pointer<Void> handle, int expValue) async {
    return _invokeMethod("onCameraExposureUpdate", handle, {"exposure_value": expValue});
  }

  Future<T> _invokeMethod<T>(String methodName, Pointer<Void>? handle, [Map? params]) async {
    params ??= {};
    if (handle != null) {
      params["handle"] = handle.address;
    }
    return await _mediaEditorMethodChannel.invokeMethod(methodName, params);
  }
}
