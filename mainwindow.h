#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "rational.h"

#include <QMainWindow>
#include <array>
#include <QAbstractButton>
#include <QLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;
    void add_text(const QString& t);
    std::array<rational, 5> mem = {0, 0, 0, 0, 0};
    rational current_result = 0;
    bool rational_format = true;
    int precision = 3;
    int error_lines = 0;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void evaluate();
    void button_0_clicked();
    void button_1_clicked();
    void button_2_clicked();
    void button_3_clicked();
    void button_4_clicked();
    void button_5_clicked();
    void button_6_clicked();
    void button_7_clicked();
    void button_8_clicked();
    void button_9_clicked();

    void button_plus_clicked();
    void button_minus_clicked();
    void button_multiply_clicked();
    void button_divide_clicked();

    void button_open_parenthesis_clicked();
    void button_close_parenthesis_clicked();

    void button_backspace_clicked();
    void button_AC_clicked();

    void button_dot_clicked();
    void button_power_clicked();

    void button_MS_clicked();
    void button_Mplus_clicked();
    void button_MC_clicked();
    void button_MR_clicked();
    void memory_changed(int index);

    void change_format(QAbstractButton* button);

    void open_settings();
    void copy_into_clipboard();

private:
    void set_colors();
    void move_history_scrollbar_down();
    void connect_buttons();
    void display_result();
    void display_MR();
    int memory_index() const;
    void add_result_to_history();
    QLayout* get_history_layout();
};
#endif // MAINWINDOW_H
