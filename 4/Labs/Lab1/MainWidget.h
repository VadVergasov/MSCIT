#pragma once

#include <QWidget>
#include "ui_MainWidget.h"
#include <vector>
#include <utility>
#include <regex>

class MainWidget : public QWidget {
	Q_OBJECT

public:
	MainWidget(QWidget *parent = nullptr);
	~MainWidget();

private:
	Ui::Lab1 ui;

	std::vector<std::pair<std::string, std::vector<std::string>>> data;
	const static std::regex class_regular;
	std::string variables_types = "(const )?(signed |unsigned )?(char|short|int|long long|bool|float|double|void \\*|std::string|size_t)";
	std::string functions_regular = "((friend )?(const )?(signed |unsigned )?(char|short|int|long long|bool|float|double|void|std::string|size_t))( +| *\\*+ +| +\\* *| *&{0,2} +| +&{0,2} *)([a-zA-Z][\\w=+!-]*) *\\(((const ){0,1}(((signed |unsigned ){0,1}(char|short|int|long long))|(bool|float|double|void \\*|std::string|size_t))(( +| *\\*+ +| +\\* *| *&{0,2} +| +&{0,2} *)[a-zA-Z]?[\\w]*,*)+)*\\)";
	constexpr static std::string_view var_suffix = "(( +| *\\*+ +| +\\* *| *&{0,2} +| +&{0,2} *)([a-zA-Z][\\w+=\\<\\>*_.-]*,*))+";
	const static std::regex edit_regex, errors_regex, branch_regex;
	std::vector<std::pair<std::string, int>> functions;

	void find_types();
	void find_variables();
	void find_functions();
	void find_edits();
	void find_locals();
	void find_branches();
	void find_errors();

private slots:
	void on_chooseFile_clicked();
	void on_Code_textChanged();
};
