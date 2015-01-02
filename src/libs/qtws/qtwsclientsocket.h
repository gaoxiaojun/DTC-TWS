#ifndef __QTWS_CLIENT_SOCKET_BASE_H__
#define __QTWS_CLIENT_SOCKET_BASE_H__

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QTcpSocket>
#include <QTimer>

#include "twscommon.h"
#include "twscontract.h"
#include "twsstring.h"
#include "twstagvalue.h"
#include "twscommissionreport.h"
#include "twsexecution.h"
#include "twsorder.h"
#include "twsorderstate.h"
#include "twsscannersubscription.h"
#include "twssocketclienterrors.h"

namespace TWS {

class QTWSClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit QTWSClientSocket(QObject *parent = 0);
    ~QTWSClientSocket();

    Q_INVOKABLE bool start();
    Q_INVOKABLE void stop();

public:
    int serverVersion() const { return m_serverVersion;}
    QString connectionTime() const { return m_TwsTime;}

    QString host() const { return m_host;}
    void setHost(const QString& host) { m_host = host;}
    unsigned int port() const { return m_port;}
    void setPort(unsigned int port) { m_port = port;}
    unsigned int waitTime() const { return m_waitTime;}
    void setWaitTime(unsigned int msecs) { m_waitTime = msecs;}
    unsigned int retryTime() { return m_retryTime;}
    void setRetryTime(unsigned int msecs) {m_retryTime = msecs;}

    bool isConnected() const { return m_connected;}
    int clientId() const { return m_clientId; }
    void setClientId(int clientId) { m_clientId = clientId;}

signals:
    void twsConnectionClosed();
    void twsConnectionEstablish();

