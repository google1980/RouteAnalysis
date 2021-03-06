﻿#ifndef FORMBERTHCONFIG_H
#define FORMBERTHCONFIG_H

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#include <QWidget>
#include <QSqlTableModel>

namespace Ui {
class FormBerthConfig;
}

class FormBerthConfig : public QWidget
{
    Q_OBJECT

public:
    explicit FormBerthConfig(QWidget *parent = 0);
    ~FormBerthConfig();
private slots:
    void OnSubmit();
    void OnInsert();
    void OnDelete();
private:
    Ui::FormBerthConfig *ui;
    QSqlTableModel *model;
};

#endif // FORMBERTHCONFIG_H
