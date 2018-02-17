#include "ternaryplotwindow.h"
#include "ui_ternaryplotwindow.h"
#include "globals.h"
#include <QScreen>
#include <QLayout>


TernaryPlotWindow::TernaryPlotWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TernaryPlotWindow)
{
    ui->setupUi(this);
    QRect r = QApplication::screens().at(0)->availableGeometry();

    if(r.width() < 1440)
    {
        resize(r.height() * 1.2, r.height());
    }
    else
    {
        resize(r.height() * 1.02, r.height() * 0.85);
    }
    move(20 * nextTernaryPlot, 25 * nextTernaryPlot);

    canvas_ = new TernaryCanvas(this);
    canvas_->setMinimumHeight(200);
    canvas_->setMinimumWidth(200);
    dataSetWidget_ = new QTableWidget(1,1,this);
    dataSetWidget_->setHorizontalHeaderLabels(QStringList() << "Data Sets");
    dataSetWidget_->horizontalHeader()->setStretchLastSection(true);
    dataSetWidget_->setSelectionBehavior(QAbstractItemView::SelectRows);
    dataSetWidget_->verticalHeader()->hide();
    nextDataSet_ = 1;
    QTableWidgetItem *tableItem = new QTableWidgetItem("Data Set #" + QString::number(nextDataSet_));
    nextDataSet_++;
    tableItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tableItem->setCheckState(Qt::Checked);
    dataSetWidget_->setItem(0,0,tableItem);
    dataSetWidget_->resizeColumnToContents(0);
    addDataSet(PointDataSet);

    dataPointWidgets_ = new QStackedWidget(this);

    QObject::connect(currentDataSet_->tableWidget(),&QTableWidget::itemChanged,this,&TernaryPlotWindow::dataPointWidget_item_changed);
    dataPointWidgets_->addWidget(currentDataSet_->tableWidget());

    setWindowTitle("Ternary Plot #" + QString::number(nextTernaryPlot));

    QHBoxLayout* mainLayout = new QHBoxLayout;
    QVBoxLayout* tableLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(canvas_,15);
    mainLayout->addSpacing(5);

    tableLayout->setContentsMargins(0,0,0,0);
    tableLayout->setSpacing(0);
    tableLayout->addWidget(dataSetWidget_,5);
    tableLayout->addSpacing(5);
    tableLayout->addWidget(dataPointWidgets_,15);
    mainLayout->addLayout(tableLayout,5);
    centralWidget()->setLayout(mainLayout);

    windowAction_ = new QAction("Ternary Plot #" + QString::number(nextTernaryPlot));
    QObject::connect(windowAction_,&QAction::triggered,this,&TernaryPlotWindow::front);

    windowMenu->insertAction(0, windowAction_);
    ui->menuBar->addMenu(windowMenu);

    plotWindows->append(this);
    nextTernaryPlot++;
}

TernaryPlotWindow::~TernaryPlotWindow()
{
    delete ui;
}

void TernaryPlotWindow::front()
{
    show();
    raise();
    activateWindow();
}

TernaryDataSet *TernaryPlotWindow::addDataSet(DataSetType type)
{
    currentDataSet_ = new TernaryDataSet(type);
    canvas_->addDataSet(currentDataSet_);
}

void TernaryPlotWindow::on_actionNew_Ternary_Plot_triggered()
{
    TernaryPlotWindow *w = new TernaryPlotWindow(0);
    w->show();
}

void TernaryPlotWindow::on_actionShow_Grid_triggered(bool checked)
{
    canvas_->toggleGrid(checked);

}

void TernaryPlotWindow::dataPointWidget_item_changed(QTableWidgetItem *item)
{
    double A, B, C;
    bool isANumber;

    QTableWidget *dataPointWidget = (QTableWidget *)dataPointWidgets_->currentWidget();

    if(item->column() == 0)
    {
        int row = item->row();
        if(TernaryDataPoint *p = currentDataSet_->dataPoint(row))
            p->setVisible(item->checkState());
        canvas_->update();
    }
    else if(item->row() == (dataPointWidget->rowCount() - 1))
    {
        int row = item->row();
        QTableWidgetItem *item;
        if((item = dataPointWidget->item(row,1)))
        {
            A = item->text().toDouble(&isANumber);
            if(isANumber)
            {
                if((item = dataPointWidget->item(row,2)))
                {
                    B = item->text().toDouble(&isANumber);
                    if(isANumber)
                    {
                        if((item = dataPointWidget->item(row,3)))
                        {
                            C = item->text().toDouble(&isANumber);
                            if(isANumber)
                            {
                                // add this data point
                                item = dataPointWidget->item(row,0);
                                TernaryDataPoint *p = new TernaryDataPoint(A,B,C);
                                p->setTableRow(row);
                                QObject::connect(p,&TernaryDataPoint::selectTableRow,dataPointWidget,&QTableWidget::selectRow);
                                currentDataSet_->addDataPoint(p);
                                canvas_->update();
                                item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                                item->setCheckState(Qt::Checked);

                                // add new row to table
                                dataPointWidget->insertRow(dataPointWidget->rowCount());
                                item = new QTableWidgetItem("");
                                item->setFlags(Qt::ItemIsUserCheckable);
                                item->setCheckState(Qt::Unchecked);
                                dataPointWidget->setItem(row+1,0,item);
                            }
                        }
                    }
                }
            }
        }
    }
}
