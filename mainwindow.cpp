﻿#include "mainwindow.h"
#include "formroutedataquery.h"
#include "formterminalquery.h"
#include "formnavigationquery.h"
#include "painter.h"
#include "dataSource.h"
#include "formberthconfig.h"
#include <QDebug>

MainWindow::MainWindow()
    : mdiArea(new QMdiArea),
      statusBar(new QStatusBar),
      menuBar(new QMenuBar),
      toolBar(new QToolBar),
      progressBar(new QProgressBar),
      scene(new PainterScene)
{


    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);

    createStatusBar();

    setStatusBar(statusBar);

    setMenuBar(menuBar);

    addToolBar(toolBar);

    createActions();

    setWindowTitle(QString::fromUtf8("RouteAnalysis"));

    QSqlError err = initDb();

    if (err.type() != QSqlError::NoError) {

        statusBar->showMessage("load db file error ", 10000);
    }

    this->setWindowState(Qt::WindowMaximized);

}

MainWindow::~MainWindow()
{

    QSqlDatabase::database().close();

}

void MainWindow::clean()
{
    if (QMessageBox::Yes == QMessageBox::warning(this,
                                                  QString::fromUtf8("警告"),
                                                  QString::fromUtf8("该操作将导致所有数据被清理!!! \r\n是否继续?"),
                                                  QMessageBox::Yes | QMessageBox::No,
                                                  QMessageBox::Yes)) {

        QSqlQuery query;
        query.exec(QObject::tr("drop table BERTH"));
        query.exec(QObject::tr("drop table ROUTE_ARRANGEMENT"));
        query.exec(QObject::tr("drop table TERMINAL"));
        query.exec(QObject::tr("drop table NAVIGATION"));

        query.exec(QObject::tr("create table BERTH (TERMINAL_CODE vchar, SHIP_LIMIT integer, START_POINT integer, END_POINT integer , NICE integer)"));
        query.exec(QObject::tr("create table TERMINAL (TERMINAL_NAME vchar,TERMINAL_CODE vchar, TERMINAL_LEN integer DEFAULT 1500,DAY integer DEFAULT 8, ENABLED vchar)"));
        query.exec(QObject::tr("create table ROUTE_ARRANGEMENT (TERMINAL_NAME vchar, ROUTE_NAME vchar,ROUTE_CODE vchar,NAVIGATION_NAME vchar,SHIP_LENGTH integer, "
                                   "TEU vchar,TYPE vchar, START_WEEK_DAY vchar,START_TIME vchar,END_WEEK_DAY vchar,END_TIME vchar,START_BERTH_POINT integer,IS_LOCKED vchar DEFAULT 'N')"));
        query.exec(QObject::tr("create table NAVIGATION (NAVIGATION_NAME vchar,COLOUR vchar)"));

    } else {

    }
}

void MainWindow::about()
{

   QMessageBox message(QMessageBox::NoIcon,QString::fromUtf8("关于 RouteAnalysis"),
                       QString::fromUtf8("RouteAnalysis 是协助用户航线安排、分析的小软件,请务必按要求的EXCEL格式提供原始数据.\r\n\r\n"
                                                                                              "开发者: Road\r\n"
                                                                                              "版本号: 1.0.1"),QMessageBox::Close,this,Qt::Dialog);
   message.setIconPixmap(QPixmap(":/images/web.ico"));
   message.exec();


}

void MainWindow::updateWindowMenu()
{

}

void MainWindow::queryData()
{

    FormRouteDataQuery *child = new FormRouteDataQuery;

    QMdiSubWindow * subWindow = mdiArea->addSubWindow(child);

    subWindow->resize(QSize(1200,600));

    subWindow->setWindowTitle(QString::fromUtf8("航线数据"));

    subWindow->show();
}

void MainWindow::queryTerminal()
{

    FormTerminalQuery *child = new FormTerminalQuery;

    QMdiSubWindow * subWindow = mdiArea->addSubWindow(child);

    subWindow->resize(QSize(600,450));

    subWindow->setWindowTitle(QString::fromUtf8("作业单位设置"));

    subWindow->show();
}

void MainWindow::queryBerth()
{

    FormBerthConfig *child = new FormBerthConfig;

    QMdiSubWindow * subWindow = mdiArea->addSubWindow(child);

    subWindow->resize(QSize(540,450));

    subWindow->setWindowTitle(QString::fromUtf8("靠泊设置"));

    subWindow->show();
}

