import numpy as np

def read_data(file_path):
    with open(file_path, 'r') as f:
        x_values = [float(x) for x in f.readline().split()]
        y_values = [float(y) for y in f.readline().split()]
    return np.array(x_values), np.array(y_values)

def least_squares_fit(x_values, y_values):
    a, b = np.polyfit(x_values, y_values, 1)
    y_pred = a * x_values + b
    
    y_std = np.std(y_values)
    a_std = np.sqrt(np.sum((y_values - y_pred) ** 2) / (len(y_values) - 2)) / np.sqrt(np.sum((x_values - np.mean(x_values)) ** 2))
    b_std = a_std * np.sqrt(np.sum(x_values ** 2) / len(x_values))

    correlation_coefficient = np.corrcoef(x_values, y_values)[0, 1]
    return a, b, y_std, a_std, b_std, correlation_coefficient

def save_results(a, b, y_std, a_std, b_std, correlation_coefficient):
    with open('result.txt', 'w') as f:
        f.write('回归直线方程: y = {:.10f}x + {:.10f}\n'.format(a, b))
        f.write('y的标准差: {:.10f}\n'.format(y_std))
        f.write('a的标准差: {:.10f}\n'.format(a_std))
        f.write('b的标准差: {:.10f}\n'.format(b_std))
        f.write('相关系数: {:.10f}\n'.format(correlation_coefficient))

if __name__ == '__main__':
    file_path = 'input.txt' # 修改为你的input.txt文件路径
    x_values, y_values = read_data(file_path)
    a, b, y_std, a_std, b_std, correlation_coefficient = least_squares_fit(x_values, y_values)
    save_results(a, b, y_std, a_std, b_std, correlation_coefficient)
