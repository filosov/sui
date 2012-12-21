#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <QtCore/qglobal.h>

#if defined(SCSTORE_PLUGIN)
#  define SCSCTORESHARED_EXPORT
#elif defined(SCSTORE_LIBRARY)
#  define SCSTORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define SCSTORESHARED_EXPORT Q_DECL_IMPORT
#endif


#endif // DEFINES_HPP
