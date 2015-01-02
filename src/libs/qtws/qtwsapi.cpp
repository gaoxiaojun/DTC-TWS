#include "qtwsapi.h"
#include "qtwsclientsocket.h"

#include <QThread>
#include <QAtomicInteger>

namespace TWS {

void registerTwsMetaType()
{
    qRegisterMetaType<TickType>("TickType");
    qRegisterMetaType<CommissionReport>("CommissionReport");
    qRegisterMetaType<ComboLeg>("ComboLeg");
    qRegisterMetaType<UnderComp>("UnderComp");
    qRegisterMetaType<Contract>("Contract");
    qRegisterMetaType<ContractDetails>("ContractDetails");
    qRegisterMetaType<Execution>("Execution");
    qRegisterMetaType<ExecutionFilter>("ExecutionFilter");
    qRegisterMetaType<OrderComboLeg>("OrderComboLeg");
    qRegisterMetaType<Order>("Order");
    qRegisterMetaType<OrderState>("OrderState");
    qRegisterMetaType<ScannerSubscription>("ScannerSubscription");
    qRegisterMetaType<TagValue>("TagValue");
    qRegisterMetaType<TagValueSPtr>("TagValueSPtr");
    qRegisterMetaType<TagValueList>("TagValueList");
    qRegisterMetaType<TagValueListSPtr>("TagValueListSPtr");
}

QString QTwsAPI::tickTypeString(TickType field)
{
    switch( field) {
             case BID_SIZE:                    return "bidSize";
             case BID:                         return "bidPrice";
             case ASK:                         return "askPrice";
             case ASK_SIZE:                    return "askSize";
             case LAST:                        return "lastPrice";
             case LAST_SIZE:                   return "lastSize";
             case HIGH:                        return "high";
             case LOW:                         return "low";
             case VOLUME:                      return "volume";
             case CLOSE:                       return "close";
             case BID_OPTION_COMPUTATION:      return "bidOptComp";
             case ASK_OPTION_COMPUTATION:      return "askOptComp";
             case LAST_OPTION_COMPUTATION:     return "lastOptComp";
             case MODEL_OPTION:                return "modelOptComp";
             case OPEN:                        return "open";
             case LOW_13_WEEK:                 return "13WeekLow";
             case HIGH_13_WEEK:                return "13WeekHigh";
             case LOW_26_WEEK:                 return "26WeekLow";
             case HIGH_26_WEEK:                return "26WeekHigh";
             case LOW_52_WEEK:                 return "52WeekLow";
             case HIGH_52_WEEK:                return "52WeekHigh";
             case AVG_VOLUME:                  return "AvgVolume";
             case OPEN_INTEREST:               return "OpenInterest";
             case OPTION_HISTORICAL_VOL:       return "OptionHistoricalVolatility";
             case OPTION_IMPLIED_VOL:          return "OptionImpliedVolatility";
             case OPTION_BID_EXCH:             return "OptionBidExchStr";
             case OPTION_ASK_EXCH:             return "OptionAskExchStr";
             case OPTION_CALL_OPEN_INTEREST:   return "OptionCallOpenInterest";
             case OPTION_PUT_OPEN_INTEREST:    return "OptionPutOpenInterest";
             case OPTION_CALL_VOLUME:          return "OptionCallVolume";
             case OPTION_PUT_VOLUME:           return "OptionPutVolume";
             case INDEX_FUTURE_PREMIUM:        return "IndexFuturePremium";
             case BID_EXCH:                    return "bidExch";
             case ASK_EXCH:                    return "askExch";
             case AUCTION_VOLUME:              return "auctionVolume";
             case AUCTION_PRICE:               return "auctionPrice";
             case AUCTION_IMBALANCE:           return "auctionImbalance";
             case MARK_PRICE:                  return "markPrice";
             case BID_EFP_COMPUTATION:         return "bidEFP";
             case ASK_EFP_COMPUTATION:         return "askEFP";
             case LAST_EFP_COMPUTATION:        return "lastEFP";
             case OPEN_EFP_COMPUTATION:        return "openEFP";
             case HIGH_EFP_COMPUTATION:        return "highEFP";
             case LOW_EFP_COMPUTATION:         return "lowEFP";
             case CLOSE_EFP_COMPUTATION:       return "closeEFP";
             case LAST_TIMESTAMP:              return "lastTimestamp";
             case SHORTABLE:                   return "shortable";
             case FUNDAMENTAL_RATIOS:          return "fundamentals";
             case RT_VOLUME:                   return "RTVolume";
             case HALTED:                      return "halted";
             case BID_YIELD:                   return "bidYield";
             case ASK_YIELD:                   return "askYield";
             case LAST_YIELD:                  return "lastYield";
             case CUST_OPTION_COMPUTATION:     return "custOptComp";
             case TRADE_COUNT:                 return "trades";
             case TRADE_RATE:                  return "trades/min";
             case VOLUME_RATE:                 return "volume/min";
             case LAST_RTH_TRADE:              return "lastRTHTrade";
             default:                          return "unknown";
    }
}

#define CS(x) connect(this, &QTwsAPIPrivate::x, m_socket, &QTWSClientSocket::x)
#define SC(x) connect(m_socket, &QTWSClientSocket::tws##x, this, &QTwsAPIPrivate::on##x)

namespace Internal {

class subscribe {
public:
    subscribe(long id)
        : reqId(id), askSize(-1), bidSize(-1), ask(0.0), bid(0.0), isCash(false){}
    long reqId;
    int askSize; // work around for IB duplicate tickSize bug
    int bidSize;
    double ask;
    double bid;
    double last;
    int lastSize;
    bool isCash;
};

class QTwsAPIPrivate : public QObject
{
    Q_OBJECT
public:
    QTwsAPI *q;
    QTWSClientSocket  *m_socket;
    QThread m_thread;
    QTimer m_timer;
    QAtomicInteger<long> m_tick_id;
    QAtomicInteger<long> m_order_id;
    QHash<long, subscribe*> m_subscribe;

public:
    explicit QTwsAPIPrivate(QTwsAPI *parent)
        : QObject(parent),
          q(parent),
          m_tick_id(long(1))
    {
        m_socket = new QTWSClientSocket();
        m_socket->moveToThread(&m_thread);
        m_thread.start();
        // connect slot to socket
        CS(reqMktData);
        CS(cancelMktData);
        CS(placeOrder);
        CS(cancelOrder);
        CS(reqOpenOrders);
        CS(reqAccountUpdates);
        CS(reqExecutions);
        CS(reqIds);
        CS(reqContractDetails);
        CS(reqMktDepth);
        CS(cancelMktDepth);
        CS(reqNewsBulletins);
        CS(cancelNewsBulletins);
        CS(setServerLogLevel);
        CS(reqAutoOpenOrders);
        CS(reqAllOpenOrders);
        CS(reqManagedAccts);
        CS(requestFA);
        CS(replaceFA);
        CS(reqHistoricalData);
        CS(exerciseOptions);
        CS(cancelHistoricalData);
        CS(reqRealTimeBars);
        CS(cancelRealTimeBars);
        CS(cancelScannerSubscription);
        CS(reqScannerParameters);
        CS(reqScannerSubscription);
        CS(reqCurrentTime);
        CS(reqFundamentalData);
        CS(cancelFundamentalData);
        CS(calculateImpliedVolatility);
        CS(calculateOptionPrice);
        CS(cancelCalculateImpliedVolatility);
        CS(cancelCalculateOptionPrice);
        CS(reqGlobalCancel);
        CS(reqMarketDataType);
        CS(reqPositions);
        CS(cancelPositions);
        CS(reqAccountSummary);
        CS(cancelAccountSummary);
        CS(verifyRequest);
        CS(verifyMessage);
        CS(queryDisplayGroups);
        CS(subscribeToGroupEvents);
        CS(updateDisplayGroup);
        CS(unsubscribeFromGroupEvents);
        // connect signal from socket
        SC(ConnectionClosed);
        SC(ConnectionEstablish);
        SC(TickPrice);
        SC(TickSize);
        SC(TickOptionComputation);
        SC(TickGeneric);
        SC(TickString);
        SC(TickEFP);
        SC(OrderStatus);
        SC(OpenOrder);
        SC(OpenOrderEnd);
        SC(UpdateAccountValue);
        SC(UpdatePortfolio);
        SC(UpdateAccountTime);
        SC(AccountDownloadEnd);
        SC(NextValidId);
        SC(ContractDetails);
        SC(BondContractDetails);
        SC(ContractDetailsEnd);
        SC(ExecDetails);
        SC(ExecDetailsEnd);
        SC(Error);
        SC(UpdateMktDepth);
        SC(UpdateMktDepthL2);
        SC(UpdateNewsBulletin);
        SC(ManagedAccounts);
        SC(ReceiveFA);
        SC(HistoricalData);
        SC(ScannerParameters);
        SC(ScannerData);
        SC(ScannerDataEnd);
        SC(RealtimeBar);
        SC(CurrentTime);
        SC(FundamentalData);
        SC(DeltaNeutralValidation);
        SC(TickSnapshotEnd);
        SC(MarketDataType);
        SC(CommissionReport);
        SC(Position);
        SC(PositionEnd);
        SC(AccountSummary);
        SC(AccountSummaryEnd);
    }

