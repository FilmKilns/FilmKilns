import 'package:flutter/material.dart';
import 'package:fk_flutter/channels/media_editor_channel.dart';
import 'package:fk_flutter/ui/camera_op_panel.dart';
import 'package:fk_flutter/entity/camera_state.dart';

class CameraSheetTop extends StatefulWidget {
  final MediaEditorChannel _editor;
  const CameraSheetTop(this._editor, {super.key});

  @override
  State<CameraSheetTop> createState() => _CameraSheetTop();
}

enum TopButtonType { flash, camera, settings, info }

class _CameraSheetTop extends State<CameraSheetTop> {
  final List<CameraOpItem> camOpItems = [
    const CameraOpItem("test", Icons.abc),
    const CameraOpItem("test", Icons.abc),
    const CameraOpItem("test", Icons.abc),
    const CameraOpItem("test", Icons.abc),
    const CameraOpItem("test", Icons.abc),
  ];
  final List<CameraOpItem> settingsOpItems = [
    const CameraOpItem("test", Icons.abc),
    const CameraOpItem("test", Icons.abc),
    const CameraOpItem("test", Icons.abc),
  ];
  final List<CameraOpItem> infoOpItems = [
    const CameraOpItem("test", Icons.abc),
    const CameraOpItem("test", Icons.abc),
    const CameraOpItem("test", Icons.abc),
  ];
  List<CameraOpItem>? curOp = null;
  CameraFlashState flashState = CameraFlashState.off;

  Widget _wrap(Icon icon, List<CameraOpItem> opItems) {
    return Expanded(
      flex: 1,
      child: IconButton(
        onPressed: () {
          CameraFlashState flashMode = flashState;
          if (flashMode == CameraFlashState.off) {
            flashMode = CameraFlashState.auto;
            widget._editor.setFlashMode(1);
          } else if (flashMode == CameraFlashState.auto) {
            flashMode = CameraFlashState.on;
            widget._editor.setFlashMode(2);
          } else if (flashMode == CameraFlashState.on) {
            flashMode = CameraFlashState.off;
            widget._editor.setFlashMode(0);
          }
          setState(() {
            curOp = curOp == opItems ? null : opItems;
            flashState = flashMode;
          });
        },
        style: ButtonStyle(
            padding: WidgetStateProperty.all(const EdgeInsets.all(0))),
        icon: icon,
        iconSize: 26,
        splashColor: Colors.grey,
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Positioned.fill(
      top: 0,
      child: Container(
        margin: const EdgeInsets.fromLTRB(0, 0, 0, 0),
        child: Column(
          children: [
            Row(
              children: [
                _wrap(const Icon(Icons.flash_auto, color: Colors.white), []),
                _wrap(const Icon(Icons.camera, color: Colors.white), camOpItems),
                _wrap(const Icon(Icons.settings, color: Colors.white), settingsOpItems),
                _wrap(const Icon(Icons.info_outline, color: Colors.white), infoOpItems),
              ],
            ),
            if (curOp != null) CameraOpPanel(curOp!, (index) {

            })
          ],
        ),
      ),
    );
  }
}
