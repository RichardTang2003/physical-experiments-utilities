#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <tuple>
#include <iomanip>
#include <limits>
#include <Windows.h>

double t683(const std::vector<double>& data) {
	double t = 0;
	int len = data.size();
	if (len == 2) {
		t = 1.84;
	}
	else if (len == 3) {
		t = 1.32;
	}
	else if (len == 4) {
		t = 1.20;
	}
	else if (len == 5) {
		t = 1.14;
	}
	else if (len >= 6) {
		t = 1;
	}
	return t;
}

std::vector<std::tuple<std::vector<double>, double, double>> read_data(const std::string& file_path) {
	std::vector<std::tuple<std::vector<double>, double, double>> data_sets;

	std::ifstream file(file_path);
	if (!file) {
		std::cerr << "无法打开文件: " << file_path << "\n输入q退出程序..." << std::endl;
		char ch;
		while (std::cin.get(ch) && ch != '\n');
		std::cin.get();
		exit(1);
	}

	std::string line;
	while (std::getline(file, line)) {
		std::vector<double> data_list;
		double instrument_error = 0;
		double confidence_coefficient = std::sqrt(3);

		std::istringstream iss(line);
		std::copy(std::istream_iterator<double>(iss),
			std::istream_iterator<double>(),
			std::back_inserter(data_list));

		if (std::getline(file, line)) {
			std::istringstream iss(line);
			std::vector<double> values{ std::istream_iterator<double>(iss),
									   std::istream_iterator<double>() };
			if (values.size() > 0) {
				instrument_error = values[0];
			}
			if (values.size() > 1) {
				confidence_coefficient = values[1];
			}
		}

		data_sets.push_back(std::make_tuple(data_list, instrument_error, confidence_coefficient));
	}

	file.close();

	return data_sets;
}



std::pair<std::vector<double>, std::vector<double>> remove_outliers(const std::vector<double>& data) {
	std::vector<double> all_outliers;
	std::vector<double> good_values(data);

	while (true) {
		double sum = 0;
		for (const double& val : good_values) {
			sum += val;
		}
		double mean = sum / good_values.size();

		double variance = 0;
		for (const double& val : good_values) {
			variance += (val - mean) * (val - mean);
		}
		double std_dev = std::sqrt(variance / (good_values.size() - 1)) * t683(good_values);

		double lower_bound = mean - 3 * std_dev;
		double upper_bound = mean + 3 * std_dev;

		std::vector<double> new_good_values;
		std::vector<double> outliers;
		for (const double& val : good_values) {
			if (lower_bound <= val && val <= upper_bound) {
				new_good_values.push_back(val);
			}
			else {
				outliers.push_back(val);
			}
		}

		if (outliers.empty()) {
			break;
		}

		all_outliers.insert(all_outliers.end(), outliers.begin(), outliers.end());
		good_values = new_good_values;
	}

	return { good_values, all_outliers };
}



void save_results(const std::vector<double>& good_values, const std::vector<double>& outliers, double mean, double std_dev, double instrument_error, double confidence_coefficient, double S) {
	std::vector<double> individual_std_devs;
	for (const double& val : good_values) {
		individual_std_devs.push_back(std::abs(val - mean));
	}

	std::ofstream f("result.txt", std::ios::app);
	if (!f) {
		std::cerr << "无法写入result.txt文件，输入q退出程序..." << std::endl;
		char ch;
		while (std::cin.get(ch) && ch != '\n');
		std::cin.get();
		exit(1);
	}

	f << std::fixed << std::setprecision(6);

	f << "剩余的值: ";
	std::copy(good_values.begin(), good_values.end(), std::ostream_iterator<double>(f, " "));
	f << '\n';

	f << "剔除的值: ";
	std::copy(outliers.begin(), outliers.end(), std::ostream_iterator<double>(f, " "));
	f << '\n';

	f << "最终的平均数: " << mean << '\n';
	f << "S = " << S << '\n';
	f << "σ = " << std_dev << ", 3σ = " << 3 * std_dev << '\n';

	f << "每个最终值的标准差: ";
	std::copy(individual_std_devs.begin(), individual_std_devs.end(), std::ostream_iterator<double>(f, " "));
	f << '\n';

	double a_class_uncertainty = std_dev / std::sqrt(good_values.size());
	double b_class_uncertainty = instrument_error / confidence_coefficient;
	double combined_uncertainty = std::sqrt(a_class_uncertainty * a_class_uncertainty + b_class_uncertainty * b_class_uncertainty);

	f << "A类不确定度（标准偏差）: " << a_class_uncertainty << '\n';
	f << "B类不确定度: " << b_class_uncertainty << '\n';
	f << "不确定度: " << combined_uncertainty << '\n';
	f << "相对不确定度: " << 100 * combined_uncertainty / mean << "%\n\n";
	f << "测量结果为: \n       x = " << mean << "±" << combined_uncertainty << '\n';
	f << "     Δx/x = " << 100 * combined_uncertainty / mean << "%\n";
	f << "---------------------" << '\n';
	f.close();
}


int main() {
	SetConsoleOutputCP(CP_UTF8);
	std::string file_path;
	std::cout << "--------------使用说明-----------------" << std::endl;
	std::cout << "输入文件请使用纯文本格式文件，建议使用txt文件." << std::endl;
	std::cout << "第一行填写多个数据，数据间用空格隔开." << std::endl;
	std::cout << "第二行可选参数：仪器误差 仪器误差置信系数，默认值分别为0和根号3." << std::endl;
	std::cout << "支持多组数据同时处理，但是输入多组数据时必须两行都输入，以两行为一个数据组（置信系数可省略）." << std::endl;
	std::cout << "--------------------------------------" << std::endl;
	std::cout << "请输入数据文件名:";
	std::cin >> file_path;

	std::vector<double> data;
	double instrument_error, confidence_coefficient;
	auto data_sets = read_data(file_path);

	std::size_t i = 0;
	for (i = 0; i < data_sets.size(); i++) {
		auto data_set = data_sets[i];
		std::tie(data, instrument_error, confidence_coefficient) = data_set;

		std::vector<double> good_values, outliers;
		std::tie(good_values, outliers) = remove_outliers(data);

		double sum = 0;
		for (const double& val : good_values) {
			sum += val;
		}
		double mean = sum / good_values.size();

		double variance = 0;
		for (const double& val : good_values) {
			variance += (val - mean) * (val - mean);
		}
		double std_dev = std::sqrt(variance / (good_values.size() - 1)) * t683(good_values);
		double S = std::sqrt(variance / (good_values.size() - 1));

		save_results(good_values, outliers, mean, std_dev, instrument_error, confidence_coefficient, S);
	}
	std::cout << "result.txt已生成，请在文件管理器中查看。输入q退出... " << std::endl;
	std::cout << "如果文件夹中存在result.txt，数据将会附加到原始数据之后，请注意区分。" << std::endl;
	std::cout << "输入q退出... ";
	char ch;
	while (std::cin.get(ch) && ch != '\n');
	std::cin.get();
	return 0;
}


