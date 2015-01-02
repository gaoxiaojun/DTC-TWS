#include "qtwsapilogging.h"
#include "qtwsapi.h"

#include <QDateTime>

#define SC(x) connect(api, &QTwsAPI::x, this, &TwsLogging::x)

Q_LOGGING_CATEGORY(QT_TWS_LOG, "tws.logging")

using namespace TWS;

TwsLogging::TwsLogging(QObject *parent)
{
}

void TwsLogging::monitor(QTwsAPI *api)
{
    SC(connectionClosed);
    SC(connectionEstablish);
    SC(serverLost);
    SC(serverRestore);
    SC(marketDataLost);
    SC(marketDataRestore);
    SC(historicalDataLost);
    SC(historicalDataRestore);

    SC(tickPrice);
    SC(tickSize);
    SC(tickOptionComputation);
    SC(tickGeneric);
    SC(tickString);
    SC(tickEFP);
    SC(orderStatus);
    SC(openOrder);
    SC(openOrderEnd);
    SC(updateAccountValue);
    SC(updatePortfolio);
    SC(updateAccountTime);
    SC(accountDownloadEnd);
    SC(nextValidId);
    SC(contractDetails);
    SC(bondContractDetails);
    SC(contractDetailsEnd);
    SC(execDetails);
    SC(execDetailsEnd);
    SC(error);
    SC(updateMktDepth);
    SC(updateMktDepthL2);
    SC(updateNewsBulletin);
    SC(managedAccounts);
    SC(receiveFA);
    SC(historicalData);
    SC(scannerParameters);
    SC(scannerData);
    SC(scannerDataEnd);
    SC(realtimeBar);
    SC(currentTime);
    SC(fundamentalData);
    SC(deltaNeutralValidation);
    SC(tickSnapshotEnd);
    SC(marketDataType);
    SC(commissionReport);
    SC(position);
    SC(positionEnd);
    SC(accountSummary);
    SC(accountSummaryEnd);
}

void TwsLogging::connectionClosed()
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::connectionEstablish()
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::serverLost()
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::serverRestore(bool needReSubscribe)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO << (needReSubscribe ? "needReSubscribe" : "");
}

void TwsLogging::marketDataLost()
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::marketDataRestore()
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::historicalDataLost()
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::historicalDataRestore()
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::tickPrice(long reqId, TickType field, double price, int size, int canAutoExecute)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO << QDateTime::currentMSecsSinceEpoch() << reqId << QTwsAPI::tickTypeString(field) << price << size << canAutoExecute;
}

void TwsLogging::tickSize(long reqId, TickType field, int size)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO << reqId << QTwsAPI::tickTypeString(field) << size;
}

void TwsLogging::tickOptionComputation(long reqId, TickType tickType, double impliedVol,
                            double delta, double optPrice, double pvDividend, double gamma,
                            double vega, double theta, double undPrice)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::tickGeneric(long reqId, TickType field, double value)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO << reqId << QTwsAPI::tickTypeString(field) << value;
}

void TwsLogging::tickString(long reqId, TickType field, const QString& value)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO << reqId << QTwsAPI::tickTypeString(field) << value;
}

void TwsLogging::tickEFP(long reqId, TickType tickType, double basisPoints,
             const QString& formattedBasisPoints, double totalDividends,
             int holdDays, const QString& futureExpiry, double dividendImpact, double dividendsToExpiry)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::orderStatus(long orderId, const QString &status, int filled,
                  int remaining, double avgFillPrice, int permId, int parentId,
                  double lastFillPrice, int clientId, const QString& whyHeld)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::openOrder(long orderId, const Contract&, const Order&, const OrderState&)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::openOrderEnd()
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::updateAccountValue(const QString& key, const QString& val,
                        const QString& currency, const QString& accountName)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO << key << val << currency << accountName;
}

void TwsLogging::updatePortfolio( const Contract& contract, int position, double marketPrice,
                      double marketValue, double averageCost, double unrealizedPNL,
                      double realizedPNL, const QString& accountName)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::updateAccountTime(const QString& timeStamp)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO << timeStamp;
}

void TwsLogging::accountDownloadEnd(const QString& accountName)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO << accountName;
}

void TwsLogging::nextValidId(long orderId)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO << orderId;
}

void TwsLogging::contractDetails( int reqId, const ContractDetails& contractDetails)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::bondContractDetails( int reqId, const ContractDetails& contractDetails)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::contractDetailsEnd( int reqId)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::execDetails( int reqId, const Contract& contract, const Execution& execution)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::execDetailsEnd( int reqId)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::error(const int id, const int errorCode, const QString errorString)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO << id << errorCode << errorString;
}

void TwsLogging::updateMktDepth(long reqId, int position, int operation, int side,
                                double price, int size)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::updateMktDepthL2(long reqId, int position, QString marketMaker, int operation, int side, double price, int size)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::updateNewsBulletin(int msgId, int msgType, const QString& newsMessage, const QString& originExch)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::managedAccounts( const QString& accountsList)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO << accountsList;
}

void TwsLogging::receiveFA(faDataType pFaDataType, const QString& cxml)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::historicalData(long reqId, const QString& date, double open, double high,
                    double low, double close, int volume, int barCount, double WAP, int hasGaps)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO << reqId << date << open << high << low << close << volume << barCount << WAP << hasGaps;
}

void TwsLogging::scannerParameters(const QString &xml)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::scannerData(int reqId, int rank, const ContractDetails &contractDetails,
    const QString &distance, const QString &benchmark, const QString &projection,
    const QString &legsStr)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::scannerDataEnd(int reqId)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::realtimeBar(long reqId, long time, double open, double high, double low, double close,
    long volume, double wap, int count)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::currentTime(long time)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO << time;
}

void TwsLogging::fundamentalData(long reqId, const QString& data)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::deltaNeutralValidation(int reqId, const UnderComp& underComp)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::tickSnapshotEnd( int reqId)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO << reqId;
}

void TwsLogging::marketDataType(long reqId, int marketDataType)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO << reqId << marketDataType;
}

void TwsLogging::commissionReport(const CommissionReport &commissionReport)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::position(const QString& account, const Contract& contract, int position)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO << account << contract.symbol << position;
}

void TwsLogging::positionEnd()
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO;
}

void TwsLogging::accountSummary(int reqId, const QString& account, const QString& tag, const QString& value, const QString& currency)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO << reqId << account << tag << value << currency;
}

void TwsLogging::accountSummaryEnd(int reqId)
{
    qCDebug(QT_TWS_LOG) << Q_FUNC_INFO << reqId;
}
