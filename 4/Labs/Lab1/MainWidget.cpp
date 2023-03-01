#include "stdafx.h"
#include "MainWidget.h"

const std::regex MainWidget::class_regular("(class|struct){1} +([a-zA-Z][\\w_-]+)( *(:) *(public|private|protected)? +([\\w\\:<>, -]*))?");
const std::regex MainWidget::edit_regex("(([a-zA-Z][\\w+=\\<\\>*_.-]*) *= *[\\w{}*-][\\w+=\\<\\>*_.{}\\[\\]*\\:() -]*,?)+;");
const std::regex MainWidget::errors_regex("(while\\(false\\))|(if\\(false\\))");
const std::regex MainWidget::branch_regex("(if *\\(.+\\) *\\{([^\\}]*)\\})( *else *\\{([^\\}]*)\\})*");

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent) {
	ui.setupUi(this);
}

MainWidget::~MainWidget() {}

void MainWidget::find_types() {
	data[1].second.clear();
	std::string code = ui.Code->toPlainText().toStdString();
	std::smatch result;
	while (std::regex_search(code, result, class_regular)) {
		data[1].second.push_back(result[0].str());
		MainWidget::variables_types.insert(MainWidget::variables_types.size() - 2, "|");
		MainWidget::variables_types.insert(MainWidget::variables_types.size() - 2, result[2].str().c_str());
		MainWidget::functions_regular.insert(107, "|" + result[2].str());
		MainWidget::functions_regular.insert(MainWidget::functions_regular.size() - 66, "|" + result[2].str());
		code = result.suffix();
	}
}

void MainWidget::find_variables() {
	data[0].second.clear();
	std::string code = ui.Code->toPlainText().toStdString();
	std::smatch result;
	find_types();
	std::regex variables_regex(MainWidget::variables_types + std::string(var_suffix) + ";");
	while (std::regex_search(code, result, variables_regex)) {
		data[0].second.push_back(result[0].str());
		code = result.suffix();
	}
}

void MainWidget::find_functions() {
	data[3].second.clear();
	data[5].second.clear();
	std::string code = ui.Code->toPlainText().toStdString();
	std::smatch result;
	std::regex functions_regex(MainWidget::functions_regular);
	while (std::regex_search(code, result, functions_regex)) {
		std::string current_function = result[1].str() + result[7].str() + "()";
		functions.clear();
		bool exists = false;
		for (auto &[f, s] : functions) {
			if (f == current_function) {
				s++;
				exists = true;
				break;
			}
		}
		if (!exists) {
			functions.push_back({ current_function, 0 });
		}
		data[3].second.push_back(result[0].str());
		code = result.suffix();
	}
	for (const auto &[f, s] : functions) {
		data[5].second.push_back("Количество перегрузок функции: " + f + ": " + std::to_string(s));
	}
}

void MainWidget::find_edits() {
	data[2].second.clear();
	std::string code = ui.Code->toPlainText().toStdString();
	std::smatch result;
	while (std::regex_search(code, result, edit_regex)) {
		data[2].second.push_back(result[0].str());
		code = result.suffix();
	}
}

void MainWidget::find_locals() {
	data[4].second.clear();
	bool in_function = false;
	std::string local_code;
	for (const auto &cur : ui.Code->toPlainText().toStdString()) {
		if (cur == '{') {
			in_function = true;
			continue;
		} else if (cur == '}') {
			in_function = false;
			continue;
		}
		if (in_function) {
			local_code += cur;
		}
	}
	std::smatch result;
	std::regex variables_regex(MainWidget::variables_types + std::string(var_suffix) + ";");
	while (std::regex_search(local_code, result, variables_regex)) {
		data[4].second.push_back(result[0].str());
		local_code = result.suffix();
	}
}

int count_depth(std::string code, std::regex regular, int current = 0) {
	std::smatch result;
	int res = current;
	while (std::regex_search(code, result, regular)) {
		if (result[2].matched) {
			res = std::max(res, count_depth(result[2].str() + "}", regular, current + 1));
		}
		if (result[4].matched) {
			res = std::max(res, count_depth(result[4].str() + "}", regular, current + 1));
		}
		code = result.suffix();
	}
	return res;
}

void MainWidget::find_branches() {
	data[6].second.clear();
	std::string code = ui.Code->toPlainText().toStdString();
	data[6].second.push_back(std::to_string(count_depth(code, branch_regex)));
}

void MainWidget::find_errors() {
	data[7].second.clear();
	std::string code = ui.Code->toPlainText().toStdString();
	std::smatch result;
	while (std::regex_search(code, result, MainWidget::errors_regex)) {
		data[7].second.push_back(result[0].str());
		code = result.suffix();
	}
}
