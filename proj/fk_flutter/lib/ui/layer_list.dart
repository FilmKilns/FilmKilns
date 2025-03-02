import 'package:fk_flutter/utils/logcat.dart';
import 'package:flutter/material.dart';
import 'dart:io';
import 'package:fk_flutter/fk/entity/FkImageLayer.pb.dart';
import 'package:fk_flutter/utils/system_utils.dart';
import 'package:fk_flutter/channels/media_editor_channel.dart';

class LayerListPage extends StatefulWidget {
  const LayerListPage({super.key});

  @override
  State<LayerListPage> createState() => _LayerListPageState();
}

class _LayerListPageState extends State<LayerListPage> with WidgetsBindingObserver {
  final _editor = MediaEditorChannel();
  List<FkImageLayer>? layers;
  static const double _ITEM_SIZE = 30;
  static const double _ITEM_TEXT_SIZE = 14;
  static const _MAIN_EDGE = EdgeInsets.fromLTRB(0, 6, 0, 6);

  @override
  void didChangeAppLifecycleState(AppLifecycleState state) {
    switch (state) {
      case AppLifecycleState.resumed:
      //TODO
        // _editor.getPictureModel().then((value) => setState(() {
        //       layers = value?.layers;
        //     }));
      case AppLifecycleState.paused:
      case AppLifecycleState.detached:
      case AppLifecycleState.inactive:
      case AppLifecycleState.hidden:
    }
  }

  Widget createItem(FkImageLayer layer) {
    return Text("${layer.size.width}x${layer.size.height}",
        style: const TextStyle(color: Colors.black, fontSize: _ITEM_TEXT_SIZE));
  }

  Widget wrapItem(Widget index, Widget icon, Widget info) {
    var mainRadius = BorderRadius.circular(_ITEM_SIZE / 2);
    var decoration =
        BoxDecoration(color: Colors.white, borderRadius: mainRadius);
    final indexIcon = Container(
      height: _ITEM_SIZE - 4,
      width: _ITEM_SIZE - 4,
      margin: const EdgeInsets.fromLTRB(2, 0, 2, 0),
      alignment: Alignment.center,
      decoration: BoxDecoration(
          color: Colors.black,
          borderRadius: BorderRadius.circular((_ITEM_SIZE - 4) / 2)),
      child: index,
    );
    return Container(
      height: _ITEM_SIZE,
      alignment: Alignment.centerLeft,
      decoration: decoration,
      margin: _MAIN_EDGE,
      child: Row(
        children: [
          indexIcon,
          Container(
            alignment: Alignment.centerLeft,
            child: Row(children: [
              Container(
                padding: const EdgeInsets.fromLTRB(2, 0, 4, 0),
                child: icon,
              ),
              info
            ]),
          ),
        ],
      ),
    );
  }

  Widget createHeaderButton(IconData icon, VoidCallback? calback) {
    return IconButton(
      padding: const EdgeInsets.all(2),
      iconSize: 24,
      onPressed: calback,
      icon: AspectRatio(
        aspectRatio: 1.0,
        child: Icon(
          icon,
          color: Colors.white,
        ),
      ),
    );
  }

  void pickImage() {
    SystemUtils.pickImage(context, (file) {
      if (file != null) {
        _editor.convertImage(file.path).then((path) {
          //TODO
          // _editor.newLayerWithFile(File(path));
          // _editor.notifyRender();
          // _editor.getPictureModel().then((value) => setState(() {
          //       layers = value?.layers;
          //       Navigator.of(context).pop();
          //     }));
        });
      }
    });
  }

  void showHist() {
    //TODO
    // histPlugin = FkImageHistPlugin();
    // if (histPlugin != null) {
    //   histPlugin?.updateParams({});
    //   histPluginLayerId = widget._editor.newLayerWithPlugin(histPlugin!);
    //   if (layers?.isNotEmpty == true && histPluginLayerId > 0) {
    //     widget._editor.setProjectionLayer(histPluginLayerId, layers![0].id);
    //     Logcat.debug('showHist: $histPluginLayerId, ${layers![0].id}');
    //     widget._editor.notifyRender();
    //   }
    // }
  }

  void undo() {
    //TODO
    // _editor.notifyRender();
  }

  void redo() {

  }

  Widget createHeader() {
    return Container(
      color: Colors.grey.shade900,
      height: _ITEM_SIZE + 8,
      child: Row(
        mainAxisAlignment: MainAxisAlignment.end,
        children: [
          createHeaderButton(Icons.insert_chart_outlined, showHist),
          createHeaderButton(Icons.add_circle, pickImage),
          createHeaderButton(Icons.undo, undo),
          createHeaderButton(Icons.redo, redo),
        ],
      ),
    );
  }

  @override
  void initState() {
    super.initState();
    WidgetsBinding.instance.addObserver(this);
    //TODO
    // _editor.getPictureModel().then((value) => setState(() {
    //   if (value?.layers == null) {
    //     layers = [];
    //   } else {
    //     layers = value?.layers;
    //   }
    // }));
  }

  @override
  void dispose() {
    super.dispose();
    WidgetsBinding.instance.removeObserver(this);
  }

  @override
  Widget build(BuildContext context) {
    if (layers == null) {
      return Container(
        width: double.infinity,
        color: Colors.black,
      );
    }
    const iconImage = Icon(Icons.image_outlined);
    var widgets = layers
        ?.map((layer) => wrapItem(
            Text(
              "${layer.id}",
              style: const TextStyle(color: Colors.white, fontSize: _ITEM_TEXT_SIZE),
            ),
            iconImage,
            createItem(layer)))
        .toList();
    return Container(
      child: Column(
        children: [
          createHeader(),
          ListView(
            shrinkWrap: true,
            scrollDirection: Axis.vertical,
            padding: const EdgeInsets.all(8),
            children: widgets!,
          ),
        ],
      ),
    );
  }
}
