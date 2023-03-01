#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "MainWidget.h"

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	Ui::Application ui;
};
