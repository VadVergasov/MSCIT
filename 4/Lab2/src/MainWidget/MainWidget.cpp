#include "MainWidget.h"

#include <QMessageBox>

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
    ui.setupUi(this);
    absoluteLabelText = ui.absoluteLabel->text();
    relativeLabelText = ui.relativeLabel->text();
    maxdepthLabelText = ui.maxdepthLabel->text();
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
    ParserJilb parser;
    char const* argv[6];
    argv[0] = "./Lab2";
    argv[1] = "tmp.cpp";
    argv[2] = "--";
    argv[3] = "-std=c++20";
    argv[4] = "-isystem";
    argv[5] = "/usr/include/clang/12/include";
    try {
        auto [absolute, relative, max_depth] = parser.parse(6, argv);
        ui.absoluteLabel->setText(absoluteLabelText + QString::number(absolute));
        ui.relativeLabel->setText(
            relativeLabelText + QString::number(absolute) + " / " +
            QString::number(MyRecursiveASTVisitor::operators_count) + " = " +
            QString::number(relative));
        ui.maxdepthLabel->setText(maxdepthLabelText + QString::number(max_depth));
    } catch (const CompileException& error) {
        QMessageBox::critical(
            this, "Ошибка",
            "Введенный код содержит ошибки и не может быть скомпилирован!");
    }
    code.remove();
}
