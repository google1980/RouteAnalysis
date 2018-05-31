#include "formterminalquery.h"
#include "ui_formterminalquery.h"
#include "delegate.h"

FormTerminalQuery::FormTerminalQuery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTerminalQuery)
{
    ui->setupUi(this);
    model = new QSqlTableModel(this);
    model->setTable("TERMINAL");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model->select();

    model->setHeaderData(0, Qt::Horizontal, QString::fromUtf8("作业单位名称")  );
    model->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("作业单位代码"));
    model->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("码头长度"));
    model->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("显示天数"));
    model->setHeaderData(4, Qt::Horizontal, QString::fromUtf8("是否当前")  );

    ui->tableView->setModel(model);

    //ui->tableView->setItemDelegate();
    ui->tableView->setItemDelegate(new MyComboDelegate(this,2,4));

    connect(ui->submitButton, SIGNAL(clicked()), this, SLOT(OnSubmit()));
    connect(ui->insertButton, SIGNAL(clicked()), this, SLOT(OnInsert()));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(OnDelete()));



    setAttribute(Qt::WA_DeleteOnClose);
}

FormTerminalQuery::~FormTerminalQuery()
{
    delete ui;
}

void FormTerminalQuery::OnSubmit()
{
    //int curRow = ui->tableView->currentIndex().row();
    //model->removeRow(curRow);
    model->database().transaction();
    if (model->submitAll()) {
        model->database().commit();
    }
    else {
        model->database().rollback();
    }

}

void FormTerminalQuery::OnInsert()
{
    int rowNum = model->rowCount();
    model->insertRow(rowNum);

}

void FormTerminalQuery::OnDelete()
{
    int curRow = ui->tableView->currentIndex().row();
    model->removeRow(curRow);
}

