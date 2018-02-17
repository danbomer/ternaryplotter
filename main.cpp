#include "ternaryplotwindow.h"
#include <QApplication>
#include "globals.h"

int nextTernaryPlot = 1;
QMenu *windowMenu;
QAction *windowSeparator;
QList<TernaryPlotWindow*> *plotWindows;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    windowMenu = new QMenu("Window");
    windowSeparator = windowMenu->addSeparator();
    plotWindows = new QList<TernaryPlotWindow*>();

    TernaryPlotWindow w;
    w.show();

    return a.exec();
}
