#include "ternarydatapoint.h"

TernaryDataPoint::TernaryDataPoint(QObject *parent) : QObject(parent)
{

}

TernaryDataPoint::TernaryDataPoint(double A, double B, double C)
{
    setUserValues(A, B, C);
    isVisible_ = true;
    isSelected_ = false;
}

void TernaryDataPoint::setUserValues(double A, double B, double C)
{
    userA_ = A;
    userB_ = B;
    userC_ = C;

    a_ = A/(A + B + C);
    b_ = B/(A + B + C);
    c_ = C/(A + B + C);
}


double TernaryDataPoint::a()
{
    return a_;
}

double TernaryDataPoint::b()
{
    return b_;
}

double TernaryDataPoint::c()
{
    return c_;
}

bool TernaryDataPoint::isVisible()
{
    return isVisible_;
}

void TernaryDataPoint::setVisible(bool visible)
{
    isVisible_ = visible;
}

bool TernaryDataPoint::isSelected()
{
    return isSelected_;
}

void TernaryDataPoint::setSelection(bool selected)
{
    isSelected_ = selected;
    if(isSelected_)
        emit selectTableRow(tableRow_);
}

void TernaryDataPoint::setTableRow(int row)
{
    tableRow_ = row;
}
