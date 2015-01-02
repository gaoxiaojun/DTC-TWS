/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#ifndef scanner_def
#define scanner_def

#include <float.h>
#include <limits.h>

#include <QString>
#include <QMetaType>

#define UNSET_DOUBLE DBL_MAX
#define UNSET_INTEGER INT_MAX
#define NO_ROW_NUMBER_SPECIFIED -1;

namespace TWS {
struct ScannerSubscription {
	ScannerSubscription() {
		numberOfRows = NO_ROW_NUMBER_SPECIFIED;
		abovePrice = DBL_MAX;
		belowPrice = DBL_MAX;
		aboveVolume = INT_MAX;
		marketCapAbove = DBL_MAX;
		marketCapBelow = DBL_MAX;
		couponRateAbove = DBL_MAX;
		couponRateBelow = DBL_MAX;
		excludeConvertible = 0;
		averageOptionVolumeAbove = 0;
	}
    int numberOfRows;
    QString instrument;
    QString locationCode;
    QString scanCode;
    double abovePrice;
    double belowPrice;
    int aboveVolume;
    double marketCapAbove;
    double marketCapBelow;
    QString moodyRatingAbove;
    QString moodyRatingBelow;
    QString spRatingAbove;
    QString spRatingBelow;
    QString maturityDateAbove;
    QString maturityDateBelow;
    double couponRateAbove;
    double couponRateBelow;
    int excludeConvertible;
	int averageOptionVolumeAbove;
    QString scannerSettingPairs;
    QString stockTypeFilter;
};

} // namespace TWS

Q_DECLARE_METATYPE(TWS::ScannerSubscription)

#endif
