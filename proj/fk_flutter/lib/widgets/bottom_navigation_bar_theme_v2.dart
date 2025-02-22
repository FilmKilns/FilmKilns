// Copyright 2014 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import 'dart:ui' show lerpDouble;

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart' show WidgetStateProperty, Theme;
import 'package:flutter/widgets.dart';

import 'bottom_navigation_bar_v2.dart';

// Examples can assume:
// late BuildContext context;

/// Defines default property values for descendant [BottomNavigationBar]
/// widgets.
///
/// Descendant widgets obtain the current [BottomNavigationBarThemeData] object
/// using `BottomNavigationBarTheme.of(context)`. Instances of
/// [BottomNavigationBarThemeData] can be customized with
/// [BottomNavigationBarThemeData.copyWith].
///
/// Typically a [BottomNavigationBarThemeData] is specified as part of the
/// overall [Theme] with [ThemeData.bottomNavigationBarTheme].
///
/// All [BottomNavigationBarThemeData] properties are `null` by default. When
/// null, the [BottomNavigationBar]'s build method provides defaults.
///
/// See also:
///
///  * [ThemeData], which describes the overall theme information for the
///    application.
@immutable
class BottomNavigationBarV2ThemeData with Diagnosticable {
  /// Creates a theme that can be used for [ThemeData.bottomNavigationBarTheme].
  const BottomNavigationBarV2ThemeData({
    this.backgroundColor,
    this.elevation,
    this.selectedIconTheme,
    this.unselectedIconTheme,
    this.selectedItemColor,
    this.unselectedItemColor,
    this.selectedLabelStyle,
    this.unselectedLabelStyle,
    this.showSelectedLabels,
    this.showUnselectedLabels,
    this.type,
    this.enableFeedback,
    this.landscapeLayout,
    this.mouseCursor,
  });

  /// The color of the [BottomNavigationBar] itself.
  ///
  /// See [BottomNavigationBar.backgroundColor].
  final Color? backgroundColor;

  /// The z-coordinate of the [BottomNavigationBar].
  ///
  /// See [BottomNavigationBar.elevation].
  final double? elevation;

  /// The size, opacity, and color of the icon in the currently selected
  /// [BottomNavigationBarItem.icon].
  ///
  /// If [BottomNavigationBar.selectedIconTheme] is non-null on the widget,
  /// the whole [IconThemeData] from the widget will be used over this
  /// [selectedIconTheme].
  ///
  /// See [BottomNavigationBar.selectedIconTheme].
  final IconThemeData? selectedIconTheme;

  /// The size, opacity, and color of the icon in the currently unselected
  /// [BottomNavigationBarItem.icon]s.
  ///
  /// If [BottomNavigationBar.unselectedIconTheme] is non-null on the widget,
  /// the whole [IconThemeData] from the widget will be used over this
  /// [unselectedIconTheme].
  ///
  /// See [BottomNavigationBar.unselectedIconTheme].
  final IconThemeData? unselectedIconTheme;

  /// The color of the selected [BottomNavigationBarItem.icon] and
  /// [BottomNavigationBarItem.label].
  ///
  /// See [BottomNavigationBar.selectedItemColor].
  final Color? selectedItemColor;

  /// The color of the unselected [BottomNavigationBarItem.icon] and
  /// [BottomNavigationBarItem.label]s.
  ///
  /// See [BottomNavigationBar.unselectedItemColor].
  final Color? unselectedItemColor;

  /// The [TextStyle] of the [BottomNavigationBarItem] labels when they are
  /// selected.
  ///
  /// See [BottomNavigationBar.selectedLabelStyle].
  final TextStyle? selectedLabelStyle;

  /// The [TextStyle] of the [BottomNavigationBarItem] labels when they are not
  /// selected.
  ///
  /// See [BottomNavigationBar.unselectedLabelStyle].
  final TextStyle? unselectedLabelStyle;

  /// Whether the labels are shown for the selected [BottomNavigationBarItem].
  ///
  /// See [BottomNavigationBar.showSelectedLabels].
  final bool? showSelectedLabels;

  /// Whether the labels are shown for the unselected [BottomNavigationBarItem]s.
  ///
  /// See [BottomNavigationBar.showUnselectedLabels].
  final bool? showUnselectedLabels;

  /// Defines the layout and behavior of a [BottomNavigationBar].
  ///
  /// See [BottomNavigationBar.type].
  final BottomNavigationBarV2Type? type;

  /// If specified, defines the feedback property for [BottomNavigationBar].
  ///
  /// If [BottomNavigationBar.enableFeedback] is provided, [enableFeedback] is ignored.
  final bool? enableFeedback;

  /// If non-null, overrides the [BottomNavigationBar.landscapeLayout] property.
  final BottomNavigationBarV2LandscapeLayout? landscapeLayout;

  /// If specified, overrides the default value of [BottomNavigationBar.mouseCursor].
  final WidgetStateProperty<MouseCursor?>? mouseCursor;