    void twsTickPrice(long id, TickType field, double price, int size, int canAutoExecute);
    void twsTickSize(long id, TickType field, int size);
    void twsTickOptionComputation(long id, TickType tickType, double impliedVol, double delta,
        double optPrice, double pvDividend, double gamma, double vega, double theta, double undPrice);
    void twsTickGeneric(long id, TickType tickType, double value);
    void twsTickString(long id, TickType tickType, const QString& value);
    void twsTickEFP(long id, TickType tickType, double basisPoints, const QString& formattedBasisPoints,
        double totalDividends, int holdDays, const QString& futureExpiry, double dividendImpact, double dividendsToExpiry);
    void twsOrderStatus(long orderId, const QString &status, int filled,
                        int remaining, double avgFillPrice, int permId, int parentId,
                        double lastFillPrice, int clientId, const QString& whyHeld);
    void twsOpenOrder(long orderId, const Contract&, const Order&, const OrderState&);
    void twsOpenOrderEnd();
    void twsUpdateAccountValue(const QString& key, const QString& val,
                               const QString& currency, const QString& accountName);
    void twsUpdatePortfolio(const Contract& contract, int position,
                            double marketPrice, double marketValue, double averageCost,
                            double unrealizedPNL, double realizedPNL, const QString& accountName);
    void twsUpdateAccountTime(const QString& timeStamp);
    void twsAccountDownloadEnd(const QString& accountName);
    void twsNextValidId(long orderId);
    void twsContractDetails(int reqId, const ContractDetails& contractDetails);
    void twsBondContractDetails(int reqId, const ContractDetails& contractDetails);
    void twsContractDetailsEnd(int reqId);
    void twsExecDetails(int reqId, const Contract& contract, const Execution& execution);
    void twsExecDetailsEnd(int reqId);
    void twsError(const int id, const int errorCode, const QString errorString);
    void twsUpdateMktDepth(long id, int position, int operation, int side,
                           double price, int size);
    void twsUpdateMktDepthL2(long id, int position, QString marketMaker, int operation,
                             int side, double price, int size);
    void twsUpdateNewsBulletin(int msgId, int msgType, const QString& newsMessage, const QString& originExch);
    void twsManagedAccounts( const QString& accountsList);
    void twsReceiveFA(faDataType pFaDataType, const QString& cxml);
    void twsHistoricalData(long reqId, const QString& date, double open, double high,
                           double low, double close, int volume, int barCount, double WAP, int hasGaps);
    void twsScannerParameters(const QString &xml);
    void twsScannerData(int reqId, int rank, const ContractDetails &contractDetails,
                        const QString &distance, const QString &benchmark, const QString &projection,
                        const QString &legsStr);
    void twsScannerDataEnd(int reqId);
    void twsRealtimeBar(long reqId, long time, double open, double high, double low, double close,
                        long volume, double wap, int count);
    void twsCurrentTime(long time);
    void twsFundamentalData(long reqId, const QString& data);
    void twsDeltaNeutralValidation(int reqId, const UnderComp& underComp);
    void twsTickSnapshotEnd(int reqId);
    void twsMarketDataType(long reqId, int marketDataType);
    void twsCommissionReport(const CommissionReport &commissionReport);
    void twsPosition(const QString& account, const Contract& contract, int position, double avgCost);
    void twsPositionEnd();
    void twsAccountSummary(int reqId, const QString& account, const QString& tag, const QString& value, const QString& curency);
    void twsAccountSummaryEnd(int reqId);
    void twsVerifyMessageAPI(const QString& apiData);
    void twsVerifyCompleted(bool isSuccessful, const QString& errorText);
    void twsDisplayGroupList(int reqId, const QString& groups);
    void twsDisplayGroupUpdated(int reqId, const QString& contractInfo);

public slots:
    void reqMktData(long id, const Contract &contract,
                    const QString &genericTicks, bool snapshot, const TagValueListSPtr& mktDataOptions);
    void cancelMktData(long id);
    void placeOrder(long id, const Contract &contract, const Order &order);
    void cancelOrder(long id) ;
	void reqOpenOrders();
    void reqAccountUpdates(bool subscribe, const QString& acctCode);
	void reqExecutions(int reqId, const ExecutionFilter& filter);
	void reqIds(int numIds);
	bool checkMessages();
	void reqContractDetails(int reqId, const Contract &contract);
    void reqMktDepth(long tickerId, const Contract &contract, int numRows, const TagValueListSPtr& mktDepthOptions);
    void cancelMktDepth(long tickerId);
	void reqNewsBulletins(bool allMsgs);
	void cancelNewsBulletins();
	void setServerLogLevel(int level);
	void reqAutoOpenOrders(bool bAutoBind);
	void reqAllOpenOrders();
	void reqManagedAccts();
	void requestFA(faDataType pFaDataType);
    void replaceFA(faDataType pFaDataType, const QString& cxml);
    void reqHistoricalData(long id, const Contract &contract,
                            const QString &endDateTime, const QString &durationStr,
                            const QString & barSizeSetting, const QString &whatToShow,
                            int useRTH, int formatDate, const TagValueListSPtr& chartOptions);
    void exerciseOptions(long tickerId, const Contract &contract,
                         int exerciseAction, int exerciseQuantity,
                         const QString &account, int override);
    void cancelHistoricalData(long tickerId );
    void reqRealTimeBars(long id, const Contract &contract, int barSize,
                         const QString &whatToShow, bool useRTH, const TagValueListSPtr& realTimeBarsOptions);
    void cancelRealTimeBars(long tickerId );
	void cancelScannerSubscription(int tickerId);
	void reqScannerParameters();
    void reqScannerSubscription(int tickerId, const ScannerSubscription &subscription,
                                const TagValueListSPtr& scannerSubscriptionOptions);
	void reqCurrentTime();
    void reqFundamentalData(long reqId, const Contract&, const QString& reportType);
    void cancelFundamentalData(long reqId);
    void calculateImpliedVolatility(long reqId, const Contract &contract, double optionPrice, double underPrice);
    void calculateOptionPrice(long reqId, const Contract &contract, double volatility, double underPrice);
    void cancelCalculateImpliedVolatility(long reqId);
    void cancelCalculateOptionPrice(long reqId);
	void reqGlobalCancel();
	void reqMarketDataType(int marketDataType);
	void reqPositions();
	void cancelPositions();
    void reqAccountSummary( int reqId, const QString& groupName, const QString& tags);
	void cancelAccountSummary( int reqId);
    void verifyRequest(const QString& apiName, const QString& apiVersion);
    void verifyMessage(const QString& apiData);
    void queryDisplayGroups(int reqId);
    void subscribeToGroupEvents(int reqId, int groupId);
    void updateDisplayGroup(int reqId, const QString& contractInfo);
    void unsubscribeFromGroupEvents(int reqId);

private slots:
    // callback from socket
    void onClose();
    void onReceive();
    void socketError(QAbstractSocket::SocketError);
public:

