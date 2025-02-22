import 'dart:ffi';
import 'dart:io';
import 'package:ffi/ffi.dart';
import 'package:fk_flutter/fk/plugin/fk_abs_plugin.dart';
import 'package:fk_flutter/utils/logcat.dart';
import 'package:fk_flutter/channels/media_editor_channel.dart';
import 'package:flutter/material.dart';
import 'entity/FkPictureModel.pb.dart';
import 'package:fk_flutter/fk/fk_libs.dart';
import 'package:fk_flutter/Properties.dart';

class FkImageEngine {
  late final Directory _workspace;
  late final String _workspaceName;
  late final Pointer<Void> handle;
  late final Pointer<Void> modelHandle;
  late final MediaEditorChannel editorChannel;
  void Function()? _onCameraFrameUpdatedListener;
  late final int Function(Pointer<Void>) _fCreateEngine;
  late final int Function(Pointer<Void>) _fDestroyEngine;
  late final int Function(Pointer<Void>) _fStartEngine;
  late final int Function(Pointer<Void>) _fStopEngine;
  late final int Function(Pointer<Void>) _fNotifyRender;
  late final int Function(Pointer<Void>, int, int) _fNewLayerWithColor;
  late final int Function(Pointer<Void>, Pointer<Utf8>) _fNewLayerWithFile;
  late final int Function(Pointer<Void>, Pointer<Void>) _fNewLayerWithPlugin;
  late final int Function(Pointer<Void>, int) _fRemoveLayer;
  late final int Function(Pointer<Void>, int, int) _fSetBackgroundColor;
  late final int Function(Pointer<Void>, int, int) _fSetProjectionLayer;
  late final int Function(Pointer<Void>, int, int, int) _fSetTranslate;
  late final int Function(Pointer<Void>, int, int) _fSetVisibility;
  late final int Function(Pointer<Void>, Pointer<Utf8>, int, int) _fExportFile;

  late final int Function(Pointer<Void>) _fCreateModelEngine;
  late final int Function(Pointer<Void>) _fDestroyModelEngine;
  late final int Function(Pointer<Void>) _fStartModelEngine;
  late final int Function(Pointer<Void>) _fStopModelEngine;
  late final int Function(Pointer<Void>, Pointer<Utf8>) _fModelEngineLoad;
  late final int Function(Pointer<Void>, Pointer<Utf8>) _fModelEngineSave;
  late final CArray Function(Pointer<Void>) _fModelEngineGetModelBuf;

  FkImageEngine(Directory dir, String workspaceName) {
    _workspaceName = workspaceName;
    _workspace = Directory("${dir.path}/$workspaceName");
    _workspace.createSync(recursive: true);
    editorChannel = MediaEditorChannel(_attachToSurfaceDone, _detachToSurfaceDone, _onCameraFrameUpdated);
    _registerImageEngineFunc();
    _registerImageModelEngineFunc();
    DynamicLibrary nativeLib = FilmKilnsLibs.getInstance().libFkAndroid();
    final Pointer<Void> Function(Pointer<Utf8>) nativeCreate = nativeLib.lookup<NativeFunction<Pointer<Void> Function(Pointer<Utf8>)>>('createImageEngine').asFunction();
    handle = nativeCreate(_workspace.path.toNativeUtf8());
    final Pointer<Void> Function(Pointer<Void>) nativeCreateModel = nativeLib.lookup<NativeFunction<Pointer<Void> Function(Pointer<Void>)>>('createImageModelEngine').asFunction();
    modelHandle = nativeCreateModel(handle);
    Logcat.debug("Create image engine instance=$handle, modelHandle=$modelHandle, workspace=$_workspace");
  }

