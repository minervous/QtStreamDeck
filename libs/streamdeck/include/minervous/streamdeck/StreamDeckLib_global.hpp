#pragma once

#include <QtCore/QtCompilerDetection>

#if defined(STREAMDECKLIB_LIBRARY)
#	define STREAMDECKLIB_EXPORT Q_DECL_EXPORT
#else
#	define STREAMDECKLIB_EXPORT Q_DECL_IMPORT
#endif