  /// Creates a copy of this object but with the given fields replaced with the
  /// new values.
  BottomNavigationBarV2ThemeData copyWith({
    Color? backgroundColor,
    double? elevation,
    IconThemeData? selectedIconTheme,
    IconThemeData? unselectedIconTheme,
    Color? selectedItemColor,
    Color? unselectedItemColor,
    TextStyle? selectedLabelStyle,
    TextStyle? unselectedLabelStyle,
    bool? showSelectedLabels,
    bool? showUnselectedLabels,
    BottomNavigationBarV2Type? type,
    bool? enableFeedback,
    BottomNavigationBarV2LandscapeLayout? landscapeLayout,
    WidgetStateProperty<MouseCursor?>? mouseCursor,
  }) {
    return BottomNavigationBarV2ThemeData(
      backgroundColor: backgroundColor ?? this.backgroundColor,
      elevation: elevation ?? this.elevation,
      selectedIconTheme: selectedIconTheme ?? this.selectedIconTheme,
      unselectedIconTheme: unselectedIconTheme ?? this.unselectedIconTheme,
      selectedItemColor: selectedItemColor ?? this.selectedItemColor,
      unselectedItemColor: unselectedItemColor ?? this.unselectedItemColor,
      selectedLabelStyle: selectedLabelStyle ?? this.selectedLabelStyle,
      unselectedLabelStyle: unselectedLabelStyle ?? this.unselectedLabelStyle,
      showSelectedLabels: showSelectedLabels ?? this.showSelectedLabels,
      showUnselectedLabels: showUnselectedLabels ?? this.showUnselectedLabels,
      type: type ?? this.type,
      enableFeedback: enableFeedback ?? this.enableFeedback,
      landscapeLayout: landscapeLayout ?? this.landscapeLayout,
      mouseCursor: mouseCursor ?? this.mouseCursor,
    );
  }

  /// Linearly interpolate between two [BottomNavigationBarThemeData].
  ///
  /// {@macro dart.ui.shadow.lerp}
  static BottomNavigationBarV2ThemeData lerp(BottomNavigationBarV2ThemeData? a, BottomNavigationBarV2ThemeData? b, double t) {
    if (identical(a, b) && a != null) {
      return a;
    }
    return BottomNavigationBarV2ThemeData(
      backgroundColor: Color.lerp(a?.backgroundColor, b?.backgroundColor, t),
      elevation: lerpDouble(a?.elevation, b?.elevation, t),
      selectedIconTheme: IconThemeData.lerp(a?.selectedIconTheme, b?.selectedIconTheme, t),
      unselectedIconTheme: IconThemeData.lerp(a?.unselectedIconTheme, b?.unselectedIconTheme, t),
      selectedItemColor: Color.lerp(a?.selectedItemColor, b?.selectedItemColor, t),
      unselectedItemColor: Color.lerp(a?.unselectedItemColor, b?.unselectedItemColor, t),
      selectedLabelStyle: TextStyle.lerp(a?.selectedLabelStyle, b?.selectedLabelStyle, t),
      unselectedLabelStyle: TextStyle.lerp(a?.unselectedLabelStyle, b?.unselectedLabelStyle, t),
      showSelectedLabels: t < 0.5 ? a?.showSelectedLabels : b?.showSelectedLabels,
      showUnselectedLabels: t < 0.5 ? a?.showUnselectedLabels : b?.showUnselectedLabels,
      type: t < 0.5 ? a?.type : b?.type,
      enableFeedback: t < 0.5 ? a?.enableFeedback : b?.enableFeedback,
      landscapeLayout: t < 0.5 ? a?.landscapeLayout : b?.landscapeLayout,
      mouseCursor: t < 0.5 ? a?.mouseCursor : b?.mouseCursor,
    );
  }

  @override
  int get hashCode => Object.hash(
    backgroundColor,
    elevation,
    selectedIconTheme,
    unselectedIconTheme,
    selectedItemColor,
    unselectedItemColor,
    selectedLabelStyle,
    unselectedLabelStyle,
    showSelectedLabels,
    showUnselectedLabels,
    type,
    enableFeedback,
    landscapeLayout,
    mouseCursor,
  );

  @override
  bool operator ==(Object other) {
    if (identical(this, other)) {
      return true;
    }
    if (other.runtimeType != runtimeType) {
      return false;
    }
    return other is BottomNavigationBarV2ThemeData
        && other.backgroundColor == backgroundColor
        && other.elevation == elevation
        && other.selectedIconTheme == selectedIconTheme
        && other.unselectedIconTheme == unselectedIconTheme
        && other.selectedItemColor == selectedItemColor
        && other.unselectedItemColor == unselectedItemColor
        && other.selectedLabelStyle == selectedLabelStyle
        && other.unselectedLabelStyle == unselectedLabelStyle
        && other.showSelectedLabels == showSelectedLabels
        && other.showUnselectedLabels == showUnselectedLabels
        && other.type == type
        && other.enableFeedback == enableFeedback
        && other.landscapeLayout == landscapeLayout
        && other.mouseCursor == mouseCursor;
  }

