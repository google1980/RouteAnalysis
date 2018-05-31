#include "formberthconfig.h"
#include "ui_formberthconfig.h"

FormBerthConfig::FormBerthConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormBerthConfig)
{
    ui->setupUi(this);
    model = new QSqlTableModel(this);
    model->setTable("BERTH");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model->select();

    model->setHeaderData(0, Qt::Horizontal, QString::fromUtf8("作业单位"));
    model->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("船长限制"));
    model->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("靠泊起点"));
    model->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("靠泊终点"));
    model->setHeaderData(4, Qt::Horizontal, QString::fromUtf8("优先值"));

    ui->tableView->setModel(model);

    //ui->tableView->setItemDelegate();

    connect(ui->submitButton, SIGNAL(clicked()), this, SLOT(OnSubmit()));
    connect(ui->insertButton, SIGNAL(clicked()), this, SLOT(OnInsert()));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(OnDelete()));


    setAttribute(Qt::WA_DeleteOnClose);
}

FormBerthConfig::~FormBerthConfig()
{
    delete ui;
}

void FormBerthConfig::OnInsert()
{
    int rowNum = model->rowCount();
    model->insertRow(rowNum);

}

void FormBerthConfig::OnSubmit()
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

void FormBerthConfig::OnDelete()
{
    int curRow = ui->tableView->currentIndex().row();
    model->removeRow(curRow);
}



