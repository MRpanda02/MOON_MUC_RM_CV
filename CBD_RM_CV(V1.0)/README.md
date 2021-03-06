# Project Clementine

MUC Computer Vision Project for Robomaster 2021

## 使用介绍



### 下载须知

本仓库主要模块使用 Git Submodule 管理，获取代码时需要注意。

建议使用 `git clone`：

```bash
git clone https://github.com/MUC-RM-CV/project-clementine.git --recurse-submodules
```



如果点击 Download Zip 下载源代码包，会发现下载后的包里**不包含子模块的代码**，因此还需要手动下载子模块的代码，并将 Zip 包解压缩后，放置在项目对应的路径下。

### 依赖

- OpenCV 4
- C++ 17

## 子模块介绍

### 图像源 I/O 封装类：image

⚙️ Git Submodule

仓库地址：[MUC-RM-CV/mindvision-cam-wrapper](https://github.com/MUC-RM-CV/mindvision-cam-wrapper)

**注**：需要手动安装迈德威视相机 SDK，详见项目目录下 README 使用说明

### 能量机关识别：windmill

⚙️ Git Submodule

仓库地址：[MUC-RM-CV/windmill-detect](https://github.com/MUC-RM-CV/windmill-detect)

使用说明见目录下 README

算法介绍详见文档（工程内 `doc` 目录）

### 装甲识别：armor



## 知识库 / 文档

[C++ 浮点数转为字符串并保留一定小数位数](doc/float-to-chars-fixed.md)


