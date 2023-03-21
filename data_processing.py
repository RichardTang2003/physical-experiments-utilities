import numpy as np

def t683(data):
    t = 0
    len = np.size(data)
    if len == 2:
      t = 1.84
    elif len == 3:
      t = 1.32
    elif len == 4:
      t = 1.20
    elif len == 5:
      t = 1.14
    elif len >= 6:
      t = 1

    return t



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

def remove_outliers(data):
    all_outliers = []
    
    while True:
        mean = np.mean(data)
        std_dev = np.std(data, ddof=1) * t683(data)
        lower_bound = mean - 3 * std_dev
        upper_bound = mean + 3 * std_dev

        good_values = [x for x in data if lower_bound <= x <= upper_bound]
        outliers = [x for x in data if x not in good_values]

        if len(outliers) == 0:
            break

        all_outliers.extend(outliers)
        data = np.array(good_values)

    return np.array(good_values), np.array(all_outliers)

def save_results(good_values, outliers, mean, std_dev, instrument_error, confidence_coefficient):
    individual_std_devs = [abs(x - mean) for x in good_values]

    with open('result.txt', 'w') as f:
        f.write('剩余的值: ' + ' '.join(map(str, good_values)) + '\n')
        f.write('剔除的值: ' + ' '.join(map(str, outliers)) + '\n')
        f.write('最终的平均数: ' + str(mean) + '\n')
        f.write('σ = '+ str(std_dev) + ', 3σ = '+ str(3*std_dev) + '\n')
        f.write('每个最终值的标准差: ' + ' '.join(map(str, individual_std_devs)) + '\n')
        a_class_uncertainty = std_dev / np.sqrt(np.size(good_values))
        b_class_uncertainty = instrument_error / confidence_coefficient
        combined_uncertainty = np.sqrt(a_class_uncertainty**2 + b_class_uncertainty**2)
        f.write('A类不确定度（标准偏差）: ' + str(a_class_uncertainty) + '\n')
        f.write('B类不确定度: ' + str(b_class_uncertainty) + '\n')
        f.write('不确定度: ' + str(combined_uncertainty) + '\n')
        f.write('相对不确定度: ' + str(combined_uncertainty / (100*mean)) + '%\n')
        f.write('\n')
        f.write('测量结果为: \n       x = ' + str(mean) + '±' + str(combined_uncertainty) + '\n')
        f.write('     Δx/x = ' + str(combined_uncertainty / (100 * mean)) + '%\n')
        

if __name__ == '__main__':
    file_path = 'input.txt' # 修改为你的input.txt文件路径
    data, instrument_error, confidence_coefficient = read_data(file_path)
    good_values, outliers = remove_outliers(data)
    mean = np.mean(good_values)
    std_dev = np.std(good_values, ddof=1)  * t683(data)
    save_results(good_values, outliers, mean, std_dev, instrument_error, confidence_coefficient)
