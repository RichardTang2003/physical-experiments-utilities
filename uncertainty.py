import numpy as np

def read_data(file_path):
    with open(file_path, 'r') as f:
        lines = f.readlines()
    data_list = [float(x) for x in lines[0].split()]
    if len(lines) > 1:
        values = lines[1].split()
        instrument_error = float(values[0]) if len(values) > 0 else 0
        confidence_coefficient = float(values[1]) if len(values) > 1 else np.sqrt(3)
    else:
        instrument_error, confidence_coefficient = 0, np.sqrt(3)
    return np.array(data_list), instrument_error, confidence_coefficient

def calculate_uncertainty(data, instrument_error, confidence_coefficient):
    mean = np.mean(data)
    std_dev = np.std(data)
    a_class_uncertainty = std_dev / np.sqrt(len(data))
    b_class_uncertainty = instrument_error
    combined_uncertainty = np.sqrt(a_class_uncertainty**2 + (b_class_uncertainty/confidence_coefficient)**2)
    return mean, a_class_uncertainty, b_class_uncertainty, combined_uncertainty

def save_results(mean, a_class_uncertainty, b_class_uncertainty, combined_uncertainty):
    with open('result.txt', 'w') as f:
        f.write('平均值: {:.10f}\n'.format(mean))
        f.write('A类不确定度: {:.10f}\n'.format(a_class_uncertainty))
        f.write('B类不确定度: {:.10f}\n'.format(b_class_uncertainty/confidence_coefficient))
        f.write('合成不确定度: {:.10f}\n'.format(combined_uncertainty))

if __name__ == '__main__':
    file_path = 'input.txt' # 修改为你的input.txt文件路径
    data_list, instrument_error, confidence_coefficient = read_data(file_path)
    mean, a_class_uncertainty, b_class_uncertainty, combined_uncertainty = calculate_uncertainty(data_list, instrument_error, confidence_coefficient)
    save_results(mean, a_class_uncertainty, b_class_uncertainty, combined_uncertainty)