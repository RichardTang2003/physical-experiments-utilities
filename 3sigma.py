import numpy as np

def read_data(file_path):
    with open(file_path, 'r') as f:
        data_str = f.read()
    data_list = [float(x) for x in data_str.split()]
    return np.array(data_list)

def remove_outliers(data):
    all_outliers = []
    
    while True:
        mean = np.mean(data)
        std_dev = np.std(data)
        lower_bound = mean - 3 * std_dev
        upper_bound = mean + 3 * std_dev

        good_values = [x for x in data if lower_bound <= x <= upper_bound]
        outliers = [x for x in data if x not in good_values]

        if len(outliers) == 0:
            break

        all_outliers.extend(outliers)
        data = np.array(good_values)

    return np.array(good_values), np.array(all_outliers)

def save_results(good_values, outliers, mean, std_dev):
    individual_std_devs = [abs(x - mean) for x in good_values]

    with open('result.txt', 'w') as f:
        f.write('剩余的值: ' + ' '.join(map(str, good_values)) + '\n')
        f.write('剔除的值: ' + ' '.join(map(str, outliers)) + '\n')
        f.write('最终的平均数和σ: ' + str(mean) + ' ' + str(std_dev) + '\n')
        f.write('每个good_values的标准差: ' + ' '.join(map(str, individual_std_devs)) + '\n')

if __name__ == '__main__':
    file_path = 'input.txt' # 修改为你的input.txt文件路径
    data = read_data(file_path)
    good_values, outliers = remove_outliers(data)
    mean = np.mean(good_values)
    std_dev = np.std(good_values)
    save_results(good_values, outliers, mean, std_dev)
