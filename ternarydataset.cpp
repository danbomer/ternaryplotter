#include "ternarydataset.h"

TernaryDataSet::TernaryDataSet(DataSetType type, QObject *parent) : QObject(parent)
{
    dataSetType_ = type;
    tableWidget_ = new QTableWidget(1,4);
    tableWidget_->setHorizontalHeaderLabels(QStringList() << "" << "A" << "B" << "C");
    //verticalHeader()->hide();
    tableWidget_->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tableWidget_->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    tableWidget_->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    //verticalHeader()->setSectionsMovable(true);

    QTableWidgetItem *tableItem = new QTableWidgetItem("");
    tableItem->setFlags(Qt::ItemIsUserCheckable);
    tableItem->setCheckState(Qt::Unchecked);
    tableWidget_->setItem(0,0,tableItem);
    tableWidget_->resizeColumnToContents(0);
}

void TernaryDataSet::addDataPoint(TernaryDataPoint *point)
{
    dataPoints_.append(point);
}


int TernaryDataSet::pointCount()
{
    return dataPoints_.count();
}

TernaryDataPoint* TernaryDataSet::dataPoint(int i)
{
    if(i < dataPoints_.size())
        return dataPoints_.at(i);
    else
        return nullptr;
}

bool TernaryDataSet::isVisible()
{
    return isVisible_;
}

void TernaryDataSet::setVisible(bool visible)
{
    TernaryDataPoint *p;

    isVisible_ = visible;

    foreach (p, dataPoints_)
    {
        p->setVisible(visible);
    }
}


void TernaryDataSet::selectAll(bool select)
{
    TernaryDataPoint *p;

    foreach (p, dataPoints_)
    {
        p->setSelection(select);
    }
}

QTableWidget *TernaryDataSet::tableWidget()
{
    return tableWidget_;
}
