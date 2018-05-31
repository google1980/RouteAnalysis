#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QtSql>

QSqlError initDb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data.db");

    if (!db.open())
        return db.lastError();
    //QSqlQuery query;
    //query.exec(QObject::tr("drop table BERTH"));
    //query.exec(QObject::tr("drop table ROUTE_ARRANGEMENT"));
    //query.exec(QObject::tr("drop table TERMINAL"));
    //query.exec(QObject::tr("drop table NAVIGATION"));

    //query.exec(QObject::tr("create table BERTH (TERMINAL_CODE vchar, SHIP_LIMIT integer, START_POINT integer, END_POINT integer , NICE integer)"));
    //query.exec(QObject::tr("create table TERMINAL (TERMINAL_NAME vchar,TERMINAL_CODE vchar, ENABLED vchar)"));
    //query.exec(QObject::tr("create table ROUTE_ARRANGEMENT (TERMINAL_NAME vchar, ROUTE_NAME vchar,ROUTE_CODE vchar,NAVIGATION_NAME vchar,SHIP_LENGTH integer, "
    //                           "TEU vchar,TYPE vchar, START_WEEK_DAY vchar,START_TIME vchar,END_WEEK_DAY vchar,END_TIME vchar,START_BERTH_POINT integer,IS_LOCKED vchar DEFAULT 'N')"));
    //query.exec(QObject::tr("create table NAVIGATION (NAVIGATION_NAME vchar,COLOUR vchar)"));


    //query.exec(QObject::tr("ALTER TABLE ROUTE_ARRANGEMENT ADD COLUMN IS_LOCKED vchar DEFAULT 'N' "));
    //query.exec(QObject::tr("drop table BERTH"));
    //query.exec(QObject::tr("create table BERTH (TERMINAL_CODE vchar, SHIP_LIMIT integer, START_POINT integer, END_POINT integer , NICE integer)"));
    //query.exec(QObject::tr("insert into BERTH values ('BLCTMS',300,500,1000,1)"));
    //query.exec(QObject::tr("insert into BERTH values ('BLCTMS',0,0,1500,1)"));
    //query.exec(QObject::tr("insert into NAVIGATION values ('3','3')"));
    //query.exec(QObject::tr("insert into NAVIGATION values ('4','1')"));
    //query.exec(QObject::tr("insert into NAVIGATION values ('5','2')"));
    //query.exec(QObject::tr("insert into NAVIGATION values ('6','3')"));

    //query.exec(QObject::tr("drop table ROUTE_ARRANGEMENT"));
    //query.exec(QObject::tr("create table TERMINAL (TERMINAL_NAME vchar,TERMINAL_CODE vchar, ENABLED vchar)"));

    //query.exec(QObject::tr("insert into TERMINAL values ('BLCTMS','','')"));
    //query.exec(QObject::tr("insert into TERMINAL values ('BLCT','','')"));
    //query.exec(QObject::tr("insert into TERMINAL values ('BLCT2','','')"));
    //query.exec(QObject::tr("insert into TERMINAL values ('BLCT3','','')"));
    //query.exec(QObject::tr("insert into TERMINAL values ('BLCTZS','','')"));
    //query.exec(QObject::tr("insert into TERMINAL values ('YZCT','','')"));


    //QSqlQuery query;

    //query.exec(QObject::tr("drop table ROUTE_ARRANGEMENT"));
    //query.exec(QObject::tr("create table ROUTE_ARRANGEMENT (TERMINAL_NAME vchar, ROUTE_NAME vchar,ROUTE_CODE vchar,NAVIGATION_NAME vchar,SHIP_LENGTH integer, "
    //                       "TEU vchar,TYPE vchar, START_WEEK_DAY vchar,START_TIME vchar,END_WEEK_DAY vchar,END_TIME vchar,START_BERTH_POINT integer)"));
    //query.exec(QObject::tr("insert into ROUTE_ARRANGEMENT values ('BLCTMS','OCEAN美东5线(SAX/AWE3/ECX1/SAX)','AWE5-OA', '',366,450 ,'周一','00:00','周一','19:00')"));
    //query.exec(QObject::tr("insert into ROUTE_ARRANGEMENT values ('BLCTMS','马士基中南美一线','AC1', '' ,266, 20 ,'周一','06:00','周一','23:00')"));
    //query.exec(QObject::tr("insert into ROUTE_ARRANGEMENT values ('BLCTMS','OCEAN美西2线(PE2/CPNW/CECP/TPX)','PNW2-OA', '',334, 800 ,'周一','00:00','周一','22:00')"));

    return QSqlError();
}

#endif // DATASOURCE_H
