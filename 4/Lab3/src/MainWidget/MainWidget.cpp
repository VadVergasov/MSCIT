#include "MainWidget.h"

#include <QMessageBox>

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
    ui.setupUi(this);
    result_label = ui.result->text();
}

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
    Parser parser;
    char const* argv[6];
    argv[0] = "./Lab3";
    argv[1] = "tmp.cpp";
    argv[2] = "--";
    argv[3] = "-std=c++20";
    argv[4] = "-isystem";
    argv[5] = "/usr/include/clang/12/include";
    ui.spenTable->clearContents();
    ui.spenTable->setColumnCount(0);
    ui.chepTable->clearContents();
    ui.result->setText(result_label);
    try {
        auto variables = parser.parse(6, argv);
        size_t sum = 0, input = 0, modify = 0, control = 0, junk = 0;
        for (const auto& [address, val] : variables) {
            ui.spenTable->insertColumn(0);
            ui.spenTable->setItem(
                0, 0, new QTableWidgetItem(QString(val.name.c_str())));
            ui.spenTable->setItem(1, 0,
                                  new QTableWidgetItem(QString::number(
                                      val.locations.size() - 1)));
            sum += val.locations.size() - 1;
            QString text = "";
            switch (val.type) {
                case record::Type::INPUT:
                    if (ui.chepTable->item(1, 0)) {
                        text = ui.chepTable->item(1, 0)->text();
                    }
                    input++;
                    ui.chepTable->setItem(
                        1, 0,
                        new QTableWidgetItem(text + val.name.c_str() + ", "));
                    break;
                case record::Type::MODIFY:
                    if (ui.chepTable->item(1, 1)) {
                        text = ui.chepTable->item(1, 1)->text();
                    }
                    modify++;
                    ui.chepTable->setItem(
                        1, 1,
                        new QTableWidgetItem(text + val.name.c_str() + ", "));
                    break;
                case record::Type::CONTROL:
                    if (ui.chepTable->item(1, 2)) {
                        text = ui.chepTable->item(1, 2)->text();
                    }
                    control++;
                    ui.chepTable->setItem(
                        1, 2,
                        new QTableWidgetItem(text + val.name.c_str() + ", "));
                    break;
                case record::Type::JUNK:
                    if (ui.chepTable->item(1, 3)) {
                        text = ui.chepTable->item(1, 3)->text();
                    }
                    junk++;
                    ui.chepTable->setItem(
                        1, 3,
                        new QTableWidgetItem(text + val.name.c_str() + ", "));
                    break;
            }
        }
        ui.chepTable->setItem(2, 0,
                              new QTableWidgetItem(QString::number(input)));
        ui.chepTable->setItem(2, 1,
                              new QTableWidgetItem(QString::number(modify)));
        ui.chepTable->setItem(2, 2,
                              new QTableWidgetItem(QString::number(control)));
        ui.chepTable->setItem(2, 3,
                              new QTableWidgetItem(QString::number(junk)));
        ui.spenTable->insertColumn(ui.spenTable->columnCount());
        ui.spenTable->setItem(0, ui.spenTable->columnCount() - 1,
                              new QTableWidgetItem("Суммарный спен программы"));
        ui.spenTable->setItem(1, ui.spenTable->columnCount() - 1,
                              new QTableWidgetItem(QString::number(sum)));
        ui.spenTable->resizeColumnsToContents();
        ui.spenTable->resizeRowsToContents();
        ui.chepTable->resizeColumnsToContents();
        ui.chepTable->resizeRowsToContents();
        ui.result->setText(
            result_label + " 1 * " + QString::number(input) + " + 2 * " +
            QString::number(modify) + " + 3 * " + QString::number(control) +
            " + 0.5 * " + QString::number(junk) + " = " +
            QString::number(input + 2 * modify + 3 * control + 0.5 * junk));
    } catch (const CompileException& error) {
        QMessageBox::critical(
            this, "Ошибка",
            "Введенный код содержит ошибки и не может быть скомпилирован!");
    }
    code.remove();
}
