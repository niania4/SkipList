

# 项目中文件

* main.cpp 包含skiplist.h使用跳表进行数据操作
* build cmake编译目录
* skiplist.h 跳表实现     
* makefile 编译脚本
* store 跳表输出目录 
* stress_test.cpp 压力测试脚本
* CMakeLists.txt cmake命令文件


# 提供接口

* insertElement（插入数据）
* deleteElement（删除数据）
* searchElement（查询数据）
* displayList（展示已存数据）
* dumpFile（数据落盘）
* height（跳表当前高度）
* size（返回数据规模）


# 存储引擎数据表现

## 插入操作

跳表树高：16 

|插入数据规模（万条） |耗时（秒） | 
|---|---|
|10 |0.128654 |
|50 |0.583948 |
|100 |1.21802 |




# 项目运行方式

将文件夹在vscode中打开，直接执行即可（前提电脑上必须装有cmake和mingw64）
若想切换执行的文件，修改CMakeLists.txt





