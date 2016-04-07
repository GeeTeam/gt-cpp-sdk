=========================SDK依赖======================================

1、sdk依赖libcurl库（一般机器都是安装有的，没有源码安装即可）

在demo/lib目录下直接解压安装 或者从http://curl.haxx.se/下载
  $tar xzf curl-7.46.0.tar.gz  
  $cd curl-7.46.0
  $./configure
  $make
  $sudo make install   
运行以上命令安装curl

=========================SDK使用=======================================

2、使用本SDK（在cplus_sdk目录下）只需要将sdk整个源码目录拷贝到自己的项目中
例如：demo中的源码路径为 demo/src，需要将sdk拷贝到demo/src/中，并且修改自己的makefile



==========================demo依赖=====================================

3、demo程序依赖cgicc（处理cgi）

在demo/lib目录下直接解压安装 或者从ftp://ftp.gnu.org/gnu/cgicc/下载
  $tar xzf cgicc-3.2.16.tar.gz 
  $cd cgicc-3.2.16
  $./configure
  $make
  $sudo make install   
运行以上命令安装cgicc

=========================demo运行=======================================

4、进入demo/src目录下
   $make
   $sudo make install   

make 命令将编译demo cgi源码，并打包到demo/dist目录下，方便发布到apache
make install 命令将dst整个文件到拷贝到apache配置的cgi-bin下（需要根据自己apache配置修改makefile）


浏览直接访问 http://127.0.0.1/cgi-bin/login_page.cgi即可

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
apache 配置cgi 自行百度