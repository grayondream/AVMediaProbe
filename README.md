# AVMediaProbe简介
![Github stars](https://img.shields.io/github/stars/grayondream/AVMediaProbe.svg) [![](https://img.shields.io/github/last-commit/grayondream/AVMediaProbe?style=flat)](https://github.com/grayondream/AVMediaProbe/commits/develop) [![](https://img.shields.io/github/license/grayondream/AVMediaProbe?style=flat)](https://github.com/grayondream/AVMediaProbe/blob/develop/LICENSE.md) ![](https://img.shields.io/badge/software_status-work_on_progress-red)

&emsp;&emsp;AVMediaProbe是一款使用C++开发，基于Qt，FFmpeg的音视频媒体文件流检测软件，能够解析出媒体文件中的媒流体信息，显示出对应流的详细信息。其大体的结构类似于MediaInfo，能够显示媒体中每个流的详细信息。开发此软件是为了解决一些MediaInfo，ffprobe这类软件本身对于一些媒体信息检测存在错误的问题。

# 安装
&emsp;&emsp;AVMediaProbe大部分以来的三方库是源码依赖，比如spdlog和configor，clone下仓库之后直接更新submodule之后即可。需要准备的是FFmpeg动态库，需要将FFmpeg动态库存放到仓库中的```thirdparty/ffmpeg```目录下，对于Windows系统需要按照bin和lib的目录结构分别存放dll和lib，其他系统则需要存放到lib目录下，以及设置```src/env.cmake```中的```QT_PATH```为你机器上的QT的目录。一切准备好之后直接根据各端的情况执行cmake生成工程即可。

&emsp;&emsp;运行时需要注意的问题，如果是使用visual studio编译程序因为workspace的问题，需要修改```GlobalConfig.h```中的Debug宏，其他平台则不需要。

&emsp;&emsp;一切准备好后直接编译即可，cmake脚本会将FFmpeg动态库和Qt动态库拷贝到对应的执行目录。

# 支持内容
&emsp;&emsp;下面描述的信息一般媒体文件只有部分，而不是全部：
- 媒体文件详细信息：
  - 文件大小；
  - 媒体时长；
  - 媒体格式；
  - 媒体格式置信度（FFmpeg给媒体格式的估分，越高置信度越高）；
  - 媒体总体码率；
  - 媒体开始时间；
  - 元数据；
- 常规流详细信息（即所有流都会有的信息）：
  - 媒体流索引；
  - 媒体类型；
  - 媒体编码格式；
  - 码率；
  - 流大小；
  - 流时长；
  - 流开始时间；
  - 元数据；
  - 帧数量；
  - 详细帧信息：
    - 帧大小；
    - DTS；
    - PTS；
    - 帧时长；
    - 帧位置；
    - 是否为关键帧；
- 视频流详细信息：
  - 视频高度；
  - 视频宽度；
  - 视频旋转角度；
  - 显示宽高比（DAR）；
  - 像素宽高比（PAR）；
  - 采样宽高比（SAR）；
  - 变换矩阵；
  - X方向是否Flip；
  - Y方向是否Flip；
  - 帧率模式；
  - 平均帧率；
  - 最大帧率；
  - 色彩空间；
  - 分片采样；
  - 位深；
  - 色彩原色；
  - 颜色空间；
  - 传输特性；
  - 色彩范围；
- 音频流详细信息：
  - 采样率；
  - 通道数；
  - 通道布局；
- 支持导出的文件格式：
  - json


# Dependencies
- [Qt](https://github.com/qt/qt5)
- [FFmpeg](https://github.com/FFmpeg)
- [spdlog](https://github.com/gabime/spdlog)
- [configor](https://github.com/jinzhu/configor)
