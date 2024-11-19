#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "expr_parser.h"
#include "format.h"

#include <QButtonGroup>
#include <QAbstractButton>
#include <QStack>
#include <QString>
#include <QLineEdit>
#include <QComboBox>
#include <QScrollArea>
#include <QDialog>
#include <QTimer>
#include <QScrollBar>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Calculator");

    connect_buttons();
    set_colors();

    connect(ui->combo_box_mem_select, &QComboBox::currentIndexChanged, this, &MainWindow::memory_changed);
    connect(ui->line_edit_main, &QLineEdit::returnPressed, this, &MainWindow::evaluate);

    ui->widget_history->layout()->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    display_MR();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connect_buttons()
{
    connect(ui->button_equals, &QAbstractButton::clicked, this, &MainWindow::evaluate);

    connect(ui->button_0, &QAbstractButton::clicked, this, &MainWindow::button_0_clicked);
    connect(ui->button_1, &QAbstractButton::clicked, this, &MainWindow::button_1_clicked);
    connect(ui->button_2, &QAbstractButton::clicked, this, &MainWindow::button_2_clicked);
    connect(ui->button_3, &QAbstractButton::clicked, this, &MainWindow::button_3_clicked);
    connect(ui->button_4, &QAbstractButton::clicked, this, &MainWindow::button_4_clicked);
    connect(ui->button_5, &QAbstractButton::clicked, this, &MainWindow::button_5_clicked);
    connect(ui->button_6, &QAbstractButton::clicked, this, &MainWindow::button_6_clicked);
    connect(ui->button_7, &QAbstractButton::clicked, this, &MainWindow::button_7_clicked);
    connect(ui->button_8, &QAbstractButton::clicked, this, &MainWindow::button_8_clicked);
    connect(ui->button_9, &QAbstractButton::clicked, this, &MainWindow::button_9_clicked);
    connect(ui->button_plus, &QAbstractButton::clicked, this, &MainWindow::button_plus_clicked);
    connect(ui->button_minus, &QAbstractButton::clicked, this, &MainWindow::button_minus_clicked);
    connect(ui->button_power, &QAbstractButton::clicked, this, &MainWindow::button_power_clicked);
    connect(ui->button_divide, &QAbstractButton::clicked, this, &MainWindow::button_divide_clicked);
    connect(ui->button_multiply, &QAbstractButton::clicked, this, &MainWindow::button_multiply_clicked);

    connect(ui->button_open_parent, &QAbstractButton::clicked, this, &MainWindow::button_open_parenthesis_clicked);
    connect(ui->button_close_parent, &QAbstractButton::clicked, this, &MainWindow::button_close_parenthesis_clicked);

    connect(ui->button_AC, &QAbstractButton::clicked, this, &MainWindow::button_AC_clicked);
    connect(ui->button_back, &QAbstractButton::clicked, this, &MainWindow::button_backspace_clicked);
    connect(ui->button_decimal, &QAbstractButton::clicked, this, &MainWindow::button_dot_clicked);

    connect(ui->button_MS, &QAbstractButton::clicked, this, &MainWindow::button_MS_clicked);
    connect(ui->button_Mplus, &QAbstractButton::clicked, this, &MainWindow::button_Mplus_clicked);
    connect(ui->button_MC, &QAbstractButton::clicked, this, &MainWindow::button_MC_clicked);
    connect(ui->button_MR, &QAbstractButton::clicked, this, &MainWindow::button_MR_clicked);

    connect(ui->button_settings, &QAbstractButton::clicked, this, &MainWindow::open_settings);
    connect(ui->button_copy, &QAbstractButton::clicked, this, &MainWindow::copy_into_clipboard);

}

void MainWindow::set_colors()
{
    ui->button_equals->setStyleSheet("background-color: #0e6420;");
    ui->button_back->setText("⌫");
    ui->button_back->setStyleSheet("background-color: #ff5300;");
    ui->button_AC->setStyleSheet("background-color: #ff0000;");
}

void MainWindow::add_text(const QString& t)
{
    ui->line_edit_main->setText(ui->line_edit_main->text() + t);
}

void MainWindow::display_result()
{
    if (rational_format) {
        ui->line_edit_main->setText(current_result);
    } else {
        ui->line_edit_main->setText(QString::number(static_cast<double>(current_result)));
    }
}


std::vector<size_t> find_mismatched_parentheses(const QString& input)
{
    QStack<size_t> stack;
    std::vector<size_t> mismatched_indices;

    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '(')
            stack.push(i);
        else if (input[i] == ')') {
            if (!stack.empty())
                stack.pop();
            else
                mismatched_indices.push_back(i);
        }
    }

    while (!stack.empty()) {
        mismatched_indices.push_back(stack.top());
        stack.pop();
    }

    return mismatched_indices;
}

QString highlight_indices(const QString& input, std::vector<size_t>&& indices)
{
    std::sort(indices.begin(), indices.end());

    QString result;
    size_t  last_pos = 0;

    for (size_t i : indices) {
        result += input.mid(last_pos, i - last_pos);
        result += "<font color='red'>" % input[i] % "</font>";
        last_pos = i + 1;
    }
    result += input.mid(last_pos);

    return result;
}


