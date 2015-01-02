#ifndef TWSPROXYCLIENT_H
#define TWSPROXYCLIENT_H

#include <QObject>
#include "qtwsclient_global.h"

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

namespace Internal {
class QTwsAPIPrivate;
}

void registerTwsMetaType();

class TWS_EXPORT QTwsAPI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int clientId READ clientId() WRITE setClientId)
    Q_PROPERTY(QString host READ host WRITE setHost)
    Q_PROPERTY(unsigned int port READ port WRITE setPort)
    Q_PROPERTY(unsigned int waitTime READ waitTime WRITE setWaitTime)
    Q_PROPERTY(unsigned int retryTime READ retryTime WRITE setRetryTime)

    Q_PROPERTY(int serverVersion READ serverVersion)
    Q_PROPERTY(QString TwsConnectionTime READ TwsConnectionTime)
    Q_PROPERTY(bool isConnected READ isConnected)

public:
    explicit QTwsAPI(QObject *parent = 0);
    ~QTwsAPI();

public:
    // Tws API
    int serverVersion() const;
    QString TwsConnectionTime() const;

    QString host() const;
    void setHost(const QString& host);
    unsigned int port() const;
    void setPort(unsigned int port);
    unsigned int waitTime() const;
    void setWaitTime(unsigned int msecs);
    unsigned int retryTime();
    void setRetryTime(unsigned int msecs);

    bool isConnected();
    int clientId() const;
    void setClientId(int clientId);

    bool eConnect(const char *host, unsigned int port, int clientId = 0);
    void eDisconnect();

    long nextTickerId();
    long nextOrderId();
    static QString tickTypeString(TickType field);

    void reqMktData(long reqId, const Contract &contract, const QString &genericTicks, bool snapshot = false,
                    const TagValueListSPtr& mktDataOptions = TagValueListSPtr());
    void cancelMktData(long reqId);
    void placeOrder(long reqId, const Contract &contract, const Order &order);
    void cancelOrder(long orderId) ;
    void reqOpenOrders();
    void reqAccountUpdates(bool subscribe, const QString& acctCode);
    void reqExecutions(long reqId, const ExecutionFilter& filter);
    void reqIds(int numIds);
    void reqContractDetails(long reqId, const Contract &contract);
    void reqMktDepth(long reqId, const Contract &contract, int numRows = 10,
                     const TagValueListSPtr& mktDepthOptions = TagValueListSPtr());
    void cancelMktDepth(long reqId);
    void reqNewsBulletins(bool allMsgs = true);
    void cancelNewsBulletins();
    void setServerLogLevel(int level = 5);
    void reqAutoOpenOrders(bool bAutoBind = true);
    void reqAllOpenOrders();
    void reqManagedAccts();
    void requestFA(faDataType pFaDataType);
    void replaceFA(faDataType pFaDataType, const QString& cxml);
    void reqHistoricalData(long reqId, const Contract &contract,
        const QString &endDateTime, const QString &durationStr,
        const QString & barSizeSetting, const QString &whatToShow,
        int useRTH, int formatDate, const TagValueListSPtr& chartOptions = TagValueListSPtr());
    void exerciseOptions(long reqId, const Contract &contract, int exerciseAction, int exerciseQuantity,
                         const QString &account, int override);
    void cancelHistoricalData(long reqId );
    void reqRealTimeBars(long reqId, const Contract &contract, int barSize,
                         const QString &whatToShow, bool useRTH,
                         const TagValueListSPtr& realTimeBarsOptions = TagValueListSPtr());
    void cancelRealTimeBars(long reqId );
    void cancelScannerSubscription(int reqId);
    void reqScannerParameters();
    void reqScannerSubscription(long reqId, const ScannerSubscription &subscription,
                                const TagValueListSPtr& scannerSubscriptionOptions = TagValueListSPtr());
    void reqCurrentTime();
    void reqFundamentalData(long reqId, const Contract&c, const QString& reportType);
    void cancelFundamentalData(long reqId);
    void calculateImpliedVolatility(long reqId, const Contract &contract, double optionPrice, double underPrice);
    void calculateOptionPrice(long reqId, const Contract &contract, double volatility, double underPrice);
    void cancelCalculateImpliedVolatility(long reqId);
    void cancelCalculateOptionPrice(long reqId);
    void reqGlobalCancel();
    void reqMarketDataType(int marketDataType);
    void reqPositions();
    void cancelPositions();
    void reqAccountSummary(long reqId, const QString& groupName, const QString& tags);
    void cancelAccountSummary( int reqId);
    void verifyRequest(const QString& apiName, const QString& apiVersion);
    void verifyMessage(const QString& apiData);
    void queryDisplayGroups(int reqId);
    void subscribeToGroupEvents(int reqId, int groupId);
    void updateDisplayGroup(int reqId, const QString& contractInfo);
    void unsubscribeFromGroupEvents(int reqId);

signals:
    void connectionClosed();
    void connectionEstablish();
    void serverLost();
    void serverRestore(bool needReSubscribe);
    void marketDataLost();
    void marketDataRestore();
    void historicalDataLost();
    void historicalDataRestore();

    void barPrice(long reqId, double price, int size);
    void tickPrice(long reqId, TickType field, double price, int size, int canAutoExecute);
    void tickSize(long reqId, TickType field, int size);
    void tickOptionComputation(long reqId, TickType tickType, double impliedVol,
                                double delta, double optPrice, double pvDividend, double gamma,
                                double vega, double theta, double undPrice);
    void tickGeneric(long reqId, TickType tickType, double value);
    void tickString(long reqId, TickType tickType, const QString& value);
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
    void accountSummary( int reqId, const QString& account, const QString& tag, const QString& value, const QString& curency);
    void accountSummaryEnd( int reqId);

private:
    Internal::QTwsAPIPrivate *d;
};

} // namespace tws
#endif // TWSPROXYCLIENT_H
