#include "MainWidget.h"

#include <QMessageBox>

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
    ui.setupUi(this);
    dictionaryLabelText = ui.dictionaryLabel->text();
    lengthLabelText = ui.lengthLabel->text();
    volumeLabelText = ui.volumeLabel->text();
}

MainWidget::~MainWidget() {}

void MainWidget::on_chooseFile_clicked() {
    file = QFileDialog::getOpenFileName(this, "Выберите файл", "",
                                        "Source files (*.cpp)");
    if (file.isEmpty()) {
        return;
    }
    QFile input = QFile(file, this);
    input.open(QIODevice::ReadOnly);
    QString content = input.readAll();
    input.close();
    ui.Code->setPlainText(content);
}

void MainWidget::on_parseButton_clicked() {
    QFile code("tmp.cpp");
    code.open(QIODeviceBase::WriteOnly);
    code.write(ui.Code->toPlainText().toStdString().c_str(),
               qstrlen(ui.Code->toPlainText().toStdString().c_str()));
    code.close();
    ParserHalstead parser;
    char const* argv[6];
    argv[0] = "./Lab1";
    argv[1] = "tmp.cpp";
    argv[2] = "--";
    argv[3] = "-std=c++20";
    argv[4] = "-isystem";
    argv[5] = "/usr/include/clang/12/include";
    ui.resultTable->setRowCount(0);
    try {
        auto [operators, operands, N1, N2, n1, n2, N, n, V] = parser.parse(6, argv);
        ui.resultTable->setRowCount(std::max(operators.size(), operands.size()) +
                                    1);
        ui.dictionaryLabel->setText(dictionaryLabelText + QString::number(n1) +
                                    " + " + QString::number(n2) + " = " +
                                    QString::number(n));
        ui.lengthLabel->setText(lengthLabelText + QString::number(N1) + " + " +
                                QString::number(N2) + " = " + QString::number(N));
        ui.volumeLabel->setText(volumeLabelText + QString::number(N) + " * log(" +
                                QString::number(n) + ") = " + QString::number(V));
        size_t row = 0;
        for (const auto& op : operators) {
            ui.resultTable->setItem(row, 0,
                                    new QTableWidgetItem(QString::number(row + 1)));
            ui.resultTable->setItem(
                row, 1, new QTableWidgetItem(QString(op.first.c_str())));
            ui.resultTable->setItem(
                row, 2, new QTableWidgetItem(QString::number(op.second)));
            row++;
        }
        ui.resultTable->setItem(
            row, 0, new QTableWidgetItem("n1 = " + QString::number(row)));
        ui.resultTable->setItem(
            row, 2, new QTableWidgetItem("N1 = " + QString::number(N1)));
        row = 0;
        for (const auto& op : operands) {
            if (op.first == " " || op.first.empty()) {
                continue;
            }
            ui.resultTable->setItem(row, 3,
                                    new QTableWidgetItem(QString::number(row + 1)));
            ui.resultTable->setItem(
                row, 4, new QTableWidgetItem(QString(op.first.c_str())));
            ui.resultTable->setItem(
                row, 5, new QTableWidgetItem(QString::number(op.second)));
            row++;
        }
        ui.resultTable->setItem(
            row, 3, new QTableWidgetItem("n2 = " + QString::number(row)));
        ui.resultTable->setItem(
            row, 5, new QTableWidgetItem("N2 = " + QString::number(N2)));
        ui.resultTable->horizontalHeader()->setStretchLastSection(true);
        ui.resultTable->resizeColumnsToContents();
        ui.resultTable->resizeRowsToContents();
    } catch (const CompileException& error) {
        QMessageBox::critical(
            this, "Ошибка",
            "Введенный код содержит ошибки и не может быть скомпилирован!");
    }
    code.remove();
}
