# 物理实验计算工具
基于Python的物理实验计算工具。适用于大量数据的处理。

Made with ❤️ by illlights && ChatGPT4.

Website: [illlights.com](https://www.illlights.com)



## data_processing.py

简介：基本的数据处理，先运用3σ去除坏值并计算数据的不确定度。

使用方法：

1. 在同一目录下建立一个input.txt，第一行输入实验数据一个数据一个空格，第二行输入仪器误差和置信系数，如果不写置信系数默认为根号3.

2. 运行data_processing.py

   （如果在文件在C盘需要以管理员身份运行）

3. 结果会输出在result.txt



## successiveDifferences.py

简介：逐差法

使用方法：

1. 在同一目录下建立一个input.txt，把实验数据全部放输进去，一个数据一个空格。

2. 运行successiveDifferences.py

   （如果在文件在C盘需要以管理员身份运行）

3. 结果会输出在result.txt



## leastSquaresFitting.py（尚未检验正确性）

简介：最小二乘法拟合

使用方法：

1. 输入从input.txt中获取

   格式： 第一行写x坐标，第二行写y坐标，一一对应 

2. 运行leastSquaresFitting.py

3. 输出到result.txt: 

   第一行：回归直线方程 

   第二、三、四行：y、a、b的标准差 

   第五行：相关系数



## lagrange_multiplier_optimization.py（不建议使用）

简介：二元拉格朗日乘子法解决条件约束问题

使用方法：

1. 打开lagrange_multiplier_optimization.py源文件
2. 修改定义目标函数的函数内容
3. 用python运行，结果会显示在程序输出
