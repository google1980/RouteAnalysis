#ifndef FORMROUTEDATAQUERY_H
#define FORMROUTEDATAQUERY_H

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#include <QWidget>
#include <QSqlTableModel>
#include <QItemDelegate>

namespace Ui {
class FormRouteDataQuery;
}

class FormRouteDataQuery : public QWidget
{
    Q_OBJECT

public:
    explicit FormRouteDataQuery(QWidget *parent = 0);
    ~FormRouteDataQuery();
private slots:
    void OnSubmit();
    void OnExport();
    void OnDelete();

private:
    Ui::FormRouteDataQuery *ui;
    QSqlTableModel *model;
};

#endif // FORMROUTEDATAQUERY_H
