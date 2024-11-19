#ifndef FORMAT_H
#define FORMAT_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class format;
}

class format : public QDialog
{
    Q_OBJECT

public:
    explicit format(bool& f_format, int& prec, QWidget *parent = nullptr);
    ~format();

public slots:
    void save_box_clicked(QAbstractButton* button);

private:
    void save();
    Ui::format *ui;
    bool& fractional_format;
    int& precision;
};

#endif // FORMAT_H