void MainWindow::queryNavigation()
{

    FormNavigationQuery *child = new FormNavigationQuery;

    QMdiSubWindow * subWindow = mdiArea->addSubWindow(child);

    subWindow->resize(QSize(480,450));

    subWindow->setWindowTitle(QString::fromUtf8("航区颜色配置"));

    subWindow->show();
}

void MainWindow::saveData()
{
    if (scene->items().count() != 0){

        QSqlQuery sql;

        QString update_sql = "update ROUTE_ARRANGEMENT set START_BERTH_POINT = :x1 , "
                             "SHIP_LENGTH = :x2 , START_WEEK_DAY = :x3, START_TIME = :x4, END_WEEK_DAY = :x5, END_TIME = :x6, IS_LOCKED = 'Y' "
                             "where ROUTE_NAME = :x7";
        sql.prepare(update_sql);

        QList<QGraphicsItem *> items = scene->items();

        foreach (QGraphicsItem *item, items) {
            if (item->type() == QGraphicsItem::UserType + 1) {  // 矩形

                RouteRectangle * routeItem = static_cast<RouteRectangle *> (item);

                sql.bindValue(":x1", PainterHelper::convertScreenToStartBerthPoint(routeItem->getStartPosScene()));
                sql.bindValue(":x2", PainterHelper::convertScreenToLength(routeItem->getStartPosScene(),routeItem->getEndPosScene()));
                sql.bindValue(":x3", PainterHelper::convertScreenToStartWeekDay(routeItem->getStartPosScene()));
                sql.bindValue(":x4", PainterHelper::convertScreenToStartTime(routeItem->getStartPosScene()));
                sql.bindValue(":x5", PainterHelper::convertScreenToEndWeekDay(routeItem->getEndPosScene()));
                sql.bindValue(":x6", PainterHelper::convertScreenToEndTime(routeItem->getEndPosScene()));
                sql.bindValue(":x7", routeItem->getId());

                if(!sql.exec())
                {
                    qDebug() << sql.lastError();
                }
                else
                {
                    qDebug() << "updated!";
                }


            }

        }


    }else{

        qDebug() << "view is not opened.";

    }
}

void MainWindow::lock()
{
    if (scene->items().count() != 0){

        QSqlQuery sql;

        QString update_sql = "update ROUTE_ARRANGEMENT set IS_LOCKED = 'Y',START_BERTH_POINT = :x0 where ROUTE_NAME = :x1";

        sql.prepare(update_sql);

        QList<QGraphicsItem *> items = scene->items();

        foreach (QGraphicsItem *item, items) {
            if ((item->type() == QGraphicsItem::UserType + 1) && (item->isSelected())) {  // 矩形

                RouteRectangle * routeItem = static_cast<RouteRectangle *> (item);
                sql.bindValue(":x0", PainterHelper::convertScreenToStartBerthPoint(routeItem->getStartPosScene()));
                sql.bindValue(":x1", routeItem->getId());

                if(!sql.exec())
                {
                    qDebug() << sql.lastError();
                }
                else
                {
                    qDebug() << "updated!";
                }

            }

        }

    }else{

        qDebug() << "view is not opened.";

    }
}

void MainWindow::unlock()
{
    if (scene->items().count() != 0){

        QSqlQuery sql;

        QString update_sql = "update ROUTE_ARRANGEMENT set IS_LOCKED = 'N' where ROUTE_NAME = :x1";

        sql.prepare(update_sql);

        QList<QGraphicsItem *> items = scene->items();

        foreach (QGraphicsItem *item, items) {
            if ((item->type() == QGraphicsItem::UserType + 1) && (item->isSelected())) {  // 矩形

                RouteRectangle * routeItem = static_cast<RouteRectangle *> (item);

                sql.bindValue(":x1", routeItem->getId());

                if(!sql.exec())
                {
                    qDebug() << sql.lastError();
                }
                else
                {
                    qDebug() << "updated!";
                }

            }

        }

    }else{

        qDebug() << "view is not opened.";

    }
}

