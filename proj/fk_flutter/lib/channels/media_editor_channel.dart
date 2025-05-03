
import 'package:flutter/services.dart';
import 'dart:ffi';
import 'package:fk_flutter/utils/logcat.dart';
import 'package:fk_flutter/fk/entity/fk_result.dart';

const _mediaEditorMethodChannel =
    MethodChannel("com.alimin.flutter/media_editor");

class MediaEditorChannel {
  List<Function(FkResult)> _onInfoListeners = [];
  List<Function(FkResult)> _onTipListeners = [];
  MediaEditorChannel() {
    _mediaEditorMethodChannel.setMethodCallHandler((call) {
      Logcat.debug("Flutter setMethodCallHandler ${call.method}");
      if (call.method == "onDeliveryInfo") {
        _onDeliveryInfo(FkResult(call.arguments["Code"], call.arguments["Msg"]));
      } else if (call.method == "onDeliveryTip") {
        _onDeliveryTip(FkResult(call.arguments["Code"], call.arguments["Msg"]));
      }
      return Future(() => 0);
    });
  }

  void _onDeliveryInfo(FkResult result) {
    for (Function(FkResult) l in _onInfoListeners) {
      l(result);
    }
  }

  void addOnInfoListener(Function(FkResult) l) {
    _onInfoListeners.add(l);
  }

  void removeOnInfoListener(Function(int, String) l) {
    _onInfoListeners.remove(l);
  }

  void _onDeliveryTip(FkResult result) {
    for (Function(FkResult) l in _onTipListeners) {
      l(result);
    }
  }

  void addOnTipListener(Function(FkResult) l) {
    _onTipListeners.add(l);
  }

  void removeOnTipListener(Function(FkResult) l) {
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

  Future<int> setFlashMode(int mode) async {
    return _invokeMethod("setFlashMode", null, {"mode": mode});
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
