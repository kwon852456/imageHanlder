#ifndef DBDIALOG_H
#define DBDIALOG_H

#include <QDialog>
#include <QDebug>


namespace Ui {
class dbDialog;
}

class dbDialog : public QDialog
{
    Q_OBJECT

public:
    explicit dbDialog(QWidget *parent = nullptr);
    ~dbDialog();

signals:
    void dbItemSelected(int idx);

public slots:
    void onListRecv(QStringList list);




private slots:
    void on_lw_dbList_doubleClicked(const QModelIndex &index);

private:
    Ui::dbDialog *ui;
};

#endif // DBDIALOG_H