  void _registerImageEngineFunc() {
    DynamicLibrary nativeLib = FilmKilnsLibs.getInstance().libFkAndroid();
    _fCreateEngine = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>)>>('engineCreate').asFunction();
    _fDestroyEngine = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>)>>('engineDestroy').asFunction();
    _fStartEngine = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>)>>('engineStart').asFunction();
    _fStopEngine = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>)>>('engineStop').asFunction();
    _fNotifyRender = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>)>>('notifyRender').asFunction();
    _fNewLayerWithColor = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>, Int, Int)>>('newLayerWithColor').asFunction();
    _fNewLayerWithFile = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>, Pointer<Utf8>)>>('newLayerWithFile').asFunction();
    _fNewLayerWithPlugin = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>, Pointer<Void>)>>('newLayerWithPlugin').asFunction();
    _fRemoveLayer = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>, Int)>>('FkImage_removeLayer').asFunction();
    _fSetBackgroundColor = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>, Int, Int)>>('setBackgroundColor').asFunction();
    _fSetProjectionLayer = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>, Int, Int)>>('setProjectionLayer').asFunction();
    _fSetTranslate = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>, Int, Int, Int)>>('FkImage_setTranslate').asFunction();
    _fSetVisibility = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>, Int, Int)>>('FkImage_setVisibility').asFunction();
    _fExportFile = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>, Pointer<Utf8>, Int, Int)>>('FkImage_exportFile').asFunction();
  }

  void _registerImageModelEngineFunc() {
    DynamicLibrary nativeLib = FilmKilnsLibs.getInstance().libFkAndroid();
    _fCreateModelEngine = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>)>>('FkImageModel_engineCreate').asFunction();
    _fDestroyModelEngine = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>)>>('FkImageModel_engineDestroy').asFunction();
    _fStartModelEngine = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>)>>('FkImageModel_engineStart').asFunction();
    _fStopModelEngine = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>)>>('FkImageModel_engineStop').asFunction();
    _fModelEngineLoad = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>, Pointer<Utf8>)>>('FkImageModel_load').asFunction();
    _fModelEngineSave = nativeLib.lookup<NativeFunction<Int Function(Pointer<Void>, Pointer<Utf8>)>>('FkImageModel_save').asFunction();
    _fModelEngineGetModelBuf = nativeLib.lookup<NativeFunction<CArray Function(Pointer<Void>)>>('FkImageModel_getPictureModelBuffer').asFunction();
  }

  String getWorkspaceName() {
    return _workspaceName;
  }

  int create() {
    Logcat.debug("Create($handle)");
    final ret = _fCreateEngine(handle);
    _fCreateModelEngine(modelHandle);
    return ret;
  }

  int destroy() {
    Logcat.debug("Destroy($handle)");
    final ret = _fDestroyEngine(handle);
    _fDestroyModelEngine(modelHandle);
    return ret;
  }

  int start() {
    Logcat.debug("Start($handle)");
    final ret =  _fStartEngine(handle);
    _fStartModelEngine(modelHandle);
    load(_workspace.path);
    return ret;
  }

  int stop() {
    Logcat.debug("Stop($handle)");
    save(_workspace.path);
    _fStopModelEngine(modelHandle);
    final ret = _fStopEngine(handle);
    return ret;
  }

  void attachWindow() {
    editorChannel.attachWindow(handle);
  }

  void detachWindow() {
    editorChannel.detachWindow(handle);
  }

  void _attachToSurfaceDone() {
    setBackgroundColor(0, Colors.black);
    notifyRender();
  }

  void _detachToSurfaceDone() {

  }

  void _onCameraFrameUpdated() {
    if (_onCameraFrameUpdatedListener != null) {
      _onCameraFrameUpdatedListener!();
    }
  }

  void setOnCameraFrameUpdatedListener(void Function()? listener) {
    _onCameraFrameUpdatedListener = listener;
  }

  int notifyRender() {
    return _fNotifyRender(handle);
  }

  int newLayerWithColor(int width, int height) {
    return _fNewLayerWithColor(handle, width, height);
  }

  int newLayerWithFile(File file) {
    return _fNewLayerWithFile(handle, file.path.toNativeUtf8());
  }

  int removeLayer(int layerId) {
    return _fRemoveLayer(handle, layerId);
  }

  Future<FkPictureModel?> getPictureModel() {
    return Future(() {
      CArray result = _fModelEngineGetModelBuf(modelHandle);
      if (result.len <= 0) {
        return null;
      }
      final model =
          FkPictureModel.fromBuffer(result.data.asTypedList(result.len));
      malloc.free(result.data);
      return model;
    });
  }

  Future<int> getLayerSize() {
    return Future(() {
      CArray result = _fModelEngineGetModelBuf(modelHandle);
      if (result.len <= 0) {
        return 0;
      }
      final model =
          FkPictureModel.fromBuffer(result.data.asTypedList(result.len));
      malloc.free(result.data);

      return model.layers.length;
    });
  }

  Future<String> convertImage(String path) async {
    return editorChannel.convertImage(path);
  }

  int save(String outputFilePath) {
    return _fModelEngineSave(modelHandle, outputFilePath.toNativeUtf8());
  }

  int load(String inputFilePath) {
    return _fModelEngineLoad(modelHandle, inputFilePath.toNativeUtf8());
  }

  int setBackgroundColor(int layerId, Color color) {
    return _fSetBackgroundColor(handle, layerId, color.value);
  }

  int newLayerWithPlugin(FkAbsPlugin plugin) {
    return _fNewLayerWithPlugin(handle, plugin.getHandle());
  }

  int setProjectionLayer(int layerId, int srcLayerId) {
    return _fSetProjectionLayer(handle, layerId, srcLayerId);
  }

  int setTranslate(int layerId, int x, int y) {
    return _fSetTranslate(handle, layerId, x, y);
  }

  int setVisibility(int layerId, int visibility) {
    return _fSetVisibility(handle, layerId, visibility);
  }

  int exportFile(String outputFilePath) {
    return _fExportFile(handle, outputFilePath.toNativeUtf8(), 2400, 3200);
  }

  Future<int> openCamera() async {
    return editorChannel.openCamera(handle);
  }

  Future<int> closeCamera() async {
    return editorChannel.closeCamera(handle);
  }

  Future<int> switchCamera() async {
    return editorChannel.switchCamera(handle);
  }

  Future<int> capture() async {
    return editorChannel.capture(handle);
  }

  Future<int> reqTest() async {
    return editorChannel.reqTest(handle);
  }

  void notifyCameraExposure(int expValue) async {
    editorChannel.notifyCameraExposure(handle, expValue);
  }
}
