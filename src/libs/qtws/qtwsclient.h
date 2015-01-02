#ifndef QTWSCLIENT_H
#define QTWSCLIENT_H

#include <QObject>

#include "qtwsapi.h"

namespace TWS {

namespace Internal {
class QTwsClientPrivate;
}

class QTwsClient : public QObject
{
    Q_OBJECT
public:
    explicit QTwsClient(QObject *parent = 0);

signals:


private:
    Internal::QTwsClientPrivate *d;
};

}  // namespace TWS
#endif // QTWSCLIENT_H