    ~QTwsAPIPrivate()
    {
        m_socket->deleteLater();
        m_thread.quit();
        m_thread.wait();
        QHashIterator<long, subscribe*> iter(m_subscribe);
        if(iter.hasNext()) {
            delete iter.value();
        }
        m_subscribe.clear();
    }

public slots:
    void onConnectionClosed()
    {
        emit q->connectionClosed();
    }

    void onConnectionEstablish()
    {
        emit q->connectionEstablish();
    }

    void onTickPrice( long reqId, TickType field, double price, int size, int canAutoExecute)
    {
        subscribe *item = m_subscribe.value(reqId);

        if (item != NULL) {
            switch (field) {
            case ASK:
                item->ask = price;
                item->askSize = size;
                if (canAutoExecute)
                    emit q->barPrice(reqId,  item->bid == 0.0 ? price : (price + item->bid) /2, 0);
                break;
            case BID:
                item->bid = price;
                item->bidSize = size;
                if (canAutoExecute)
                    emit q->barPrice(reqId, item->ask == 0.0 ? price : (price + item->ask) /2, 0);
                break;
            case LAST:
                if (canAutoExecute)
                    emit q->barPrice(reqId, price, size);
                break;
            default:
                break;
            }
        }else {
            qDebug() << "error on tickprice, no subscribe info";
        }

        emit q->tickPrice(reqId, field, price, size, canAutoExecute);
    }

