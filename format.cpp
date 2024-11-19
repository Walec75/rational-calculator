#include "format.h"
#include "ui_format.h"

#include <QDialogButtonBox>

format::format(bool& f_format, int& prec, QWidget *parent)
    : fractional_format(f_format), precision(prec), QDialog(parent)
    , ui(new Ui::format)
{
    ui->setupUi(this);

    setWindowTitle("Settings");

    ui->format_group->setExclusive(true);

    if (fractional_format) {
        ui->button_rational->setChecked(true);
    } else {
        ui->button_decimal->setChecked(true);
    }

    ui->spinBox->setValue(precision);

    connect(ui->save_box, &QDialogButtonBox::clicked, this, &format::save_box_clicked);
}

format::~format()
{
    delete ui;
}

void format::save_box_clicked(QAbstractButton* button)
{
    QDialogButtonBox::StandardButton sb = ui->save_box->standardButton(button);
    if (sb == QDialogButtonBox::Save) {
        save();
        close();
    } else if (sb == QDialogButtonBox::Discard) {
        close();
    }
}


void format::save()
{
    if (ui->button_rational->isChecked()) {
        fractional_format = true;
    } else {
        fractional_format = false;
    }

    precision = ui->spinBox->value();
}