void MainWindow::undo()
{
    if (scene->items().count() != 0){

        QSqlQuery sql;

        QString update_sql = "update ROUTE_ARRANGEMENT set IS_LOCKED = 'N' where ROUTE_NAME = :x1";

        sql.prepare(update_sql);

        QList<QGraphicsItem *> items = scene->items();

        foreach (QGraphicsItem *item, items) {
            if ((item->type() == QGraphicsItem::UserType + 1)) {  // 矩形

                RouteRectangle * routeItem = static_cast<RouteRectangle *> (item);

                sql.bindValue(":x1", routeItem->getId());

                if(!sql.exec())
                {
                    qDebug() << sql.lastError();
                }
                else
                {
                    qDebug() << "updated!";
                }


            }

        }


    }else{

        qDebug() << "view is not opened.";

    }
}


void MainWindow::newFile()
{

    PainterView *view = new PainterView;

    view->setScene(scene);

    view->setSceneRect(0,0,500,500);

    QMdiSubWindow * subWindow = mdiArea->addSubWindow(view);

    subWindow->resize(QSize(1200,400));

    QSqlQuery sql_query;
    QString terminal_code;

    QString select_sql = "select TERMINAL_NAME,TERMINAL_CODE,TERMINAL_LEN,DAY from TERMINAL where ENABLED = 'Y' ";
    if(!sql_query.exec(select_sql))
    {
        qDebug()<<sql_query.lastError();
        return;
    }
    else
    {
        if (sql_query.next()){
            m_current_terminal = sql_query.value(0).toString();
            terminal_code = sql_query.value(1).toString();
            m_current_terminal_len = sql_query.value(2).toInt();
            m_current_terminal_day = sql_query.value(3).toInt();
            subWindow->setWindowTitle(m_current_terminal);

            QGraphicsTextItem * text = scene->addText(m_current_terminal, QFont("Microsoft YaHei", 18, 75, true));
            text->setPos(40,15);
            text->setDefaultTextColor(QColor(0, 0, 0, 50));
        }else{

            return;

        }
    }

    XAxis * xAxis = new XAxis(m_current_terminal_len);

    xAxis->setStartPoint(QPointF(100,50));
    xAxis->setEndPoint();

    YAxis * yAxis = new YAxis(m_current_terminal_day);

    yAxis->setStartPoint(QPointF(100,50));
    yAxis->setEndPoint();

    scene->setAxis(xAxis,yAxis);

    scene->addItem(xAxis);
    scene->addItem(yAxis);

    for (int i = 1 ; i<= m_current_terminal_day - 1 ;i++){

        Axis * axis = new Axis(m_current_terminal_len);
        axis->setStartPoint(QPointF(100,50+288*i ));
        axis->setEndPoint();
        scene->addItem(axis);

    }



    //查已lock的数据
    QList<QPair <QPointF,QPointF>> list;

    select_sql = "select ROUTE_NAME,START_BERTH_POINT,SHIP_LENGTH,START_WEEK_DAY,START_TIME,END_WEEK_DAY,END_TIME,COLOUR,TEU,TYPE"
                 " from ROUTE_ARRANGEMENT,NAVIGATION where ROUTE_ARRANGEMENT.NAVIGATION_NAME = NAVIGATION.NAVIGATION_NAME and IS_LOCKED = 'Y' "
                 "and TERMINAL_NAME = '" + m_current_terminal +"'";
    if(!sql_query.exec(select_sql))
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            QString routeName = sql_query.value(0).toString();
            int startBerthPoint = sql_query.value(1).toInt();
            int shipLength = sql_query.value(2).toInt();
            int startWeekDay = sql_query.value(3).toInt();
            QString startTime = sql_query.value(4).toString();
            int endWeekDay = sql_query.value(5).toInt();
            QString endTime = sql_query.value(6).toString();
            QString color = sql_query.value(7).toString();
            QString teu = sql_query.value(8).toString();
            QString type = sql_query.value(9).toString();

            QString text = routeName+"\r\n"+ QString::fromUtf8("月均箱量 ") + teu+ "\r\n" + type + "\r\n" +
                   QString("%1").arg(startWeekDay,2,10,QLatin1Char('0'))  + "\\" + startTime + " --- "
                    +  QString("%1").arg(endWeekDay,2,10,QLatin1Char('0')) + "\\" + endTime;

            RouteRectangle * routeRectangle = new RouteRectangle(0,text,routeName);

            routeRectangle->setStartPoint(PainterHelper::convertToScreenTopLeft(startBerthPoint,startTime,startWeekDay));
            routeRectangle->setEndPoint(PainterHelper::convertToScreenBottomRight(startBerthPoint,shipLength,endTime,endWeekDay));

            routeRectangle->setToolTip(text);

            routeRectangle->setFillColor(QColor(color));

            scene->addItem(routeRectangle);

            scene->setCurrentRect(routeRectangle);

            list.append(QPair<QPointF,QPointF>(PainterHelper::convertToScreenTopLeft(startBerthPoint,startTime,startWeekDay),
                                               PainterHelper::convertToScreenBottomRight(startBerthPoint,shipLength,endTime,endWeekDay)));

        }
    }

    //未lock的数据自动排一遍

    select_sql = "select ROUTE_NAME,START_BERTH_POINT,SHIP_LENGTH,START_WEEK_DAY,START_TIME,END_WEEK_DAY,END_TIME,COLOUR,TEU,TYPE"
                 " from ROUTE_ARRANGEMENT,NAVIGATION "
                 "where ROUTE_ARRANGEMENT.NAVIGATION_NAME = NAVIGATION.NAVIGATION_NAME and IS_LOCKED = 'N' "
                 "and TERMINAL_NAME = '" + m_current_terminal +"' order by SHIP_LENGTH desc ";
    if(!sql_query.exec(select_sql))
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {

        while(sql_query.next())
        {
            QString routeName = sql_query.value(0).toString();

            int shipLength = sql_query.value(2).toInt();
            int startWeekDay = sql_query.value(3).toInt();
            QString startTime = sql_query.value(4).toString();
            int endWeekDay = sql_query.value(5).toInt();
            QString endTime = sql_query.value(6).toString();
            QString color = sql_query.value(7).toString();
            QString teu = sql_query.value(8).toString();
            QString type = sql_query.value(9).toString();

            QString text = routeName+"\r\n"+ QString::fromUtf8("月均箱量 ") + teu+ "\r\n" + type + "\r\n" +
                   QString("%1").arg(startWeekDay,2,10,QLatin1Char('0'))  + "\\" + startTime + " --- "
                    +  QString("%1").arg(endWeekDay,2,10,QLatin1Char('0')) + "\\" + endTime;

            RouteRectangle * routeRectangle = new RouteRectangle(0,text,routeName);

            QSqlQuery query_conf;

            QString conf_sql;

            conf_sql = "select START_POINT , END_POINT from BERTH where TERMINAL_CODE = '" + terminal_code + "' "
                       "and ABS(SHIP_LIMIT-" + QString::number(shipLength)+ ") = (select MIN(ABS(SHIP_LIMIT-" + QString::number(shipLength) + ")) "
                       "from BERTH where SHIP_LIMIT < "+ QString::number(shipLength) + " )  order by NICE ";

            //qDebug()<< conf_sql;

            if(!query_conf.exec(conf_sql))
            {
                qDebug()<<query_conf.lastError();
            }
            else
            {
                int flag = 0;
                QPointF topLeft;
                QPointF bottomRight;
                int startBerthPoint;
                int endBerthPoint;


                while(query_conf.next())
                {
                    startBerthPoint = query_conf.value(0).toInt();
                    endBerthPoint = query_conf.value(1).toInt();

                    topLeft = PainterHelper::convertToScreenTopLeft(startBerthPoint,startTime,startWeekDay);
                    bottomRight = PainterHelper::convertToScreenBottomRight(startBerthPoint,shipLength,endTime,endWeekDay);


                    QPair<QPointF,QPointF> pair;
                    qreal x = -1;

                    while (bottomRight.x() < (endBerthPoint + 100)){

                        foreach( pair , list) {

                            if (QRectF(topLeft,bottomRight).intersects(QRectF(pair.first,pair.second))){

                                if (x < pair.second.x()){

                                    x = pair.second.x() + 20;

                                }

                            }

                        }

                        if (x > 0){

                            topLeft = PainterHelper::convertToScreenTopLeft(x-100,startTime,startWeekDay);
                            bottomRight = PainterHelper::convertToScreenBottomRight(x-100,shipLength,endTime,endWeekDay);
                            x = -1;

                        }else{

                            break;

                        }

                    }

                    if (x < 0){
                        flag = 1 ;
                        break;
                    }
                }

                if (flag == 1){

                    routeRectangle->setStartPoint(topLeft);
                    routeRectangle->setEndPoint(bottomRight);
                    list.append(QPair<QPointF,QPointF>(topLeft,bottomRight));

                }else{

                    topLeft = PainterHelper::convertToScreenTopLeft(startBerthPoint,startTime,startWeekDay);
                    bottomRight = PainterHelper::convertToScreenBottomRight(startBerthPoint,shipLength,endTime,endWeekDay);
                    routeRectangle->setStartPoint(topLeft);
                    routeRectangle->setEndPoint(bottomRight);
                    list.append(QPair<QPointF,QPointF>(topLeft,bottomRight));

                }

            }

            routeRectangle->setToolTip(text);

            routeRectangle->setFillColor(QColor(color));

            scene->addItem(routeRectangle);

            scene->setCurrentRect(routeRectangle);

        }
    }


    connect(view, &PainterView::exit, this, &MainWindow::childWinExit);
    subWindow->show();

    newAct->setDisabled(true);
    pdfAct->setEnabled(true);
    saveAct->setEnabled(true);
    importAct->setDisabled(true);
    lockAct->setEnabled(true);
    unlockAct->setEnabled(true);
    undoAct->setEnabled(true);
}