    void onTickSize( long reqId, TickType field, int size)
    {
        if(field == ASK_SIZE || field == BID_SIZE) {
            if (m_subscribe.contains(reqId)) {
                subscribe *item = m_subscribe.value(reqId);
                if (field==ASK_SIZE) {
                    if (size == item->askSize)
                        item->askSize = -1;
                    else
                        emit q->tickPrice(reqId, ASK, item->ask, size, 0);
                }else {
                    if (size == item->bidSize)
                        item->bidSize = -1;
                    else
                        emit q->tickPrice(reqId, BID, item->bid, size, 0);
                }
            }else {
                qDebug() << "error on ticksize, no subscribe info";
                emit q->tickSize(reqId, field, size);
            }
        }else {
            emit q->tickSize(reqId, field, size);
        }
    }

    void onTickOptionComputation( long reqId, TickType tickType, double impliedVol,
                                double delta, double optPrice, double pvDividend, double gamma,
                                double vega, double theta, double undPrice)
    {
        emit q->tickOptionComputation(reqId, tickType, impliedVol, delta, optPrice, pvDividend, gamma, vega, theta, undPrice);
    }

    void onTickGeneric(long reqId, TickType tickType, double value)
    {
        emit q->tickGeneric(reqId, tickType, value);
    }

    void onTickString(long reqId, TickType tickType, const QString& value)
    {
        emit q->tickString(reqId, tickType, value);
    }

