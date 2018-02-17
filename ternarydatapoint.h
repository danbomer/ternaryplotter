#ifndef TERNARYDATAPOINT_H
#define TERNARYDATAPOINT_H

#include <QObject>

class TernaryDataPoint : public QObject
{
    Q_OBJECT
public:
    explicit TernaryDataPoint(QObject *parent = nullptr);
    explicit TernaryDataPoint(double A, double B, double C);

    void setUserValues(double A, double B, double C);
    double a();
    double b();
    double c();
    bool isVisible();
    void setVisible(bool visible);
    bool isSelected();
    void setSelection(bool selected);
    void setTableRow(int row);

signals:
    void selectTableRow(int);

public slots:

protected:
    double userA_, userB_, userC_;
    double a_,b_,c_;
    bool isVisible_;
    bool isSelected_;
    int tableRow_;
};

#endif // TERNARYDATAPOINT_H
