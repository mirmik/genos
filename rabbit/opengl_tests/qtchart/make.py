#!/usr/bin/env python3

import licant

licant.include("rabbit", "../../rabbit.g.py")

licant.cxx_application("qtchart",
	sources= ["main.cpp"],
	mdepends = ["rabbit", "rabbit.qt_opengl"],

	libs=["igris", "nos", "crow", 
		"Qt5Core", 
		"Qt5OpenGL", 
		"Qt5Widgets", 
		"Qt5Charts", 
		"Qt5Gui",
		"GL"
	],
	include_paths = ["../..", "."],

	cc_flags = "-DQT_CHARTS_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -g -O0 " 
	"-I/usr/include/x86_64-linux-gnu/qt5/QtCharts -I/usr/include/x86_64-linux-gnu/qt5 "
	"-I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5 "
	"-I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5 "
	"-I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/x86_64-linux-gnu/qt5 "
	"-fPIC -pedantic -Werror=all -Werror=extra",

	ld_flags = "-DQT_CHARTS_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -g -O0 " 
	"-I/usr/include/x86_64-linux-gnu/qt5/QtCharts -I/usr/include/x86_64-linux-gnu/qt5 "
	"-I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5 "
	"-I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5 "
	"-I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/x86_64-linux-gnu/qt5 "
	"-fPIC -pedantic -Werror=all",

	cxx_flags = "-DQT_CHARTS_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -g -O0 " 
	"-I/usr/include/x86_64-linux-gnu/qt5/QtCharts -I/usr/include/x86_64-linux-gnu/qt5 "
	"-I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5 "
	"-I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5 "
	"-I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/x86_64-linux-gnu/qt5 "
	"-I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5 "
	"-fPIC -pedantic -Werror=all",
)

licant.ex("qtchart")