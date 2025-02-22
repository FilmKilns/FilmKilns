
import 'package:flutter/services.dart';
import 'dart:ffi';
import 'package:fk_flutter/utils/logcat.dart';

const _mediaEditorMethodChannel =
    MethodChannel("com.alimin.flutter/media_editor");

class MediaEditorChannel {
  late final void Function() attachWindowDone;
  late final void Function() detachWindowDone;
  late final void Function() onCameraFrameUpdated;
  MediaEditorChannel(this.attachWindowDone, this.detachWindowDone, this.onCameraFrameUpdated) {
    _mediaEditorMethodChannel.setMethodCallHandler((call) {
      Logcat.debug("Flutter setMethodCallHandler ${call.method}");
      if (call.method == "attachToSurfaceDone") {
        attachWindowDone();
      } else if (call.method == "detachFromSurfaceDone") {
        detachWindowDone();
      } else if (call.method == "onCameraFrameUpdated") {
        onCameraFrameUpdated();
      }
      return Future(() => 0);
    });
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

  Future<int> openCamera(Pointer<Void> handle) async {
    return _invokeMethod("openCamera", handle);
  }

  Future<int> closeCamera(Pointer<Void> handle) async {
    return _invokeMethod("closeCamera", handle);
  }

  Future<int> switchCamera(Pointer<Void> handle) async {
    return _invokeMethod("switchCamera", handle);
  }

  Future<int> capture(Pointer<Void> handle) async {
    return _invokeMethod("capture", handle);
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
