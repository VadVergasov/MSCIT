#pragma once

#include <QByteArray>
#include <QFile>
#include <QFileDialog>
#include <QWidget>
#include <iostream>
#include <utility>
#include <vector>

#include "../Parser/parser.h"
#include "ui_MainWidget.h"

class MainWidget : public QWidget {
    Q_OBJECT

   public:
    MainWidget(QWidget* parent = nullptr);
    ~MainWidget() = default;

   private:
    Ui::Lab2 ui;
    QString absoluteLabelText;
    QString relativeLabelText;
    QString maxdepthLabelText;

    QString file;

   private slots:
    void on_chooseFile_clicked();
    void on_parseButton_clicked();
};