void MainWindow::evaluate()
{
    auto history = get_history_layout();
    for (int i = 0; i < error_lines; ++i) {
        QLayoutItem* item = history->takeAt(history->count() - 1);
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        } else {
            delete item;
        }
    }
    error_lines = 0;

    if (ui->line_edit_main->text() == "") {
        current_result = 0;
        display_result();
        return;
    }

    try
    {
        expr_parser parser{ui->line_edit_main->text()};

        std::vector<Token> tokens = parser.shunting_yard();

        current_result = calculate_RPN(tokens, precision);
    } catch (expr_eval_error& e) {
        history->addWidget(new QLabel(e.mess(), ui->widget_history));
        error_lines++;

        QString invalid_text = ui->line_edit_main->text();
        Token t = e.get_token();
        if (invalid_text != "") {
            QString highlighted_text;
            if (e.mess().contains("Mismatched parentheses")) { // Very bad design :(
                highlighted_text = highlight_indices(invalid_text, find_mismatched_parentheses(invalid_text));
            } else if (e.mess().contains("Invalid expression")) {
                highlighted_text = "<font color='red'>" + invalid_text + "</font>";
            }
            else {
                highlighted_text = highlight_indices(invalid_text, {t.pos});
            }
            QLabel* err_highlight = new QLabel(highlighted_text, ui->widget_history);
            history->addWidget(err_highlight);
            error_lines++;
        }
        move_history_scrollbar_down();
        return;
    }
    display_result();
    add_result_to_history();
}



void MainWindow::button_0_clicked() { add_text("0"); }
void MainWindow::button_1_clicked() { add_text("1"); }
void MainWindow::button_2_clicked() { add_text("2"); }
void MainWindow::button_3_clicked() { add_text("3"); }
void MainWindow::button_4_clicked() { add_text("4"); }
void MainWindow::button_5_clicked() { add_text("5"); }
void MainWindow::button_6_clicked() { add_text("6"); }
void MainWindow::button_7_clicked() { add_text("7"); }
void MainWindow::button_8_clicked() { add_text("8"); }
void MainWindow::button_9_clicked() { add_text("9"); }

void MainWindow::button_plus_clicked()      { add_text("+"); }
void MainWindow::button_minus_clicked()     { add_text("-"); }
void MainWindow::button_multiply_clicked()  { add_text("*"); }
void MainWindow::button_divide_clicked()    { add_text("/"); }
void MainWindow::button_dot_clicked()       { add_text("."); }
void MainWindow::button_power_clicked()     { add_text("^"); }

void MainWindow::button_open_parenthesis_clicked()  { add_text("("); }
void MainWindow::button_close_parenthesis_clicked() { add_text(")"); }

void MainWindow::button_backspace_clicked() { ui->line_edit_main->backspace(); }
void MainWindow::button_AC_clicked()        { ui->line_edit_main->clear(); }

int MainWindow::memory_index() const { return ui->combo_box_mem_select->currentIndex(); }

void MainWindow::display_MR() { ui->label_mem_result->setText(mem[memory_index()]); }

void MainWindow::button_MS_clicked()
{
    mem[memory_index()] = current_result;
    display_MR();
}

void MainWindow::button_Mplus_clicked()
{
    current_result += mem[memory_index()];
    display_result();
}

void MainWindow::button_MR_clicked()
{
    add_text(mem[ui->combo_box_mem_select->currentIndex()]);
}
void MainWindow::button_MC_clicked()
{
    mem[ui->combo_box_mem_select->currentIndex()] = 0;
    display_MR();
}

void MainWindow::memory_changed(int index)
{
    display_MR();
}

void MainWindow::change_format(QAbstractButton* button)
{
    display_result();
}


void MainWindow::open_settings()
{
    format window(rational_format, precision);
    window.exec();
}

void MainWindow::copy_into_clipboard()
{
    QClipboard* clip = QApplication::clipboard();
    if (rational_format)
        clip->setText(current_result);
    else
        clip->setText(QString::number(static_cast<double>(current_result)));
}

// TODO get_result_QString

void MainWindow::add_result_to_history()
{
    QVBoxLayout *history_layout = qobject_cast<QVBoxLayout*>(ui->widget_history->layout());

    QLabel* num_label;


    if (rational_format) {
        num_label  = new QLabel(current_result, ui->widget_history);
    } else {
        num_label = new QLabel(QString::number(static_cast<double>(current_result)), ui->widget_history);
    }
    num_label->setAlignment(Qt::AlignRight);
    history_layout->addWidget(num_label);

    move_history_scrollbar_down();

}


void MainWindow::move_history_scrollbar_down()
{
    // Why does this nor work without the timer?
    // Race condition avoidance at its finest
    QApplication::processEvents();

    auto scroll_area = ui->scroll_area_history;

    QTimer::singleShot(0, scroll_area, [scroll_area]() {
        scroll_area->verticalScrollBar()->setValue(scroll_area->verticalScrollBar()->maximum());
    });
}

QLayout* MainWindow::get_history_layout() { return ui->widget_history->layout(); }
