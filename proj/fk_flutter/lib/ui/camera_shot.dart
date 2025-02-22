import 'package:flutter/material.dart';
import 'package:fk_flutter/fk/fk_image_engine.dart';
import 'package:fk_flutter/utils/logcat.dart';
import 'package:permission_handler/permission_handler.dart';
import 'package:wakelock_plus/wakelock_plus.dart';
import 'package:fk_flutter/fk/entity/FkValue.dart';
import 'package:fk_flutter/fk/plugin/fk_image_hist_plugin.dart';

class CameraShotPage extends StatefulWidget {
  final FkImageEngine _editor;
  final VoidCallback _onClosed;
  final bool showCloseButton;
  final ValueChanged<int> _onCaptureDone;

  const CameraShotPage(this._editor, this.showCloseButton, this._onClosed, this._onCaptureDone, {super.key});

  @override
  State<CameraShotPage> createState() => _CameraShotPageState();
}

class _CameraShotPageState extends State<CameraShotPage>
    with WidgetsBindingObserver {
  int _cameraLayer = -1;
  int _captureLayer = -1;
  int _histLayer = -1;

  void _openCamera() {
    Permission.camera.status.then((status) {
      if (!status.isDenied) {
        widget._editor.openCamera().then((ret) {
          _cameraLayer = ret;
          WakelockPlus.enable();
          final histPlugin = FkImageHistPlugin();
          histPlugin.updateParams({
            "scale": FkValue.toFloat(0.8),
            "ratio": FkValue.toFloat(0.5),
            "location": FkValue.toInt32(0)
          });
          _histLayer = widget._editor.newLayerWithPlugin(histPlugin);
          if (_cameraLayer > 0 && _histLayer > 0) {
            widget._editor.setProjectionLayer(_histLayer, _cameraLayer);
            // widget._editor.setVisibility(_cameraLayer, 1);
          }
          widget._editor.setOnCameraFrameUpdatedListener((){
            final dartCntValue = histPlugin.getParam("cnt_dart");
            final mediumCntValue = histPlugin.getParam("cnt_medium");
            final brightCntValue = histPlugin.getParam("cnt_bright");
            final maxCntValue = histPlugin.getParam("cnt_max");
            final expValueValue = histPlugin.getParam("exposure_value");
            if (expValueValue != null) {
              widget._editor.notifyCameraExposure(expValueValue.int32Val);
            }
            Logcat.debug("cnt_dart=${dartCntValue?.int32Val}, cnt_medium=${mediumCntValue?.int32Val}, cnt_bright=${brightCntValue?.int32Val}, cnt_max=${maxCntValue?.int32Val}, exposure_value=${expValueValue?.int32Val}");
          });
        });
      }
    });
  }

  Future<int> _closeCamera() {
    WakelockPlus.disable();
    widget._editor.setOnCameraFrameUpdatedListener(null);
    widget._editor.removeLayer(_histLayer);
    _histLayer = -1;
    widget._editor.removeLayer(_cameraLayer);
    _cameraLayer = -1;
    return widget._editor.closeCamera();
  }

  bool _isCaptured() {
    return _captureLayer > 0;
  }

  void _captureDone() {
    _closeCamera();
    widget._onCaptureDone(_captureLayer);
  }

  void _captureCancel() {
    if (_cameraLayer <= 0) {
      _openCamera();
    }
    if (_captureLayer > 0) {
      widget._editor.removeLayer(_captureLayer);
      setState(() {
        _captureLayer = -1;
      });
    }
  }

  @override
  void didChangeAppLifecycleState(AppLifecycleState state) {
    switch (state) {
      case AppLifecycleState.resumed:
        Logcat.debug('AppLifecycleState=$state');
        if (_cameraLayer > 0) {
          _openCamera();
        }
      case AppLifecycleState.inactive:
      case AppLifecycleState.detached:
      case AppLifecycleState.paused:
      case AppLifecycleState.hidden:
    }
  }

  @override
  void initState() {
    super.initState();
    Logcat.debug('initState, mounted: $mounted');
    WidgetsBinding.instance.addObserver(this);
    _openCamera();
  }

  @override
  Widget build(BuildContext context) {
    final centerButton = ElevatedButton(
      onPressed: () {
        if (_isCaptured()) {
          _captureDone();
          return;
        }
        widget._editor.capture().then((layerId){
          Logcat.debug('Capture layerId=$layerId');
          _closeCamera();
          setState(() {
            _captureLayer = layerId;
          });
        });
      },
      style: ButtonStyle(
        shape: WidgetStateProperty.all(const CircleBorder()),
        backgroundColor: WidgetStateProperty.all(Colors.red),
      ),
        child: Container(
          width: 64,
          height: 64,
          decoration: BoxDecoration(
              shape: BoxShape.circle,
              color: Colors.transparent,
              border: Border.all(color: Colors.white, width: 3)),
          child: _isCaptured()
              ? const Icon(Icons.done, color: Colors.white, size: 32)
              : null,
        ));
    final saveButton = Align(
        alignment: Alignment.bottomLeft,
        child: Container(
          padding: const EdgeInsets.fromLTRB(16, 0, 0, 0),
          child: IconButton(
            onPressed: () {
              if (_isCaptured()) {
                widget._editor.exportFile("/sdcard/DCIM/${widget._editor.getWorkspaceName()}.jpg");
              }
            },
            style: ButtonStyle(
                padding:
                WidgetStateProperty.all(const EdgeInsets.all(12))),
            icon: Icon(
                !_isCaptured()
                    ? Icons.cameraswitch_outlined
                    : Icons.save_alt,
                color: Colors.white),
            iconSize: 40,
          ),
        ));

    return Container(
      padding: const EdgeInsets.fromLTRB(16, 32, 16, 96),
      child: Stack(
        children: [
          Align(
            alignment: Alignment.bottomCenter,
            child: centerButton,
          ),
          _isCaptured() ? saveButton : Container(),
          Align(
              alignment: Alignment.bottomRight,
              child: Container(
                padding: const EdgeInsets.fromLTRB(0, 0, 16, 0),
                child: IconButton(
                  onPressed: () {
                    if (_isCaptured()) {
                      _captureCancel();
                      return;
                    }
                    widget._editor.switchCamera();
                  },
                  style: ButtonStyle(
                      padding: WidgetStateProperty.all(const EdgeInsets.all(12))),
                  icon: Icon(
                      !_isCaptured()
                          ? Icons.cameraswitch_outlined
                          : Icons.refresh,
                      color: Colors.white),
                  iconSize: 40,
                ),
              )),
          widget.showCloseButton
              ? Align(
                  alignment: Alignment.topLeft,
                  child: Container(
                    child: IconButton(
                      onPressed: () {
                        _captureCancel();
                        _closeCamera().then((ret) {
                          widget._onClosed();
                        });
                      },
                      style: ButtonStyle(
                          padding:
                              WidgetStateProperty.all(const EdgeInsets.all(8))),
                      icon: const Icon(Icons.close, color: Colors.white),
                      iconSize: 40,
                    ),
                  ))
              : Container(),
        ],
      ),
    );
  }
}
