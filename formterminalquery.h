#ifndef FORMTERMINALQUERY_H
#define FORMTERMINALQUERY_H

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#include <QWidget>
#include <QSqlTableModel>

namespace Ui {
class FormTerminalQuery;
}

class FormTerminalQuery : public QWidget
{
    Q_OBJECT

public:
    explicit FormTerminalQuery(QWidget *parent = 0);
    ~FormTerminalQuery();
private slots:
    void OnSubmit();
    void OnInsert();
    void OnDelete();

private:

    Ui::FormTerminalQuery *ui;
    QSqlTableModel *model;
};

#endif // FORMTERMINALQUERY_H