    bool isInBufferEmpty() const;
    bool isOutBufferEmpty() const;

protected:

    void eConnectBase();
    void eDisconnectBase();

    void setExtraAuth(bool extraAuth);
	int sendBufferedData();

private:

    //bool eConnect(const char *host, unsigned int port, int clientId = 0, unsigned int wait_msecs = 1000,
    //                      bool extraAuth = false);

    void eDisconnect();

    bool eConnect();
    void eReConnect();

    int send(const char* buf, size_t sz) ;
    int receive(char* buf, size_t sz);
    // socket state
    bool isSocketOK() const;

	int bufferedSend(const char* buf, size_t sz);

	// read and buffer what's available
	int bufferedRead();

	// try to process connection request ack
	int processConnectAck(const char*& ptr, const char* endPtr);

	// try to process single msg
	int processMsg(const char*& ptr, const char* endPtr);

	void startApi();

	static bool CheckOffset(const char* ptr, const char* endPtr);
	static const char* FindFieldEnd(const char* ptr, const char* endPtr);

	// decoders
	static bool DecodeField(bool&, const char*& ptr, const char* endPtr);
	static bool DecodeField(int&, const char*& ptr, const char* endPtr);
	static bool DecodeField(long&, const char*& ptr, const char* endPtr);
	static bool DecodeField(double&, const char*& ptr, const char* endPtr);
    static bool DecodeField(QString&, const char*& ptr, const char* endPtr);

	static bool DecodeFieldMax(int&, const char*& ptr, const char* endPtr);
	static bool DecodeFieldMax(long&, const char*& ptr, const char* endPtr);
	static bool DecodeFieldMax(double&, const char*& ptr, const char* endPtr);

	// encoders
    static void EncodeField(QByteArray& os, const QString &string);
    static void EncodeField(QByteArray &os, const char *str);
    static void EncodeField(QByteArray& os, long longValue);
    static void EncodeField(QByteArray& os, int intValue);
    static void EncodeField(QByteArray& os, bool boolValue);
    static void EncodeField(QByteArray& os, double doubleValue);
	// "max" encoders
    static void EncodeFieldMax(QByteArray& os, int intValue);
    static void EncodeFieldMax(QByteArray& os, double doubleValue);

protected:

	void onConnectBase();

private:

    static void CleanupBuffer(QByteArray&, int processed);

private:
    enum ConnectionState {
        CS_IDLE,
        CS_RECONNECTION,
        CS_SHUTDOWN
    };

    QByteArray m_inBuffer;
    QByteArray m_outBuffer;

    ConnectionState m_connectionState;

    bool m_connected;
    int m_serverVersion;
    QString m_TwsTime;

    bool m_extraAuth;
	int m_clientId;
    QString m_host;
    unsigned int m_port;
    unsigned int m_waitTime;
    unsigned int m_retryTime;

    QTcpSocket m_socket;
    QTimer m_timer;
};

} // namespace TWS

#endif // __QTWS_CLIENT_SOCKET_BASE_H__
