#include "stdafx.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);
	setCentralWidget(new MainWidget(parent));
}

MainWindow::~MainWindow() {}
