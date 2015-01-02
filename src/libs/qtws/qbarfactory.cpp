#include "qbarfactory.h"
#include "qtwsapi.h"
#include <QDebug>
#include <QDateTime>
#include <QMap>

#define SC(x) connect(api, &QTwsAPI::x, this, &QBarFactory::x)

namespace TWS {


QBarFactory::QBarFactory(QObject *parent)
{
}

void QBarFactory::monitor(QTwsAPI *api)
{
    SC(connectionClosed);
    SC(connectionEstablish);
    SC(serverLost);
    SC(serverRestore);
    SC(marketDataLost);
    SC(marketDataRestore);
    SC(historicalDataLost);
    SC(historicalDataRestore);

    SC(barPrice);
    SC(historicalData);
}

void QBarFactory::connectionClosed()
{
    qDebug() << Q_FUNC_INFO;
}

void QBarFactory::connectionEstablish()
{
    qDebug() << Q_FUNC_INFO;
}

void QBarFactory::serverLost()
{
    qDebug() << Q_FUNC_INFO;
}

void QBarFactory::serverRestore(bool needReSubscribe)
{
    qDebug() << Q_FUNC_INFO << needReSubscribe;
}

void QBarFactory::marketDataLost()
{
    qDebug() << Q_FUNC_INFO;
}

void QBarFactory::marketDataRestore()
{
    qDebug() << Q_FUNC_INFO;
}

void QBarFactory::historicalDataLost()
{
    qDebug() << Q_FUNC_INFO;
}

void QBarFactory::historicalDataRestore()
{
    qDebug() << Q_FUNC_INFO;
}

void QBarFactory::generateBar(const QList<Internal::TickData>& list)
{
    double open = 0.0, high = 0.0, low = DBL_MAX, close = 0.0;
    int size = 0;

    if (!list.isEmpty()) {
        open = list.first().m_price;
        close = list.last().m_price;
    }
    foreach(const Internal::TickData& tick, list) {
        if (tick.m_price > high)
            high = tick.m_price;
        if (tick.m_price < low)
            low = tick.m_price;
        size++;
    }

    if (!list.isEmpty()) {
        emit bar(open, high, low, close, size);

        QDateTime tm = QDateTime::fromMSecsSinceEpoch(list.first().m_time);
        qDebug() << tm << " " << open << " " << high << " " << low << " " << close << " " << size;
    }
}

void QBarFactory::barPrice(long reqId, double price, int vol)
{
    Internal::TickData tick(QDateTime::currentMSecsSinceEpoch(),price, vol);

    //qDebug() << "tick:" << tick.m_time << price;

    QList<Internal::TickData> *list = m_ticks.value(tick.m_time/60000);
    if (list == NULL) {
        list = new QList<Internal::TickData>();
        m_ticks.insert(tick.m_time/60000, list);
        QList<Internal::TickData> *prevList = m_ticks.value(tick.m_time/60000 -1);
        if (prevList != NULL)
            generateBar(*prevList);
    }
    list->append(tick);
}

void QBarFactory::historicalData(long reqId, const QString& date, double open, double high,
                    double low, double close, int volume, int barCount, double WAP, int hasGaps)
{
    qDebug() << reqId << date << open << high << low << close << volume << barCount << WAP << hasGaps;
}

} // namespace TWS
