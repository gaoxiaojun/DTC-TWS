/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#ifndef contract_def
#define contract_def

#include <QSharedPointer>
#include <QList>
#include <QString>
#include <QMetaType>

#include "twsstring.h"
#include "twstagvalue.h"

/*
	SAME_POS    = open/close leg value is same as combo
	OPEN_POS    = open
	CLOSE_POS   = close
	UNKNOWN_POS = unknown
*/
namespace TWS {
enum LegOpenClose { SAME_POS, OPEN_POS, CLOSE_POS, UNKNOWN_POS };

struct ComboLeg
{
	ComboLeg()
		: conId(0)
		, ratio(0)
		, openClose(0)
		, shortSaleSlot(0)
		, exemptCode(-1)
	{
	}

	long		conId;
	long		ratio;
    QString	action; //BUY/SELL/SSHORT

    QString	exchange;
	long		openClose; // LegOpenClose enum values

	// for stock legs when doing short sale
	long		shortSaleSlot; // 1 = clearing broker, 2 = third party
    QString	designatedLocation;
	int			exemptCode;

	bool operator==( const ComboLeg &other) const
	{
		return (conId == other.conId &&
			ratio == other.ratio &&
			openClose == other.openClose &&
			shortSaleSlot == other.shortSaleSlot &&
			(Compare(action, other.action) == 0) &&
			(Compare(exchange, other.exchange) == 0) &&
			(Compare(designatedLocation, other.designatedLocation) == 0) &&
			exemptCode == other.exemptCode);
	}
};

struct UnderComp
{
	UnderComp()
		: conId(0)
		, delta(0)
		, price(0)
	{}

	long	conId;
	double	delta;
	double	price;
};

typedef QSharedPointer<ComboLeg> ComboLegSPtr;

struct Contract
{
	Contract()
		: conId(0)
		, strike(0)
		, includeExpired(false)
		, comboLegs(NULL)
		, underComp(NULL)
	{
	}

    long    conId;
    QString	symbol;
    QString	secType;
    QString	expiry;
    double	strike;
    QString	right;
    QString	multiplier;
    QString	exchange;
    QString	primaryExchange; // pick an actual (ie non-aggregate) exchange that the contract trades on.  DO NOT SET TO SMART.
    QString	currency;
    QString	localSymbol;
    QString	tradingClass;
    bool	includeExpired;
    QString	secIdType;		// CUSIP;SEDOL;ISIN;RIC
    QString	secId;

	// COMBOS
    QString comboLegsDescrip; // received in open order 14 and up for all combos

	// combo legs
    typedef QList<ComboLegSPtr> ComboLegList;
    typedef QSharedPointer<ComboLegList> ComboLegListSPtr;

	ComboLegListSPtr comboLegs;

	// delta neutral
	UnderComp* underComp;

public:

	// Helpers
	static void CloneComboLegs(ComboLegListSPtr& dst, const ComboLegListSPtr& src);
};

struct ContractDetails
{
	ContractDetails()
		: minTick(0)
		, priceMagnifier(0)
		, underConId(0)
		, evMultiplier(0)
		, callable(false)
		, putable(false)
		, coupon(0)
		, convertible(false)
		, nextOptionPartial(false)
	{
	}

	Contract	summary;
    QString	marketName;
	double		minTick;
    QString	orderTypes;
    QString	validExchanges;
	long		priceMagnifier;
	int			underConId;
    QString	longName;
    QString	contractMonth;
    QString	industry;
    QString	category;
    QString	subcategory;
    QString	timeZoneId;
    QString	tradingHours;
    QString	liquidHours;
    QString	evRule;
	double		evMultiplier;

	TagValueListSPtr secIdList;

	// BOND values
    QString	cusip;
    QString	ratings;
    QString	descAppend;
    QString	bondType;
    QString	couponType;
	bool		callable;
	bool		putable;
	double		coupon;
	bool		convertible;
    QString	maturity;
    QString	issueDate;
    QString	nextOptionDate;
    QString	nextOptionType;
	bool		nextOptionPartial;
    QString	notes;
};

inline void
Contract::CloneComboLegs(ComboLegListSPtr& dst, const ComboLegListSPtr& src)
{
    if (!src.data())
		return;

	dst->reserve(src->size());

	ComboLegList::const_iterator iter = src->begin();
	const ComboLegList::const_iterator iterEnd = src->end();

	for (; iter != iterEnd; ++iter) {
        const ComboLeg* leg = iter->data();
		if (!leg)
			continue;
		dst->push_back(ComboLegSPtr(new ComboLeg(*leg)));
	}
}

} // namespace TWS

Q_DECLARE_METATYPE(TWS::ComboLeg)
Q_DECLARE_METATYPE(TWS::UnderComp)
Q_DECLARE_METATYPE(TWS::ContractDetails)
Q_DECLARE_METATYPE(TWS::Contract)

#endif
