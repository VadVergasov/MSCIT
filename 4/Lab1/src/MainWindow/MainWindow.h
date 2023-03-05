#pragma once

#include <QMainWindow>

#include "../MainWidget/MainWidget.h"
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   private:
    Ui::Application ui;
};