void MainWindow::exportPDF()
{
    QPrinter printer;
    printer.setPageSize(QPrinter::A4);
    printer.setPaperSize(scene->sceneRect().size(),QPrinter::Unit::Point);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOrientation(QPrinter::Portrait);


    QString fileName;
    fileName = QFileDialog::getSaveFileName(this,
        tr("PDF"), m_current_terminal, tr("PDF Files (*.pdf)"));

    if (!fileName.isNull())
    {
        printer.setOutputFileName(fileName);
        QPainter p;
        if (!p.begin(&printer))
            return;
        scene->render(&p);
        p.end();
    }

}


void MainWindow::createStatusBar()
{
    statusBar->showMessage(tr("Ready"));


    statusBar->addPermanentWidget(progressBar);
    progressBar->setHidden(true);

}

void MainWindow::createActions()
{
    QMenu *fileMenu = this->menuBar->addMenu(QString::fromUtf8("航线靠泊"));

    windowMenu = this->menuBar->addMenu(QString::fromUtf8("窗体"));
    connect(windowMenu, &QMenu::aboutToShow, this, &MainWindow::updateWindowMenu);

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    newAct = new QAction(newIcon, QString::fromUtf8("航线靠泊图"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("berth plan"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    toolBar->addAction(newAct);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    saveAct = new QAction(saveIcon, QString::fromUtf8("保存"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save a file"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveData);
    toolBar->addAction(saveAct);
    saveAct->setDisabled(true);

    const QIcon lockIcon = QIcon::fromTheme("document-write", QIcon(":/images/lock.png"));
    lockAct = new QAction(lockIcon, QString::fromUtf8("写入"), this);
    lockAct->setStatusTip(tr("write"));
    connect(lockAct, &QAction::triggered, this, &MainWindow::lock);
    toolBar->addAction(lockAct);
    lockAct->setDisabled(true);

    const QIcon unlockIcon = QIcon::fromTheme("document-eraser", QIcon(":/images/unlock.png"));
    unlockAct = new QAction(unlockIcon, QString::fromUtf8("擦除"), this);
    unlockAct->setStatusTip(tr("eraser"));
    connect(unlockAct, &QAction::triggered, this, &MainWindow::unlock);
    toolBar->addAction(unlockAct);
    unlockAct->setDisabled(true);

    const QIcon undoIcon = QIcon::fromTheme("document-unlock", QIcon(":/images/undo.png"));
    undoAct = new QAction(undoIcon, QString::fromUtf8("全部擦除"), this);
    undoAct->setStatusTip(tr("Undo"));
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);
    toolBar->addAction(undoAct);
    undoAct->setDisabled(true);

    const QIcon queryIcon = QIcon::fromTheme("document-query", QIcon(":/images/open.png"));
    queryAct = new QAction(queryIcon, QString::fromUtf8("航线数据"), this);
    queryAct->setShortcuts(QKeySequence::Open);
    queryAct->setStatusTip(tr("Query Data"));
    connect(queryAct, &QAction::triggered, this, &MainWindow::queryData);
    fileMenu->addAction(queryAct);
    //toolBar->addAction(queryAct);

    const QIcon terminalIcon = QIcon::fromTheme("document-terminal", QIcon(":/images/terminal.png"));
    terminalAct = new QAction(terminalIcon, QString::fromUtf8("作业单位设置"), this);
    terminalAct->setStatusTip(tr("Query Terminal"));
    connect(terminalAct, &QAction::triggered, this, &MainWindow::queryTerminal);
    fileMenu->addAction(terminalAct);
    //toolBar->addAction(terminalAct);

    const QIcon navigationIcon = QIcon::fromTheme("document-navigation", QIcon(":/images/color.png"));
    navigationAct = new QAction(navigationIcon, QString::fromUtf8("航区颜色配置"), this);
    navigationAct->setStatusTip(tr("Query Navigation"));
    connect(navigationAct, &QAction::triggered, this, &MainWindow::queryNavigation);
    fileMenu->addAction(navigationAct);
    //toolBar->addAction(navigationAct);

    const QIcon pdfIcon = QIcon::fromTheme("document-terminal", QIcon(":/images/printer.png"));
    pdfAct = new QAction(pdfIcon, QString::fromUtf8("导出PDF"), this);
    pdfAct->setStatusTip(tr("PDF"));
    connect(pdfAct, &QAction::triggered, this, &MainWindow::exportPDF);
    fileMenu->addAction(pdfAct);
    toolBar->addAction(pdfAct);
    pdfAct->setDisabled(true);

    const QIcon importIcon = QIcon::fromTheme("document-import", QIcon(":/images/paste.png"));
    importAct = new QAction(importIcon, QString::fromUtf8("数据导入"), this);
    importAct->setStatusTip(tr("Import Data"));
    connect(importAct, &QAction::triggered, this, &MainWindow::importExcel);
    fileMenu->addAction(importAct);
    //toolBar->addAction(navigationAct);

    const QIcon berthIcon = QIcon::fromTheme("document-berth", QIcon(":/images/cut.png"));
    berthAct = new QAction(berthIcon, QString::fromUtf8("靠泊设置"), this);
    berthAct->setStatusTip(tr("Query Berth"));
    connect(berthAct, &QAction::triggered, this, &MainWindow::queryBerth);
    fileMenu->addAction(berthAct);

    const QIcon cleanIcon = QIcon::fromTheme("document-clean", QIcon(":/images/clean.png"));
    cleanAct = new QAction(cleanIcon, QString::fromUtf8("清理数据"), this);
    cleanAct->setStatusTip(tr("Query Navigation"));
    connect(cleanAct, &QAction::triggered, this, &MainWindow::clean);
    fileMenu->addAction(cleanAct);

    fileMenu->addSeparator();

    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    QAction *exitAct = fileMenu->addAction(exitIcon, QString::fromUtf8("退出"), qApp, &QApplication::closeAllWindows);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    fileMenu->addAction(exitAct);



    closeAct = new QAction(QString::fromUtf8("关闭"), this);
    closeAct->setStatusTip(tr("Close the active window"));
    connect(closeAct, &QAction::triggered,
            mdiArea, &QMdiArea::closeActiveSubWindow);

    closeAllAct = new QAction(QString::fromUtf8("关闭所有"), this);
    closeAllAct->setStatusTip(tr("Close all the windows"));
    connect(closeAllAct, &QAction::triggered, mdiArea, &QMdiArea::closeAllSubWindows);

    windowMenu->addAction(closeAct);
    windowMenu->addAction(closeAllAct);

    QMenu *helpMenu = menuBar->addMenu(QString::fromUtf8("帮助"));

    QAction *aboutAct = helpMenu->addAction(QString::fromUtf8("关于"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

}

void MainWindow::importExcel()
{
    QString xlsFile = QFileDialog::getOpenFileName(this,QString(),QString(),"excel(*.xls *.xlsx)");
    if(xlsFile.isEmpty())
            return;

    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", false);
    QAxObject *work_books = excel.querySubObject("WorkBooks");
    work_books->dynamicCall("Open (const QString&)", xlsFile);
    QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
    QAxObject *work_sheets = work_book->querySubObject("Sheets");
    int sheet_count = work_sheets->property("Count").toInt();

    QSqlQuery query;

    if(sheet_count > 0)
    {
        QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 1);
        QAxObject *used_range = work_sheet->querySubObject("UsedRange");

        QVariant var = used_range->dynamicCall("Value");

        QList<QList<QVariant> > list;

        castVariant2ListListVariant(var,list);

        qDebug() << list.at(1).at(3).toString();

        QAxObject *rows = used_range->querySubObject("Rows");
        int row_count = rows->property("Count").toInt();

        if (row_count > 0){

            progressBar->setHidden(false);

            progressBar->setRange(0,100);
            progressBar->setValue(0);

            query.exec(QObject::tr("delete from ROUTE_ARRANGEMENT"));

            query.prepare("insert into ROUTE_ARRANGEMENT(TERMINAL_NAME, ROUTE_NAME,ROUTE_CODE,NAVIGATION_NAME,SHIP_LENGTH, "
                          "TEU ,TYPE ,START_WEEK_DAY ,START_TIME ,END_WEEK_DAY ,END_TIME,START_BERTH_POINT,IS_LOCKED )"
                                  "values(:TERMINAL_NAME, :ROUTE_NAME, :ROUTE_CODE, :NAVIGATION_NAME, :SHIP_LENGTH,:TEU,:TYPE,"
                          ":START_WEEK_DAY,:START_TIME,:END_WEEK_DAY,:END_TIME,:START_BERTH_POINT,:IS_LOCKED )");

            for (int i =1 ;i < row_count ; i++){

                query.bindValue(0,work_sheet->querySubObject("Cells(int,int)", i+1, 1)->property("Value").toString());
                query.bindValue(1,work_sheet->querySubObject("Cells(int,int)", i+1, 2)->property("Value").toString());
                query.bindValue(2,work_sheet->querySubObject("Cells(int,int)", i+1, 3)->property("Value").toString());
                query.bindValue(3,work_sheet->querySubObject("Cells(int,int)", i+1, 4)->property("Value").toString());
                query.bindValue(4,work_sheet->querySubObject("Cells(int,int)", i+1, 5)->property("Value").toInt());
                query.bindValue(5,work_sheet->querySubObject("Cells(int,int)", i+1, 6)->property("Value").toString());
                query.bindValue(6,work_sheet->querySubObject("Cells(int,int)", i+1, 7)->property("Value").toString());
                query.bindValue(7,work_sheet->querySubObject("Cells(int,int)", i+1, 8)->property("Value").toString());

                query.bindValue(8,work_sheet->querySubObject("Cells(int,int)", i+1, 9)->property("Value").toString().trimmed());
                query.bindValue(9,work_sheet->querySubObject("Cells(int,int)", i+1, 10)->property("Value").toString());
                query.bindValue(10,work_sheet->querySubObject("Cells(int,int)", i+1, 11)->property("Value").toString().trimmed());

                if (work_sheet->querySubObject("Cells(int,int)", i+1, 12)->property("Value").toString().isEmpty()){

                    query.bindValue(11,0);
                    query.bindValue(12,"N");

                }else{

                    query.bindValue(11,work_sheet->querySubObject("Cells(int,int)", i+1, 12)->property("Value").toInt());
                    query.bindValue(12,"Y");

                }


                query.exec();

                progressBar->setValue( (row_count-1) *100 / i );

            }
        }
    }
    work_book->dynamicCall("Close(Boolean)", false);
    excel.dynamicCall("Quit(void)");
    progressBar->setHidden(true);

}

void MainWindow::childWinExit(int type)
{
     switch (type) {
     case 0:
         newAct->setEnabled(true);
         pdfAct->setDisabled(true);
         saveAct->setDisabled(true);
         importAct->setEnabled(true);
         lockAct->setDisabled(true);
         unlockAct->setDisabled(true);
         undoAct->setDisabled(true);
         break;
     default:
         break;
     }
}

void MainWindow::castVariant2ListListVariant(const QVariant &var, QList<QList<QVariant> > &res)
{
    QVariantList varRows = var.toList();
    if(varRows.isEmpty())
    {
        return;
    }
    const int rowCount = varRows.size();
    QVariantList rowData;
    for(int i=1;i<rowCount;++i)
    {
        rowData = varRows[i].toList();
        res.push_back(rowData);
    }
}


