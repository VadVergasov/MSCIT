#pragma once

#include <QFile>
#include <QFileDialog>
#include <QWidget>
#include <regex>
#include <utility>
#include <vector>

#include "../Parser/parser.h"
#include "ui_MainWidget.h"

class MainWidget : public QWidget {
    Q_OBJECT

   public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

   private:
    Ui::Lab1 ui;

    QString file;

   private slots:
    void on_chooseFile_clicked();
    void on_parseButton_clicked();
};
