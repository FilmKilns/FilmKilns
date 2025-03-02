import 'package:flutter/material.dart';
import 'package:fk_flutter/utils/logcat.dart';
import 'package:permission_handler/permission_handler.dart';
import 'package:wakelock_plus/wakelock_plus.dart';
import 'package:fk_flutter/fk/entity/FkValue.dart';
import 'package:fk_flutter/channels/media_editor_channel.dart';
import 'package:fk_flutter/fk_result.dart';

class CameraShotPage extends StatefulWidget {
  final VoidCallback _onClosed;
  final bool showCloseButton;
  final ValueChanged<int> _onCaptureDone;

  const CameraShotPage(this.showCloseButton, this._onClosed, this._onCaptureDone, {super.key});

  @override
  State<CameraShotPage> createState() => _CameraShotPageState();
}

class _CameraShotPageState extends State<CameraShotPage>
    with WidgetsBindingObserver {
  final _editor = MediaEditorChannel();
  bool __isCaptured = false;
  bool _showSnackBar = false;

  void _openCamera() {
    _editor.openCamera();
  }

  void _closeCamera() {
    _editor.closeCamera();
  }

  bool _isCaptured() {
    return __isCaptured;
  }

  void _captureDone() {
    setState(() {
      __isCaptured = true;
    });
  }

  void _captureCancel() {
    _openCamera();
    setState(() {
      __isCaptured = false;
    });
  }

  @override
  void didChangeAppLifecycleState(AppLifecycleState state) {
    switch (state) {
      case AppLifecycleState.resumed:
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
    _editor.addOnInfoListener((int code, String msg) {
      switch (code) {
        case FkResult.INFO_CAMERA_TAKE_PICTURE_SUCCESS:
          _captureDone();
          ScaffoldMessenger.of(context).showSnackBar(
            const SnackBar(
              content: Text('这是一个 SnackBar 消息'),
            ),
          );
          break;
      }
    });
    _editor.addOnTipListener((int code, String msg, bool isError) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(
          content: Text('这是一个 SnackBar 消息'),
        ),
      );
    });
    _openCamera();
  }

  @override
  Widget build(BuildContext context) {

    final centerButton = ElevatedButton(
      onPressed: () {
        if (_isCaptured()) {
          return;
        }
        _editor.capture();
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
                _editor.savePicture();
                // widget._editor.exportFile("/sdcard/DCIM/${widget._editor.getWorkspaceName()}.jpg");
              }
            },
            style: ButtonStyle(
                padding:
                WidgetStateProperty.all(const EdgeInsets.all(12))),
            icon: const Icon(
                Icons.save_alt,
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
                    _editor.switchCamera();
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
                        _editor.closeCamera();
                        setState(() {
                          __isCaptured = false;
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
