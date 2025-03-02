import 'package:fk_flutter/widgets/bottom_navigation_bar_v2.dart';
import 'package:flutter/material.dart';
import '../utils/logcat.dart';
import 'layer_list.dart';
import 'dart:io';
import 'dart:convert';
import 'package:crypto/crypto.dart';
import '../Properties.dart';
import 'package:fk_flutter/entity/layer_state.dart';
import 'package:fk_flutter/utils/system_utils.dart';
import 'package:fk_flutter/ui/camera_shot.dart';
import 'package:fk_flutter/channels/media_editor_channel.dart';

class MediaEditorPage extends StatefulWidget {
  const MediaEditorPage(this.fixedCameraPage, {super.key, required this.title});

  final String title;
  final bool fixedCameraPage;

  @override
  State<MediaEditorPage> createState() => _MediaEditorPageState();
}

class _MediaEditorPageState extends State<MediaEditorPage> with WidgetsBindingObserver {
  _MediaEditorPageState() {
    final content = const Utf8Encoder().convert(DateTime.now().toString());
    final md5Str = md5.convert(content).toString();
    _workspace = Directory("${Properties.appDraftDirectory.path}/$md5Str.fkp");
  }

  late final Directory _workspace;
  final _editor = MediaEditorChannel();
  int _currentIndex = -1;
  LayerState _layerState = LayerState.camera;
  Widget? curSecondPanel;

  @override
  void didChangeAppLifecycleState(AppLifecycleState state) {
    switch (state) {
      case AppLifecycleState.resumed:
        Logcat.debug('AppLifecycleState=$state');
        //TODO
        // _editor.start();
        // _editor.attachWindow();
        if (_layerState != LayerState.camera) {
          _layerState = LayerState.idle;
          _checkEmptyLayers();
        }
      case AppLifecycleState.inactive:
        Logcat.debug('AppLifecycleState=$state');
        // _editor.closeCamera();
    //TODO
        // _editor.detachWindow();
        // _editor.stop();
      case AppLifecycleState.detached:
        Logcat.debug('AppLifecycleState=$state');
        //TODO
        // _editor.destroy();
        _workspace.deleteSync(recursive: true);
      case AppLifecycleState.paused:
        Logcat.debug('AppLifecycleState=$state');
      case AppLifecycleState.hidden:
        Logcat.debug('AppLifecycleState=$state');
    }
  }

  void _checkEmptyLayers() {
    //TODO
    // _editor.getLayerSize().then((size) => setState(() {
    //       Logcat.debug('_checkEmptyLayers getLayerSize=$size');
    //       if (size == 0) {
    //         if (!widget.fixedCameraPage) {
    //           _layerState = LayerState.empty;
    //         }
    //       } else {
    //         if (_layerState != LayerState.camera) {
    //           _layerState = LayerState.editor;
    //         }
    //       }
    //     }));
  }

  void pickImage() {
    SystemUtils.pickImage(context, (file) {
      if (file != null) {
        _editor.convertImage(file.path).then((path) {
          //TODO
          // _editor.newLayerWithFile(File(path));
          // _editor.notifyRender();
          _checkEmptyLayers();
        });
      }
    });
  }

  @override
  void initState() {
    super.initState();
    Logcat.debug('initState, mounted: $mounted');
    WidgetsBinding.instance.addObserver(this);
    _checkEmptyLayers();
    //TODO
    // _editor.create();
    // _editor.start();
    // _editor.attachWindow();
  }

  @override
  void dispose() {
    super.dispose();
    Logcat.debug('dispose, mounted: $mounted');
    WidgetsBinding.instance.removeObserver(this);
  }

  @override
  Widget build(BuildContext context) {
    final partingLine = Container(
      height: curSecondPanel != null ? 1 : 0,
      color: Colors.grey.shade900,
    );
    if (_layerState == LayerState.idle) {
      return const Scaffold(backgroundColor: Colors.black);
    }
    if (_layerState == LayerState.empty) {
      return Scaffold(
        backgroundColor: Colors.black,
        body: Align(
          alignment: Alignment.center,
          child: Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              TextButton(
                  onPressed: pickImage,
                  child: const Text("相册",
                      style: TextStyle(
                        color: Colors.white,
                        fontSize: 20,
                        fontStyle: FontStyle.italic,
                      ))),
              TextButton(
                  onPressed: () {
                    setState(() {
                      _layerState = LayerState.camera;
                    });
                  },
                  child: const Text("相机",
                      style: TextStyle(
                        color: Colors.white,
                        fontSize: 20,
                        fontStyle: FontStyle.italic,
                      ))),
            ],
          ),
        ),
      );
    }
    if (_layerState == LayerState.camera) {
      return Scaffold(
        backgroundColor: Colors.transparent,
        body: CameraShotPage(
          !widget.fixedCameraPage,
          () {
            _layerState = LayerState.idle;
            _checkEmptyLayers();
          },
          (layerId) {
            _layerState = LayerState.idle;
            _checkEmptyLayers();
          },
        ),
      );
    }
    final mainBar = BottomNavigationBarV2(
      currentIndex: _currentIndex,
      onTap: (int i) => setState(() {
        if (_currentIndex == i) {
          _currentIndex = -1;
          curSecondPanel = null;
        } else {
          _currentIndex = i;
          curSecondPanel = _createPanelByIndex(i);
        }
      }),
      items: _MAIN_TABS,
      backgroundColor: Colors.black,
      unselectedItemColor: Colors.white,
      elevation: 0,
    );
    return Scaffold(
      backgroundColor: Colors.transparent,
      bottomNavigationBar: Container(
        color: Colors.black,
        child: Column(
          mainAxisSize: MainAxisSize.min,
          children: [partingLine, mainBar],
        ),
      ),
      bottomSheet: curSecondPanel != null
          ? Container(
              color: Colors.black,
              height: 180,
              child: curSecondPanel!,
            )
          : null,
    );
  }

  Widget _createPanelByIndex(int index) {
    switch (index) {
      case 0:
        return LayerListPage();
    }
    return Container();
  }

  final _MAIN_TABS = [
    BottomNavigationBarV2Item(
      icon: const Icon(Icons.layers_outlined),
      label: "图层",
      creator: () => null,
    ),
    BottomNavigationBarV2Item(
      icon: const Icon(Icons.exposure_outlined),
      label: "调整",
      creator: () => null,
    ),
    BottomNavigationBarV2Item(
      icon: const Icon(Icons.crop_outlined),
      label: "裁剪",
      creator: () => null,
    ),
    BottomNavigationBarV2Item(
      icon: const Icon(Icons.brush_outlined),
      label: "画笔",
      creator: () => null,
    ),
    BottomNavigationBarV2Item(
      icon: const Icon(Icons.text_fields_outlined),
      label: "文字",
      creator: () => null,
    ),
  ];
}
