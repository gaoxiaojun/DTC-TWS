/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#ifndef QString_h__INCLUDED
#define QString_h__INCLUDED

#include <QString>
typedef QString IBString;

inline bool IsEmpty(const QString& str)
{
    return str.isEmpty();
};

inline void Empty(QString& str)
{
    str.clear();
};

inline bool Compare(QString str, const char* strToCompare)
{
    return QString::compare(str, strToCompare, Qt::CaseInsensitive);
};

inline bool Compare(QString str, QString strToCompare)
{
    return QString::compare(str, strToCompare, Qt::CaseInsensitive);
};

inline double Atof(QString str)
{
    return str.toDouble();
};

inline int Atoi(QString str)
{
    return str.toInt();
};

#endif