    void onTickEFP(long reqId, TickType tickType, double basisPoints,
                 const QString& formattedBasisPoints, double totalDividends,
                 int holdDays, const QString& futureExpiry, double dividendImpact, double dividendsToExpiry)
    {
        emit q->tickEFP(reqId, tickType, basisPoints, formattedBasisPoints, totalDividends, holdDays, futureExpiry,
                dividendImpact, dividendsToExpiry);
    }

    void onOrderStatus( long orderId, const QString &status, int filled,
                      int remaining, double avgFillPrice, int permId, int parentId,
                      double lastFillPrice, int clientId, const QString& whyHeld)
    {
        emit q->orderStatus(orderId, status, filled, remaining, avgFillPrice, permId, parentId, lastFillPrice, clientId, whyHeld);
    }

    void onOpenOrder( long orderId, const Contract& c, const Order& o, const OrderState& os)
    {
        emit q->openOrder(orderId, c, o ,os);
    }

    void onOpenOrderEnd()
    {
        emit q->openOrderEnd();
    }

    void onUpdateAccountValue(const QString& key, const QString& val,
                            const QString& currency, const QString& accountName)
    {
        emit q->updateAccountValue(key, val, currency, accountName);
    }

    void onUpdatePortfolio( const Contract& contract, int position, double marketPrice,
                          double marketValue, double averageCost, double unrealizedPNL,
                          double realizedPNL, const QString& accountName)
    {
        emit q->updatePortfolio(contract, position, marketPrice, marketValue, averageCost, unrealizedPNL, realizedPNL, accountName);
    }

    void onUpdateAccountTime(const QString& timeStamp)
    {
        emit q->updateAccountTime(timeStamp);
    }

    void onAccountDownloadEnd(const QString& accountName)
    {
        emit q->accountDownloadEnd(accountName);
    }

    void onNextValidId( long orderId)
    {
        m_order_id = orderId;
        emit q->nextValidId(orderId);
    }

    void onContractDetails( int reqId, const ContractDetails& cd)
    {
        emit q->contractDetails(reqId, cd);
    }

    void onBondContractDetails( int reqId, const ContractDetails& contractDetails)
    {
        emit q->bondContractDetails(reqId, contractDetails);
    }

    void onContractDetailsEnd( int reqId)
    {
        emit q->contractDetailsEnd(reqId);
    }

    void onExecDetails( int reqId, const Contract& contract, const Execution& execution)
    {
        emit q->execDetails(reqId, contract, execution);
    }

    void onExecDetailsEnd( int reqId)
    {
        emit q->execDetailsEnd(reqId);
    }

    /*
     * https://www.interactivebrokers.com/en/software/api/apiguide/tables/api_message_codes.htm
     */
    void onError(const int id, const int errorCode, const QString errorString)
    {
        switch(errorCode) {
         case 1100:                 // Connectivity between IB and TWS has been lost.
             emit q->serverLost();
             break;
         case 1101:                 // Connectivity between IB and TWS has been restored- data lost.*
             emit q->serverRestore(true);  // 必须重新提交市场和账户数据订阅请求
             break;
         case 1102:                 // Connectivity between IB and TWS has been restored- data maintained.
             emit q->serverRestore(false);
             break;
         case 2103:                 // A market data farm is disconnected.
         case 2108:                 // A market data farm connection has become inactive but should be available upon demand.
             emit q->marketDataLost();
             break;
         case 2104:                 // A market data farm is connected.
             emit q->marketDataRestore();
             break;
         case 2105:                 // A historical data farm is disconnected.
         case 2107:                 // A historical data farm connection has become inactive but should be available upon demand.
             emit q->historicalDataLost();
             break;
         case 2106:                 // A historical data farm is connected.
             emit q->historicalDataRestore();
             break;
         case 2110:                 // Connectivity between TWS and server is broken. It will be restored automatically.
             emit q->serverLost();
             break;
         default:
            emit q->error(id, errorCode, errorString);
            break;
        }
    }

