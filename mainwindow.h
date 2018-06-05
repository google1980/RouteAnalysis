#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

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

typedef struct {
    QString _terminal_name;
    QString _terminal_code;
    int _terminal_len;
    int _terminal_day;
    QPointF _basePoint;
}Terminal;

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
    void gradient();

    void drawOneBerthMap(const Terminal t);
    void saveOneBerthData(const Terminal t);

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
    QAction *gradientAct;

    PainterScene *scene;

    QList<Terminal> terminals;

    void castVariant2ListListVariant(const QVariant &var, QList<QList<QVariant> > &res);




};






#endif // MAINWINDOW_H
