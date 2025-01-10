# miniToadb

教程发布于公众号：开源无限

# 编译

词法和语法解析代码依赖 flex和bison，需要安装使用。

为了方便大家使用，上传的代码中已经用flex和bison编译成C代码程序文件，可以直接使用gcc进行编译。

如果安装了flex和bison，则可以直接执行make进行编译，make clean清理编译结果。

如果没有安装上述两个工具，可以修改makefile，跳过两个工具的编译，并且修改clean中，避免删除生成的c代码文件。

如果不小心make clean，可以从git库下载缺少的文件。


