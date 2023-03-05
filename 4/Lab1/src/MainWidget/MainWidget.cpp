#include "MainWidget.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) { ui.setupUi(this); }

MainWidget::~MainWidget() {}

void MainWidget::on_chooseFile_clicked() {
    file = QFileDialog::getOpenFileName(this, "Выберете файл", "",
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
    ParserHalstead parser;
    char const* argv[5];
    argv[0] = std::string("./Lab1").c_str();
    argv[1] = file.toStdString().c_str();
    argv[2] = std::string("--").c_str();
    argv[3] = std::string("-isystem").c_str();
    argv[4] = std::string("/usr/include/clang/12/include").c_str();
    parser.parse(5, argv);
}