    void onUpdateMktDepth(long id, int position, int operation, int side,
                          double price, int size)
    {
        emit q->updateMktDepth(id, position, operation, side, price, size);
    }

    void onUpdateMktDepthL2(long id, int position, QString marketMaker, int operation,
                          int side, double price, int size)
    {
        emit q->updateMktDepthL2(id, position, marketMaker, operation, side, price, size);
    }

    void onUpdateNewsBulletin(int msgId, int msgType, const QString& newsMessage, const QString& originExch)
    {
        emit q->updateNewsBulletin(msgId, msgType, newsMessage, originExch);
    }

    void onManagedAccounts( const QString& accountsList)
    {
        emit q->managedAccounts(accountsList);
    }

    void onReceiveFA(faDataType pFaDataType, const QString& cxml)
    {
        emit q->receiveFA(pFaDataType, cxml);
    }

    void onHistoricalData(long reqId, const QString& date, double open, double high,
                        double low, double close, int volume, int barCount, double WAP, int hasGaps)
    {
        emit q->historicalData(reqId, date, open, high, low, close, volume, barCount, WAP, hasGaps);
    }

    void onScannerParameters(const QString &xml)
    {
        emit q->scannerParameters(xml);
    }

    void onScannerData(int reqId, int rank, const ContractDetails &contractDetails,
        const QString &distance, const QString &benchmark, const QString &projection,
        const QString &legsStr)
    {
        emit q->scannerData(reqId, rank, contractDetails, distance, benchmark, projection, legsStr);
    }

    void onScannerDataEnd(int reqId)
    {
        emit q->scannerDataEnd(reqId);
    }

    void onRealtimeBar(long reqId, long time, double open, double high, double low, double close,
        long volume, double wap, int count)
    {
        emit q->realtimeBar(reqId, time, open, high, low, close, volume, wap, count);
    }

    void onCurrentTime(long time)
    {
        emit q->currentTime(time);
    }

    void onFundamentalData(long reqId, const QString& data)
    {
        emit q->fundamentalData(reqId, data);
    }

    void onDeltaNeutralValidation(int reqId, const UnderComp& underComp)
    {
        emit q->deltaNeutralValidation(reqId, underComp);
    }

    void onTickSnapshotEnd( int reqId)
    {
        emit q->tickSnapshotEnd(reqId);
    }

    void onMarketDataType( long reqId, int mdt)
    {
        emit q->marketDataType(reqId, mdt);
    }

    void onCommissionReport( const CommissionReport &cr)
    {
        emit q->commissionReport(cr);
    }

    void onPosition( const QString& account, const Contract& contract, int pos)
    {
        emit q->position(account, contract, pos);
    }

    void onPositionEnd()
    {
        emit q->positionEnd();
    }

    void onAccountSummary( int reqId, const QString& account, const QString& tag, const QString& value, const QString& currency)
    {
        emit q->accountSummary(reqId, account, tag, value, currency);
    }

