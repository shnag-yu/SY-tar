# SY-tar
a small experimental compress tool for files and directories

一个实验性的压缩解压工具，适用于文件与文件夹
## Build
由`CmakeLists.txt`构建
## Run
`main`程序通过命令行指示工作模式

+ `comp` 
  
    ```
    >>> ./main comp source dest
    ```
    将`source`被压缩对象相对路径（可以是文件或者文件夹）压缩到压缩文件`dest`中。
+ `uncomp`

    ```
    >>> ./main uncomp source
    ```
    将由SY-tar程序压缩的压缩文件`source`解压到当前目录。
+ `display`
  
    ```
    >>> ./main display source
    ```
    不解压压缩文件的情况下展示原文件的结构。
