#ifndef STREAMDECKLIB_GLOBAL_H
#define STREAMDECKLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(STREAMDECKLIB_LIBRARY)
#  define STREAMDECKLIB_EXPORT Q_DECL_EXPORT
#else
#  define STREAMDECKLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // STREAMDECKLIB_GLOBAL_H
