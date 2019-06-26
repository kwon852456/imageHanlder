#include "dbdialog.h"
#include "ui_dbdialog.h"

dbDialog::dbDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dbDialog)
{
    ui->setupUi(this);
}

dbDialog::~dbDialog()
{
    delete ui;
}

void dbDialog::onListRecv(QStringList list){
    ui->lw_dbList->clear();
    ui->lw_dbList->addItems(list);
}

void dbDialog::on_lw_dbList_doubleClicked(const QModelIndex &index)
{
    emit dbItemSelected(index.row());
}
