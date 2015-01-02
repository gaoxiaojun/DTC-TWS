/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#ifndef tagvalue_def
#define tagvalue_def

#include <QSharedPointer>
#include <QList>
#include <QString>
#include <QMetaType>

namespace TWS {

//typedef QHash<QString,QString> ctkDictionary;

struct TagValue
{
	TagValue() {}
    TagValue(const QString& p_tag, const QString& p_value)
		: tag(p_tag), value(p_value)
	{}

    QString tag;
    QString value;
};

typedef QSharedPointer<TagValue> TagValueSPtr;
typedef QList<TagValueSPtr> TagValueList;
typedef QSharedPointer<TagValueList> TagValueListSPtr;

} // namespace TWS

Q_DECLARE_METATYPE(TWS::TagValue)
Q_DECLARE_METATYPE(TWS::TagValueSPtr)
Q_DECLARE_METATYPE(TWS::TagValueList)
Q_DECLARE_METATYPE(TWS::TagValueListSPtr)

#endif

