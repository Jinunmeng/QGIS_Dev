
// QGSCONFIG.H

#ifndef QGSCONFIG_H
#define QGSCONFIG_H

// Version must be specified according to
// <int>.<int>.<int>"
// or else upgrading old project file will not work
// reliably.
#define VERSION "2.18.18"

//used in vim src/core/qgis.cpp
//The way below should work but it resolves to a number like 0110 which the compiler treats as octal I think
//because debuggin it out shows the decimal number 72 which results in incorrect version status.
//As a short term fix I (Tim) am defining the version in top level cmake. It would be good to 
//reinstate this more generic approach below at some point though
//#define VERSION_INT 21818
#define VERSION_INT 21818
#define ABISYM(x) x ## 21818
//used in main.cpp and anywhere else where the release name is needed
#define RELEASE_NAME "Las Palmas"

#define QGIS_PLUGIN_SUBDIR "plugins"
#define QGIS_DATA_SUBDIR "."
#define QGIS_LIBEXEC_SUBDIR "."
#define QGIS_LIB_SUBDIR "lib"
#define CMAKE_INSTALL_PREFIX "D:/QGIS_Win_Qt5.9_VS2015_x64_qgis2.18_ltr/Debug"
#define CMAKE_SOURCE_DIR "D:/QGIS/qgis-2.18.18"

#define QSCINTILLA_VERSION_STR "2.10.1"

#if defined( __APPLE__ )
//used by Mac to find system or bundle resources relative to amount of bundling
#define QGIS_MACAPP_BUNDLE 
#endif

#define QT_PLUGINS_DIR "C:/Qt/Qt5.9.0/5.9/msvc2015_64/plugins"
#define OSG_PLUGINS_PATH ""

/* #undef USING_NMAKE */

/* #undef USING_NINJA */

#define HAVE_POSTGRESQL

/* #undef HAVE_ORACLE */

#define HAVE_TOUCH

/* #undef HAVE_OSGEARTHQT */

/* #undef SERVER_SKIP_ECW */

/* #undef HAVE_SERVER_PYTHON_PLUGINS */

/* #undef ENABLE_MODELTEST */

/* #undef WITH_ARCGIS */

#endif