    void onAccountSummaryEnd( int reqId)
    {
        emit q->accountSummaryEnd(reqId);
    }

signals:
    void reqMktData(long id, const Contract &contract,
        const QString &genericTicks, bool snapshot, const TagValueListSPtr& mktDataOptions);
    void cancelMktData(long id);
    void placeOrder(long id, const Contract &contract, const Order &order);
    void cancelOrder(long id) ;
    void reqOpenOrders();
    void reqAccountUpdates(bool subscribe, const QString& acctCode);
    void reqExecutions(int reqId, const ExecutionFilter& filter);
    void reqIds(int numIds);
    void reqContractDetails(int reqId, const Contract &contract);
    void reqMktDepth(long reqId, const Contract &contract, int numRows, const TagValueListSPtr& mktDepthOptions);
    void cancelMktDepth(long reqId);
    void reqNewsBulletins(bool allMsgs);
    void cancelNewsBulletins();
    void setServerLogLevel(int level);
    void reqAutoOpenOrders(bool bAutoBind);
    void reqAllOpenOrders();
    void reqManagedAccts();
    void requestFA(faDataType pFaDataType);
    void replaceFA(faDataType pFaDataType, const QString& cxml);
    void reqHistoricalData( long id, const Contract &contract,
        const QString &endDateTime, const QString &durationStr,
        const QString & barSizeSetting, const QString &whatToShow,
        int useRTH, int formatDate, const TagValueListSPtr& chartOptions);
    void exerciseOptions(long reqId, const Contract &contract,
        int exerciseAction, int exerciseQuantity,
        const QString &account, int override);
    void cancelHistoricalData(long reqId );
    void reqRealTimeBars(long id, const Contract &contract, int barSize,
        const QString &whatToShow, bool useRTH, const TagValueListSPtr& realTimeBarsOptions);
    void cancelRealTimeBars(long reqId );
    void cancelScannerSubscription(int reqId);
    void reqScannerParameters();
    void reqScannerSubscription(int reqId, const ScannerSubscription &subscription, const TagValueListSPtr& scannerSubscriptionOptions);
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
    void verifyRequest( const QString& apiName, const QString& apiVersion);
    void verifyMessage( const QString& apiData);
    void queryDisplayGroups( int reqId);
    void subscribeToGroupEvents( int reqId, int groupId);
    void updateDisplayGroup( int reqId, const QString& contractInfo);
    void unsubscribeFromGroupEvents( int reqId);
};

} // namespace Internal

QTwsAPI::QTwsAPI(QObject *parent)
    :QObject(parent), d(new Internal::QTwsAPIPrivate(this))
{
}

QTwsAPI::~QTwsAPI()
{
}

int QTwsAPI::serverVersion() const
{
    return d->m_socket->serverVersion();
}
QString QTwsAPI::TwsConnectionTime() const
{
    return d->m_socket->connectionTime();
}

QString QTwsAPI::host() const
{
    return d->m_socket->host();
}

void QTwsAPI::setHost(const QString& host)
{
    d->m_socket->setHost(host);
}

unsigned int QTwsAPI::port() const
{
    return d->m_socket->port();
}

void QTwsAPI::setPort(unsigned int port)
{
    d->m_socket->setPort(port);
}

unsigned int QTwsAPI::waitTime() const
{
    return d->m_socket->waitTime();
}

void QTwsAPI::setWaitTime(unsigned int msecs)
{
    d->m_socket->setWaitTime(msecs);
}

unsigned int QTwsAPI::retryTime()
{
    return d->m_socket->retryTime();
}

void QTwsAPI::setRetryTime(unsigned int msecs)
{
    d->m_socket->setRetryTime(msecs);
}

bool QTwsAPI::isConnected()
{
    return d->m_socket->isConnected();
}

int QTwsAPI::clientId() const
{
    return d->m_socket->clientId();
}

void QTwsAPI::setClientId(int clientId)
{
    d->m_socket->setClientId(clientId);
}

bool QTwsAPI::eConnect(const char *host, unsigned int port, int clientId)
{
    bool result;
    setHost(host);
    setPort(port);
    setClientId(clientId);
    QMetaObject::invokeMethod(d->m_socket, "start", Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(bool, result));

    return result;
}

void QTwsAPI::eDisconnect()
{
    QMetaObject::invokeMethod(d->m_socket, "stop", Qt::BlockingQueuedConnection);
}

long QTwsAPI::nextTickerId()
{
    return d->m_tick_id++;
}

long QTwsAPI::nextOrderId()
{
    return d->m_order_id++;
}

void QTwsAPI::reqMktData(long reqId, const Contract &contract, const QString &genericTicks,
                            bool snapshot, const TagValueListSPtr& mktDataOptions)
{
    if (!d->m_subscribe.contains(reqId)) {
        Internal::subscribe * item = new Internal::subscribe(reqId);
        if(contract.secType == "CASH")
            item->isCash = true;
        d->m_subscribe.insert(reqId, item);
    }

    emit d->reqMktData(reqId, contract, genericTicks, snapshot, mktDataOptions);
}

void QTwsAPI::cancelMktData(long reqId)
{
    if (d->m_subscribe.contains(reqId)) {
        Internal::subscribe *item = d->m_subscribe.take(reqId);
        delete item;
    }
    emit d->cancelMktData(reqId);
}

void QTwsAPI::placeOrder(long reqId, const Contract &contract, const Order &order)
{
    emit d->placeOrder(reqId, contract, order);
}

void QTwsAPI::cancelOrder(long orderId)
{
    emit d->cancelOrder(orderId);
}

void QTwsAPI::reqOpenOrders()
{
    emit d->reqOpenOrders();
}

void QTwsAPI::reqAccountUpdates(bool subscribe, const QString& acctCode)
{
    emit d->reqAccountUpdates(subscribe, acctCode);
}

void QTwsAPI::reqExecutions(long reqId, const ExecutionFilter& filter)
{
    emit d->reqExecutions(reqId, filter);
}

void QTwsAPI::reqIds(int numIds)
{
    emit d->reqIds(numIds);
}

void QTwsAPI::reqContractDetails(long reqId, const Contract &contract)
{
    emit d->reqContractDetails(reqId, contract);
}

void QTwsAPI::reqMktDepth(long reqId, const Contract &contract, int numRows, const TagValueListSPtr& mktDepthOptions)
{
    emit d->reqMktDepth(reqId, contract, numRows, mktDepthOptions);
}

void QTwsAPI::cancelMktDepth(long reqId)
{
    emit d->cancelMktDepth(reqId);
}

void QTwsAPI::reqNewsBulletins(bool allMsgs)
{
    emit d->reqNewsBulletins(allMsgs);
}

void QTwsAPI::cancelNewsBulletins()
{
    emit d->cancelNewsBulletins();
}

void QTwsAPI::setServerLogLevel(int level)
{
    emit d->setServerLogLevel(level);
}

void QTwsAPI::reqAutoOpenOrders(bool bAutoBind)
{
    emit d->reqAutoOpenOrders(bAutoBind);
}

void QTwsAPI::reqAllOpenOrders()
{
    emit d->reqAllOpenOrders();
}

void QTwsAPI::reqManagedAccts()
{
    emit d->reqManagedAccts();
}

void QTwsAPI::requestFA(faDataType pFaDataType)
{
    emit d->requestFA(pFaDataType);
}

void QTwsAPI::replaceFA(faDataType pFaDataType, const QString& cxml)
{
    emit d->replaceFA(pFaDataType, cxml);
}

void QTwsAPI::reqHistoricalData(long reqId, const Contract &contract,
    const QString &endDateTime, const QString &durationStr,
    const QString & barSizeSetting, const QString &whatToShow,
    int useRTH, int formatDate, const TagValueListSPtr& chartOptions)
{
    emit d->reqHistoricalData(reqId, contract, endDateTime, durationStr, barSizeSetting, whatToShow, useRTH, formatDate, chartOptions);
}

void QTwsAPI::exerciseOptions(long reqId, const Contract &contract,
    int exerciseAction, int exerciseQuantity,
    const QString &account, int override)
{
    emit d->exerciseOptions(reqId, contract, exerciseAction, exerciseQuantity, account, override);
}

void QTwsAPI::cancelHistoricalData(long reqId )
{
    emit d->cancelHistoricalData(reqId);
}

void QTwsAPI::reqRealTimeBars(long reqId, const Contract &contract, int barSize,
    const QString &whatToShow, bool useRTH, const TagValueListSPtr& realTimeBarsOptions)
{
    emit d->reqRealTimeBars(reqId, contract, barSize, whatToShow, useRTH, realTimeBarsOptions);
}

void QTwsAPI::cancelRealTimeBars(long reqId )
{
    emit d->cancelRealTimeBars(reqId);
}

void QTwsAPI::cancelScannerSubscription(int reqId)
{
    emit d->cancelScannerSubscription(reqId);
}

void QTwsAPI::reqScannerParameters()
{
    emit d->reqScannerParameters();
}

void QTwsAPI::reqScannerSubscription(long reqId, const ScannerSubscription &subscription, const TagValueListSPtr& scannerSubscriptionOptions)
{
    emit d->reqScannerSubscription(reqId, subscription, scannerSubscriptionOptions);
}

void QTwsAPI::reqCurrentTime()
{
    emit d->reqCurrentTime();
}

void QTwsAPI::reqFundamentalData(long reqId, const Contract& c, const QString& reportType)
{
    emit d->reqFundamentalData(reqId, c, reportType);
}

void QTwsAPI::cancelFundamentalData(long reqId)
{
    emit d->cancelFundamentalData(reqId);
}

void QTwsAPI::calculateImpliedVolatility(long reqId, const Contract &contract, double optionPrice, double underPrice)
{
    emit d->calculateImpliedVolatility(reqId, contract, optionPrice, underPrice);
}

void QTwsAPI::calculateOptionPrice(long reqId, const Contract &contract, double volatility, double underPrice)
{
    emit d->calculateOptionPrice(reqId, contract, volatility, underPrice);
}

void QTwsAPI::cancelCalculateImpliedVolatility(long reqId)
{
    emit d->cancelCalculateImpliedVolatility(reqId);
}

void QTwsAPI::cancelCalculateOptionPrice(long reqId)
{
    emit d->cancelCalculateOptionPrice(reqId);
}

void QTwsAPI::reqGlobalCancel()
{
    emit d->reqGlobalCancel();
}

void QTwsAPI::reqMarketDataType(int marketDataType)
{
    emit d->reqMarketDataType(marketDataType);
}

void QTwsAPI::reqPositions()
{
    emit d->reqPositions();
}

void QTwsAPI::cancelPositions()
{
    emit d->cancelPositions();
}

void QTwsAPI::reqAccountSummary(long reqId, const QString& groupName, const QString& tags)
{
    emit d->reqAccountSummary(reqId, groupName, tags);
}

void QTwsAPI::cancelAccountSummary(int reqId)
{
    emit d->cancelAccountSummary(reqId);
}

void QTwsAPI::verifyRequest( const QString& apiName, const QString& apiVersion)
{
    emit d->verifyRequest(apiName, apiVersion);
}

void QTwsAPI::verifyMessage( const QString& apiData)
{
    emit d->verifyMessage(apiData);
}

void QTwsAPI::queryDisplayGroups( int reqId)
{
    emit d->queryDisplayGroups(reqId);
}

void QTwsAPI::subscribeToGroupEvents( int reqId, int groupId)
{
    emit d->subscribeToGroupEvents(reqId, groupId);
}

void QTwsAPI::updateDisplayGroup( int reqId, const QString& contractInfo)
{
    emit d->updateDisplayGroup(reqId, contractInfo);
}

void QTwsAPI::unsubscribeFromGroupEvents( int reqId)
{
    emit d->unsubscribeFromGroupEvents(reqId);
}

#include "qtwsapi.moc"
} // namesapce tws
