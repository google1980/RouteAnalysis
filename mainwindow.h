#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMdiArea>
#include <QAction>
#include <QStatusBar>
#include <QMdiSubWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QGraphicsView>
#include <QResizeEvent>
#include <QPrinter>
#include <QAxObject>
#include "painter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private slots:

    void newFile();
    void queryData();
    void saveData();
    void queryTerminal();
    void exportPDF();
    void queryNavigation();
    void queryBerth();
    void importExcel();
    void childWinExit(int type);
    void lock();
    void unlock();
    void undo();
    void clean();
    void about();
    void updateWindowMenu();

private:

    void createActions();
    void createStatusBar();

    QMdiArea *mdiArea;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QToolBar * toolBar;
    QProgressBar * progressBar;

    QMenu *windowMenu;
    QAction *newAct;
    QAction *saveAct;
    QAction *queryAct;
    QAction *terminalAct;
    QAction *navigationAct;
    QAction *pdfAct;
    QAction *importAct;
    QAction *berthAct;
    QAction *lockAct;
    QAction *unlockAct;
    QAction *undoAct;
    QAction *cleanAct;
    QAction *closeAct;
    QAction *closeAllAct;

    PainterScene *scene;

    QString m_current_terminal;
    int m_current_terminal_len;
    int m_current_terminal_day;

    void castVariant2ListListVariant(const QVariant &var, QList<QList<QVariant> > &res);




};






#endif // MAINWINDOW_H
