# Luogu Submitting Crawler
### by Journals Junction Hybrid
截至目前，Luogu并未开放打包下载在其上Accepted的题目代码的功能，这为许多OIer带来了不便。但Luogu Submitting Crawler可以全自动提取所有Accepted的题目，找到最优代码并保存到本地。Luogu Submitting Crawler拥有基于命令行的UI。以下是一些注意事项。
## 最新版本
Luogu Submitting Crawler的最新版本为2.0.0。

**注意：Luogu Submitting Crawler 1.x.x的生命周期已终止**

## 兼容性
Luogu Submitting Crawler仅提供Windows系统，要求curl和tar功能。

**从2.0.0开始，Luogu Submitting Crawler无需tar组件。**

你可以在终端中键入curl和tar来查看是否拥有这两个组件。
## 所需数据
Luogu Submitting Crawler需要一些Cookie来爬取提交记录。这些Cookie分别是www.luogu.com.cn的_uid和__client_id。你可以在浏览器的开发人员页面中获取Cookie。Cookie可能会过期，请注意及时更新。

请不要将这些Cookie分享给他人。Luogu Submitting Crawler也不会将这些数据上传至网络。只会在本地保存，这样你便无需每次启动软件时键入Cookie。

**提示：从版本1.1.1开始，Cookie在本地将进行加密存储。此版本之前的版本将以明文存储。**

## 语言识别
目前，Luogu Submitting Crawler可以识别Pascal，C，C++，Python，Java，Rust，Go，Haskell，PHP编程语言并存为相应文件扩展名。对于其他语言，将以.txt形式保存。我们将不断增加可以识别的语言的种类。

提交答案将保存为.ans文件。
## 安装
Luogu Submitting Crawler无需安装。请在解压后将整个文件夹移入本地磁盘，并在桌面创建可执行文件的快捷方式后即可使用。

**注意：不要更改解压后文件夹内的任何内容，否则可能导致Luogu Submitting Crawler以意想不到的方式运行。**

**在解压时，请选择“解压到 Luogu Submitting Crawler x.x.x”。这样不会导致零散文件出现。**

## 错误修复程序
从1.1.2开始，Luogu Submitting Crawler内置了错误修复程序。在首页，你可以按Ctrl+E查看错误日志，或按Ctrl+R执行还原操作。

## 更新检查程序
从2.0.0开始，Luogu Submitting Crawler内置了更新检查功能已便检查是否有新的更新。


## 许可证和条件
Luogu Submitting Crawler是开源自由软件，遵循GPLv3许可证。


本软件属于网络爬虫范畴，请在使用时注意遵守有关规则。

尽情享用吧！
