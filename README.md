[![License](https://img.shields.io/badge/License-GPL%20v2-blue.svg)](https://gitlab.xfce.org/thunar-plugins/thunar-disk-usage-plugin/-/blob/master/COPYING)

thunar-disk-usage-plugin
========================

 The Thunar Disk Usage Plugin (thunar-disk-usage-plugin) shows what takes up the space in a folder, as a ring chart like the one in GNOME's Disk Usage Analyzer (Baobab).

It adds:

  * A "Disk Usage" page to the file properties dialog of a folder,
  * A "Show Disk Usage" item (Ctrl+Alt+U) to the context menu of a folder and of the folder background, which opens the same view in a window of its own.

Each ring is one level of sub folders. A click on a segment opens that folder in the chart, a click into the middle goes back up. Sizes are the space used on the disk (like `du`), hard links are counted once, and the scan stays on the file system of the folder (like `du -x`).

----

### Changelog

See [NEWS](https://gitlab.xfce.org/thunar-plugins/thunar-disk-usage-plugin/-/blob/master/NEWS) for details on changes and fixes made in the current release.

### Source Code Repository

[Thunar-disk-usage-plugin source code](https://gitlab.xfce.org/thunar-plugins/thunar-disk-usage-plugin)

### Download a Release Tarball

[Thunar-disk-usage-plugin archive](https://archive.xfce.org/src/thunar-plugins/thunar-disk-usage-plugin)
    or
[Thunar-disk-usage-plugin tags](https://gitlab.xfce.org/thunar-plugins/thunar-disk-usage-plugin/-/tags)

### Installation

From source:

    % cd thunar-disk-usage-plugin
    % meson setup build
    % meson compile -C build
    % meson install -C build

From release tarball:

    % tar xf thunar-disk-usage-plugin-<version>.tar.xz
    % cd thunar-disk-usage-plugin-<version>
    % meson setup build
    % meson compile -C build
    % meson install -C build

`-Dtests=true` also builds `tests/du-scan-test` and `tests/du-view-test`, which run the scanner and the view without Thunar.

### Uninstallation

    % ninja uninstall -C build

### Reporting Bugs

Visit the [issues](https://gitlab.xfce.org/thunar-plugins/thunar-disk-usage-plugin/-/issues) page to view currently open bug reports and to report new bugs.

### Credits

The ring chart is ported to C and GTK 3 from `baobab-chart.vala` and `baobab-ringschart.vala` of [Baobab](https://gitlab.gnome.org/GNOME/baobab) 49.1 (Copyright © 2006–2008 Igalia, © 2013 Stefano Facchini), GPL-2.0-or-later.