  @override
  void debugFillProperties(DiagnosticPropertiesBuilder properties) {
    super.debugFillProperties(properties);
    properties.add(ColorProperty('backgroundColor', backgroundColor, defaultValue: null));
    properties.add(DoubleProperty('elevation', elevation, defaultValue: null));
    properties.add(DiagnosticsProperty<IconThemeData>('selectedIconTheme', selectedIconTheme, defaultValue: null));
    properties.add(DiagnosticsProperty<IconThemeData>('unselectedIconTheme', unselectedIconTheme, defaultValue: null));
    properties.add(ColorProperty('selectedItemColor', selectedItemColor, defaultValue: null));
    properties.add(ColorProperty('unselectedItemColor', unselectedItemColor, defaultValue: null));
    properties.add(DiagnosticsProperty<TextStyle>('selectedLabelStyle', selectedLabelStyle, defaultValue: null));
    properties.add(DiagnosticsProperty<TextStyle>('unselectedLabelStyle', unselectedLabelStyle, defaultValue: null));
    properties.add(DiagnosticsProperty<bool>('showSelectedLabels', showSelectedLabels, defaultValue: null));
    properties.add(DiagnosticsProperty<bool>('showUnselectedLabels', showUnselectedLabels, defaultValue: null));
    properties.add(DiagnosticsProperty<BottomNavigationBarV2Type>('type', type, defaultValue: null));
    properties.add(DiagnosticsProperty<bool>('enableFeedback', enableFeedback, defaultValue: null));
    properties.add(DiagnosticsProperty<BottomNavigationBarV2LandscapeLayout>('landscapeLayout', landscapeLayout, defaultValue: null));
    properties.add(DiagnosticsProperty<WidgetStateProperty<MouseCursor?>>('mouseCursor', mouseCursor, defaultValue: null));
  }
}

/// Applies a bottom navigation bar theme to descendant [BottomNavigationBar]
/// widgets.
///
/// Descendant widgets obtain the current theme's [BottomNavigationBarTheme]
/// object using [BottomNavigationBarTheme.of]. When a widget uses
/// [BottomNavigationBarTheme.of], it is automatically rebuilt if the theme
/// later changes.
///
/// A bottom navigation theme can be specified as part of the overall Material
/// theme using [ThemeData.bottomNavigationBarTheme].
///
/// See also:
///
///  * [BottomNavigationBarThemeData], which describes the actual configuration
///    of a bottom navigation bar theme.
class BottomNavigationBarV2Theme extends InheritedWidget {
  /// Constructs a bottom navigation bar theme that configures all descendant
  /// [BottomNavigationBar] widgets.
  const BottomNavigationBarV2Theme({
    super.key,
    required this.data,
    required super.child,
  });

  /// The properties used for all descendant [BottomNavigationBar] widgets.
  final BottomNavigationBarV2ThemeData data;

  /// Returns the configuration [data] from the closest
  /// [BottomNavigationBarTheme] ancestor. If there is no ancestor, it returns
  /// [ThemeData.bottomNavigationBarTheme]. Applications can assume that the
  /// returned value will not be null.
  ///
  /// Typical usage is as follows:
  ///
  /// ```dart
  /// BottomNavigationBarThemeData theme = BottomNavigationBarTheme.of(context);
  /// ```
  static BottomNavigationBarV2ThemeData of(BuildContext context) {
    final BottomNavigationBarV2Theme? bottomNavTheme = context
        .dependOnInheritedWidgetOfExactType<BottomNavigationBarV2Theme>();
    final data = Theme.of(context).bottomNavigationBarTheme;
    return bottomNavTheme?.data ??
        BottomNavigationBarV2ThemeData(
          backgroundColor: data.backgroundColor,
          elevation: data.elevation,
          selectedIconTheme: data.selectedIconTheme,
          unselectedIconTheme: data.unselectedIconTheme,
          selectedItemColor: data.selectedItemColor,
          unselectedItemColor: data.unselectedItemColor,
          selectedLabelStyle: data.selectedLabelStyle,
          unselectedLabelStyle: data.unselectedLabelStyle,
          showSelectedLabels: data.showSelectedLabels,
          showUnselectedLabels: data.showUnselectedLabels,
          type: BottomNavigationBarV2Type.values[data.type != null ? data.type!.index: 0],
          enableFeedback: data.enableFeedback,
          landscapeLayout: BottomNavigationBarV2LandscapeLayout.values[data.landscapeLayout != null ? data.landscapeLayout!.index : 0],
          mouseCursor: data.mouseCursor,
        );
  }

  @override
  bool updateShouldNotify(BottomNavigationBarV2Theme oldWidget) => data != oldWidget.data;
}
