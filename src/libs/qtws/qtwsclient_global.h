#ifndef TWS_GLOBAL_H
#define TWS_GLOBAL_H

#include <qglobal.h>

#if defined(TWS_LIBRARY)
#  define TWS_EXPORT Q_DECL_EXPORT
#else
#  define TWS_EXPORT Q_DECL_IMPORT
#endif

#endif // TWS_GLOBAL_H
