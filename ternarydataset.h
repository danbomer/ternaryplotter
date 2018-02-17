#ifndef TERNARYDATASET_H
#define TERNARYDATASET_H

#include <QObject>
#include <QTableWidget>
#include <QHeaderView>
#include "ternarydatapoint.h"

enum DataSetType {PointDataSet, LineDataSet, AreaDataSet};

class TernaryDataSet : public QObject
{
    Q_OBJECT
public:
    explicit TernaryDataSet(DataSetType type, QObject *parent = nullptr);

    void addDataPoint(TernaryDataPoint *point);
    int pointCount();
    TernaryDataPoint* dataPoint(int i);
    bool isVisible();
    void setVisible(bool visible);
    void selectAll(bool select);
    QTableWidget *tableWidget();

signals:

public slots:

protected:
    DataSetType dataSetType_;
    QList<TernaryDataPoint *> dataPoints_;
    bool isVisible_;
    QTableWidget *tableWidget_;
};

#endif // TERNARYDATASET_H
