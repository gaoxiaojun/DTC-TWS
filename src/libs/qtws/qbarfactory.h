#ifndef QBARFACTORY_H
#define QBARFACTORY_H

#include <QObject>

#include "twscommon.h"
#include "twscommissionreport.h"
#include "twscontract.h"
#include "twsexecution.h"
#include "twsstring.h"
#include "twsorder.h"
#include "twsorderstate.h"
#include "twsscannersubscription.h"
#include "twstagvalue.h"
#include "twssocketclienterrors.h"

#include <QMap>
#include <QList>

namespace TWS {
class QTwsAPI;

namespace Internal {
class TickData {
public:
    enum TickDataType {
        TICK_ASK,
        TICK_BID,
        TICK_TRADE
    };

    TickData(qint64 time, double price, int size)
        : m_time(time), m_price(price), m_size(size) {}

    qint64 m_time;
    double m_price;
    int m_size;
};

}

class QBarFactory : public QObject
{
    Q_OBJECT
public:
    explicit QBarFactory(QObject *parent);

    void monitor(QTwsAPI *api);

signals:
    void bar(double open, double high, double low, double close, int tickVol);

public slots:
    void connectionClosed();
    void connectionEstablish();
    void serverLost();
    void serverRestore(bool needReSubscribe);
    void marketDataLost();
    void marketDataRestore();
    void historicalDataLost();
    void historicalDataRestore();

    void historicalData(long reqId, const QString& date, double open, double high,
                        double low, double close, int volume, int barCount, double WAP, int hasGaps);

    void barPrice(long reqId, double price, int vol);

protected:
    void generateBar(const QList<Internal::TickData> &list);
private:
    QMap<qint64, QList<Internal::TickData>* > m_ticks;
};

}

#endif // QBARFACTORY_H
