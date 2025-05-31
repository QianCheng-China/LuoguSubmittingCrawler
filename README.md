# Luogu Submitting Crawler
### by Journals Junction Hybrid
截至目前，Luogu并未开放打包下载在其上Accepted的题目代码的功能，这为许多OIer带来了不便。但Luogu Submitting Crawler可以全自动提取所有Accepted的题目，找到最优代码并保存到本地。Luogu Submitting Crawler拥有基于命令行的UI。以下是一些注意事项。
## 最新版本
Luogu Submitting Crawler的最新版本为1.0.0。
## 兼容性
Luogu Submitting Crawler仅提供Windows系统，要求curl和tar功能。

你可以在终端中键入curl和tar来查看是否拥有这两个组件。
## 所需数据
Luogu Submitting Crawler需要一些Cookie来爬取提交记录。这些Cookie分别是www.luogu.com.cn的_uid和__client_id。你可以在浏览器的开发人员页面中获取Cookie。Cookie可能会过期，请注意及时更新。

请不要将这些Cookie分享给他人。Luogu Submitting Crawler也不会将这些数据上传至网络。只会在本地保存，这样你便无需每次启动软件时键入Cookie。

**提示：目前，Cookie在本地以明文方式存储。我们将在之后的版本中引入加密机制加密Cookie。**

## 语言识别
目前，Luogu Submitting Crawler可以识别Pascal，C，C++，Python四种主流编程语言，并分别保存为.pas,.c,.cpp,.py。对于其他语言，将以.txt形式保存。我们将不断增加可以识别的语言的种类。
## 安装
Luogu Submitting Crawler无需安装。请在解压后将整个文件夹移入本地磁盘，并在桌面创建可执行文件的快捷方式后即可使用。

**注意：不要更改解压后文件夹内的任何内容，否则可能导致Luogu Submitting Crawler以意想不到的方式运行。**

**在解压时，请选择“解压到 Luogu Submitting Crawler x.x.x”。这样不会导致零散文件出现。**
## 许可证
Luogu Submitting Crawler是开源自由软件，遵循GPLv3许可证。

尽情享用吧！
