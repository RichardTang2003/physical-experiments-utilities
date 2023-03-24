#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <tuple>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>
#include <iomanip>




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

std::tuple<std::vector<double>, double, double> read_data(const std::string& file_path) {
    std::vector<double> data_list;
    double instrument_error = 0;
    double confidence_coefficient = std::sqrt(3);

    std::ifstream file(file_path);
    if (!file) {
        std::cerr << "无法打开文件: " << file_path << std::endl;
        exit(1);
    }

    std::string line;
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::copy(std::istream_iterator<double>(iss),
            std::istream_iterator<double>(),
            std::back_inserter(data_list));
    }

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

    file.close();

    return std::make_tuple(data_list, instrument_error, confidence_coefficient);
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



void save_results(const std::vector<double>& good_values, const std::vector<double>& outliers, double mean, double std_dev, double instrument_error, double confidence_coefficient) {
    std::vector<double> individual_std_devs;
    for (const double& val : good_values) {
        individual_std_devs.push_back(std::abs(val - mean));
    }

    std::ofstream f("result.txt");
    if (!f) {
        std::cerr << "无法打开文件: result.txt" << std::endl;
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
    f << "S = " << std_dev / std::sqrt(good_values.size() - 1) << '\n';
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

    f.close();
}


int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::string file_path;
    std::cout << "请输入input.txt文件路径: ";
    std::cin >> file_path;

    std::vector<double> data;
    double instrument_error, confidence_coefficient;
    std::tie(data, instrument_error, confidence_coefficient) = read_data(file_path);

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

    save_results(good_values, outliers, mean, std_dev, instrument_error, confidence_coefficient);

    return 0;
}


