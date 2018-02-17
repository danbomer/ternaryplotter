#ifndef TERNARYPLOTWINDOW_H
#define TERNARYPLOTWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "ternarycanvas.h"

namespace Ui {
class TernaryPlotWindow;
}

class TernaryPlotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TernaryPlotWindow(QWidget *parent = 0);
    ~TernaryPlotWindow();    

public slots:
    void front();
    TernaryDataSet *addDataSet(DataSetType type);

private:
    Ui::TernaryPlotWindow *ui;

protected:
    TernaryCanvas *canvas_;
    QTableWidget *dataSetWidget_;
    QStackedWidget *dataPointWidgets_;
    //QTableWidget *dataPointWidget_;
    QAction *windowAction_;
    int nextDataSet_;
    TernaryDataSet *currentDataSet_;

private slots:
    void on_actionNew_Ternary_Plot_triggered();
    void on_actionShow_Grid_triggered(bool checked);
    void dataPointWidget_item_changed(QTableWidgetItem *item);
};

#endif // TERNARYPLOTWINDOW_H
