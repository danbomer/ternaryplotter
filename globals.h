#ifndef GLOBALS_H
#define GLOBALS_H

#include <QMenu>

class TernaryPlotWindow;

extern int nextTernaryPlot;
extern QMenu *windowMenu;
extern QAction *windowSeparator;
extern QList<TernaryPlotWindow*> *plotWindows;

#endif // GLOBALS_H
