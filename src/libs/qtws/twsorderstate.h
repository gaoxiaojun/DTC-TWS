/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#ifndef ORDER_STATE_H__INCLUDED
#define ORDER_STATE_H__INCLUDED

#include "twsorder.h"
#include <QMetaType>

namespace TWS {
struct OrderState {

	explicit OrderState()
		:
		commission(UNSET_DOUBLE),
		minCommission(UNSET_DOUBLE),
		maxCommission(UNSET_DOUBLE)
	{}

    QString status;

    QString initMargin;
    QString maintMargin;
    QString equityWithLoan;

	double  commission;
	double  minCommission;
	double  maxCommission;
    QString commissionCurrency;

    QString warningText;
};


} // namespace TWS

Q_DECLARE_METATYPE(TWS::OrderState)

#endif
