
#include "qtwsapi.h"
#include "qtwsapilogging.h"
#include "qbarfactory.h"
#include <QCoreApplication>
#include <QTimer>
#include <QDateTime>
// TOOD
// 注册MetaType
// 缺省设置Log所有信号
//

int main(int argc, char **argv)
{
    qSetMessagePattern("[%{if-debug}D%{endif}%{if-warning}W%{endif}"
                           "%{if-critical}C%{endif}%{if-fatal}F%{endif}]"
                           "%{if-category} [%{category}]:%{endif} %{message}");
    QCoreApplication app(argc, argv);

    TWS::registerTwsMetaType();

    TWS::QTwsAPI *api = new TWS::QTwsAPI(0);
    TWS::TwsLogging *logger = new TWS::TwsLogging(0);
    TWS::QBarFactory *factory = new TWS::QBarFactory(0);

    //logger->monitor(api);
    factory->monitor(api);

    api->eConnect("127.0.0.1", 7496);

    /*QTimer *timer = new QTimer(0);

    QObject::connect(timer, &QTimer::timeout, [=]() {
        TWS::Contract contract;
        contract.symbol = "EUR";
        contract.currency = "USD";
        contract.secType = "CASH";
        contract.exchange = "IDEALPRO";

        long hid = api->nextTickerId();
        //qint64 epoc = QDateTime::currentMSecsSinceEpoch() / 1000 + 60;
        //QString timestr = QString::number(epoc);
        QString timestr = QDateTime::currentDateTime().toString("yyyyMMdd HH:mm:00 t");
        api->reqHistoricalData(hid, contract, timestr, "60 S", "1 min", "MIDPOINT", 1, 1);
    });*/

    QObject::connect(api, &TWS::QTwsAPI::connectionEstablish, [=]() {
        api->setServerLogLevel(1);
        qDebug() << "connectionEstablish";
        TWS::Contract contract;
        contract.symbol = "EUR";
        contract.currency = "USD";
        contract.secType = "CASH";
        contract.exchange = "IDEALPRO";
        long id = api->nextTickerId();
        //api->reqMktData(id, contract, "", true);
        //api->cancelMktData(id);
        api->reqMktData(id, contract, "233", false);
        //timer->start(60000);
        qDebug() << "id:" << id;
    });

    app.exec();
    api->eDisconnect();
}
