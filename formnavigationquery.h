#ifndef FORMNAVIGATIONQUERY_H
#define FORMNAVIGATIONQUERY_H

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#include <QWidget>
#include <QSqlTableModel>
#include <QItemDelegate>
#include "mysqltablemodel.h"


namespace Ui {
class FormNavigationQuery;
}

class FormNavigationQuery : public QWidget
{
    Q_OBJECT

public:
    explicit FormNavigationQuery(QWidget *parent = 0);
    ~FormNavigationQuery();

private slots:
    void OnSubmit();
    void OnInsert();
    void OnDelete();

private:
    Ui::FormNavigationQuery *ui;
    MySqlTableModel *model;

};

#endif // FORMNAVIGATIONQUERY_H
