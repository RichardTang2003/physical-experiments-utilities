import sympy as sp

# 定义变量
x, y, l = sp.symbols('x y lambda')

# 定义目标函数和约束条件
f = x**2 + y**2
g = x + y - 1
c = 0

# 构建拉氏函数
L = f + l * (g - c)

# 计算偏导数
dL_dx = sp.diff(L, x)
dL_dy = sp.diff(L, y)
dL_dl = sp.diff(L, l)

# 解梯度为零的方程组
critical_points = sp.solve([dL_dx, dL_dy, dL_dl], (x, y, l), dict=True)

# 判断极值点
for point in critical_points:
    x_val = point[x]
    y_val = point[y]
    l_val = point[l]
    if g.subs({x: x_val, y: y_val}) == c:  # 检查约束条件
        f_val = f.subs({x: x_val, y: y_val})
        print(f"极值点: ({x_val}, {y_val}), 函数值: {f_val}")

