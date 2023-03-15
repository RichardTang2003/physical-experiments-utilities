import numpy as np

def read_data(file_path):
    with open(file_path, 'r') as f:
        data = [float(x) for x in f.readline().split()]
    return np.array(data)

def calculate_mean_difference(data):
    differences = np.diff(data)
    mean_difference = np.mean(differences)
    return mean_difference

def save_result(mean_difference):
    with open('result.txt', 'w') as f:
        f.write('逐差法计算的一阶差平均值: {:.10f}'.format(mean_difference))

if __name__ == '__main__':
    file_path = 'input.txt' # 修改为你的input.txt文件路径
    data = read_data(file_path)
    mean_difference = calculate_mean_difference(data)
    save_result(mean_difference)
