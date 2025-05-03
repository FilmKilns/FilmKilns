import 'dart:ffi';

import 'package:flutter/material.dart';

typedef OpItemCallback = void Function(int);

class CameraOpItem {
  final String name;
  final IconData icon;

  const CameraOpItem(this.name, this.icon);
}

class CameraOpPanel extends StatefulWidget {
  final List<CameraOpItem> _items;
  final OpItemCallback? onItemPressed;

  const CameraOpPanel(this._items, this.onItemPressed, {super.key});

  @override
  State<CameraOpPanel> createState() => _CameraOpPanel();
}

class _CameraOpPanel extends State<CameraOpPanel> {
  @override
  Widget build(BuildContext context) {
    if (widget._items.isEmpty) {
      return Container();
    }
    return Container(
      margin: const EdgeInsets.only(top: 0),
      padding: const EdgeInsets.all(0),
      decoration: BoxDecoration(
        color: Colors.white.withOpacity(0.15),
        borderRadius: BorderRadius.circular(12),
      ),
      child: GridView.count(
        shrinkWrap: true,
        crossAxisCount: 4,
        mainAxisSpacing: 8,
        crossAxisSpacing: 8,
        childAspectRatio: 1.5,
        children: List.generate(widget._items.length, (index) {
          return GestureDetector(
            onTap: () {
              if (widget.onItemPressed != null) {
                widget.onItemPressed!(index);
              }
              setState(() {
                // buttonStates[type]![index] = !buttonStates[type]![index];
              });
            },
            child: Expanded(
              flex: 1,
              child: IconButton(
                onPressed: () {},
                style: ButtonStyle(
                    padding:
                    WidgetStateProperty.all(const EdgeInsets.all(0))),
                icon: Icon(widget._items[index].icon, color: Colors.white),
                iconSize: 26,
                splashColor: Colors.grey,
              ),
            ),
          );
        }),
      ),
    );
  }
}
