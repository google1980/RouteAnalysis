#include "formnavigationquery.h"
#include "ui_formnavigationquery.h"
#include "delegate.h"

FormNavigationQuery::FormNavigationQuery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormNavigationQuery)
{
    ui->setupUi(this);
    model = new MySqlTableModel(this,QSqlDatabase(),1);
    model->setTable("NAVIGATION");
    model->setEditStrategy(MySqlTableModel::OnManualSubmit);

    model->select();

    model->setHeaderData(0, Qt::Horizontal, QString::fromUtf8("航区"));
    model->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("颜色"));
    model->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("透明度(0~255)"));

    ui->tableView->setModel(model);

    ui->tableView->setColumnWidth(1, 150);

    connect(ui->submitButton, SIGNAL(clicked()), this, SLOT(OnSubmit()));
    connect(ui->insertButton, SIGNAL(clicked()), this, SLOT(OnInsert()));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(OnDelete()));

    ui->tableView->setItemDelegate(new MyComboDelegate(this,0,1));


    setAttribute(Qt::WA_DeleteOnClose);
}

FormNavigationQuery::~FormNavigationQuery()
{
    delete ui;
}

void FormNavigationQuery::OnSubmit()
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

void FormNavigationQuery::OnInsert()
{
    int rowNum = model->rowCount();
    model->insertRow(rowNum);

}

void FormNavigationQuery::OnDelete()
{
    int curRow = ui->tableView->currentIndex().row();
    model->removeRow(curRow);
}

