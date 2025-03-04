import 'package:flutter/material.dart';

class CameraSheetTop extends StatefulWidget {
  const CameraSheetTop({super.key});

  @override
  State<CameraSheetTop> createState() => _CameraSheetTop();
}

class _CameraSheetTop extends State<CameraSheetTop> {
  Widget _wrap(Icon icon) {
    return SizedBox(
      width: 26,
      height: 26,
      child: IconButton(
        onPressed: () {},
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
    return Container(
      margin: const EdgeInsets.fromLTRB(0, 0, 0, 0),
      child: GridView(
        shrinkWrap: true,
        physics: const NeverScrollableScrollPhysics(),
        gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
            crossAxisCount: 3,
            mainAxisExtent: 32),
        children: <Widget>[
          _wrap(const Icon(Icons.flash_auto, color: Colors.white)),
          _wrap(const Icon(Icons.camera, color: Colors.white)),
          _wrap(const Icon(Icons.info_outline, color: Colors.white)),
        ],
      ),
    );
  }
}
