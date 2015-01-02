/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#ifndef execution_def
#define execution_def

#include <QString.h>
#include <QMetaType>

namespace TWS {
struct Execution
{
	Execution()
	{
		shares = 0;
		price = 0;
		permId = 0;
		clientId = 0;
		orderId = 0;
		cumQty = 0;
		avgPrice = 0;
		evMultiplier = 0;
	}

    QString	execId;
    QString	time;
    QString	acctNumber;
    QString	exchange;
    QString	side;
	int			shares;
	double		price;
	int			permId;
	long		clientId;
	long		orderId;
	int			liquidation;
	int			cumQty;
	double		avgPrice;
    QString	orderRef;
    QString	evRule;
	double		evMultiplier;
};

struct ExecutionFilter
{
	ExecutionFilter()
		: m_clientId(0)
	{
	}

	// Filter fields
	long		m_clientId;
    QString	m_acctCode;
    QString	m_time;
    QString	m_symbol;
    QString	m_secType;
    QString	m_exchange;
    QString	m_side;
};

} //namespace TWS

Q_DECLARE_METATYPE(TWS::Execution)
Q_DECLARE_METATYPE(TWS::ExecutionFilter)

#endif // execution_def
