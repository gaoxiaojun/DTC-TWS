/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#ifndef commissionreport_def
#define commissionreport_def

#include <QString>
#include <QMetaType>

namespace TWS {
struct CommissionReport
{
	CommissionReport()
	{
		commission = 0;
		realizedPNL = 0;
		yield = 0;
		yieldRedemptionDate = 0;
	}

	// commission report fields
    QString     execId;
	double		commission;
    QString     currency;
	double		realizedPNL;
	double		yield;
	int			yieldRedemptionDate; // YYYYMMDD format
};

} //namespace TWS

Q_DECLARE_METATYPE(TWS::CommissionReport)

#endif // commissionreport_def
