#ifndef TWSLOGGING_H
#define TWSLOGGING_H

#include <QObject>
#include <QLoggingCategory>

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

namespace TWS {
class QTwsAPI;

class TwsLogging : public QObject
{
    Q_OBJECT
public:
    explicit TwsLogging(QObject *parent);

    void monitor(QTwsAPI *api);

public slots:
    void connectionClosed();
    void connectionEstablish();
    void serverLost();
    void serverRestore(bool needReSubscribe);
    void marketDataLost();
    void marketDataRestore();
    void historicalDataLost();
    void historicalDataRestore();

    void tickPrice(long reqId, TickType field, double price, int size, int canAutoExecute);
    void tickSize(long reqId, TickType field, int size);
    void tickOptionComputation(long reqId, TickType tickType, double impliedVol,
                                double delta, double optPrice, double pvDividend, double gamma,
                                double vega, double theta, double undPrice);
    void tickGeneric(long reqId, TickType field, double value);
    void tickString(long reqId, TickType field, const QString& value);
    void tickEFP(long reqId, TickType tickType, double basisPoints,
                 const QString& formattedBasisPoints, double totalDividends,
                 int holdDays, const QString& futureExpiry, double dividendImpact, double dividendsToExpiry);
    void orderStatus(long orderId, const QString &status, int filled,
                      int remaining, double avgFillPrice, int permId, int parentId,
                      double lastFillPrice, int clientId, const QString& whyHeld);
    void openOrder(long orderId, const Contract&, const Order&, const OrderState&);
    void openOrderEnd();
    void updateAccountValue(const QString& key, const QString& val,
                            const QString& currency, const QString& accountName);
    void updatePortfolio( const Contract& contract, int position, double marketPrice,
                          double marketValue, double averageCost, double unrealizedPNL,
                          double realizedPNL, const QString& accountName);
    void updateAccountTime(const QString& timeStamp);
    void accountDownloadEnd(const QString& accountName);
    void nextValidId(long orderId);
    void contractDetails( int reqId, const ContractDetails& contractDetails);
    void bondContractDetails( int reqId, const ContractDetails& contractDetails);
    void contractDetailsEnd( int reqId);
    void execDetails( int reqId, const Contract& contract, const Execution& execution);
    void execDetailsEnd( int reqId);
    void error(const int id, const int errorCode, const QString errorString);
    void updateMktDepth(long reqId, int position, int operation, int side,
       double price, int size);
    void updateMktDepthL2(long reqId, int position, QString marketMaker, int operation,
       int side, double price, int size);
    void updateNewsBulletin(int msgId, int msgType, const QString& newsMessage, const QString& originExch);
    void managedAccounts( const QString& accountsList);
    void receiveFA(faDataType pFaDataType, const QString& cxml);
    void historicalData(long reqId, const QString& date, double open, double high,
                        double low, double close, int volume, int barCount, double WAP, int hasGaps);
    void scannerParameters(const QString &xml);
    void scannerData(int reqId, int rank, const ContractDetails &contractDetails,
        const QString &distance, const QString &benchmark, const QString &projection,
        const QString &legsStr);
    void scannerDataEnd(int reqId);
    void realtimeBar(long reqId, long time, double open, double high, double low, double close,
        long volume, double wap, int count);
    void currentTime(long time);
    void fundamentalData(long reqId, const QString& data);
    void deltaNeutralValidation(int reqId, const UnderComp& underComp);
    void tickSnapshotEnd( int reqId);
    void marketDataType(long reqId, int marketDataType);
    void commissionReport( const CommissionReport &commissionReport);
    void position( const QString& account, const Contract& contract, int position);
    void positionEnd();
    void accountSummary(int reqId, const QString& account, const QString& tag, const QString& value, const QString& currency);
    void accountSummaryEnd( int reqId);
};

}

Q_DECLARE_LOGGING_CATEGORY(QT_TWS_LOG)

#endif // TWSLOGGING_H
