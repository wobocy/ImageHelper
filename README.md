# ImageHelper - 图片处理助手

一个基于Qt6开发的图片处理工具，支持图片缩放、压缩和格式转换等功能。

## 项目特性

### 🖼️ 图片处理功能
- **智能缩放**: 支持按像素尺寸或比例缩放图片，可选择保持宽高比
- **文件大小控制**: 通过智能质量调节达到目标文件大小
- **格式支持**: 支持常见图片格式（PNG、JPEG、BMP、GIF、TIFF）
- **批量处理**: 可对图片进行复合处理（尺寸+质量）

### 🎛️ 用户界面
- **直观的图形界面**: 基于Qt6的现代化界面设计
- **实时预览**: 支持图片缩放、平移查看
- **参数调节**: 实时调节宽度、高度、目标文件大小
- **进度指示**: 处理过程中显示进度条和状态提示

### ⚡ 性能优化
- **防抖动处理**: 参数调节时使用定时器防止频繁处理
- **异步处理**: 图片处理在后台进行，不阻塞UI
- **内存管理**: 智能管理原始图片和处理后图片的内存

## 系统要求

- **操作系统**: Windows 10/11, macOS 10.15+, Linux (Ubuntu 18.04+)
- **Qt版本**: Qt 6.0 或更高版本
- **编译器**: 
  - Windows: MSVC 2019+ 或 MinGW
  - macOS: Xcode 12+
  - Linux: GCC 9+ 或 Clang 10+
- **CMake**: 3.16 或更高版本

## 安装与编译

### 前置依赖

1. **安装Qt6**
   ```bash
   # Windows (使用Qt官方安装器)
   下载并安装 Qt 6.5+ from https://www.qt.io/download
   
   # macOS (使用Homebrew)
   brew install qt@6
   
   # Ubuntu/Debian
   sudo apt-get install qt6-base-dev qt6-tools-dev
   
   # Fedora/CentOS
   sudo dnf install qt6-qtbase-devel qt6-qttools-devel
   ```

2. **安装CMake**
   ```bash
   # Windows
   下载并安装 CMake from https://cmake.org/download/
   
   # macOS
   brew install cmake
   
   # Linux
   sudo apt-get install cmake  # Ubuntu/Debian
   sudo dnf install cmake      # Fedora/CentOS
   ```

### 编译步骤

1. **克隆或下载项目**
   ```bash
   git clone <repository-url>
   cd ImageHelper
   ```

2. **创建构建目录**
   ```bash
   mkdir build
   cd build
   ```

3. **配置和编译**
   ```bash
   # 配置项目
   cmake .. -DCMAKE_PREFIX_PATH=<Qt6安装路径>
   
   # 编译项目
   cmake --build . --config Release
   ```

   **示例Qt6路径:**
   - Windows: `C:/Qt/6.5.0/msvc2019_64`
   - macOS: `/usr/local/Cellar/qt@6/6.5.0`
   - Linux: `/usr/lib/x86_64-linux-gnu/qt6`

4. **运行程序**
   ```bash
   # Windows
   ./Release/ImageProcessor.exe
   
   # macOS/Linux
   ./ImageProcessor
   ```

## 使用说明

### 基本操作流程

1. **打开图片**
   - 点击"打开"按钮选择图片文件
   - 支持拖拽图片到程序窗口（计划功能）

2. **调整参数**
   - **尺寸调节**: 在左侧控制面板调整宽度和高度
   - **保持比例**: 勾选"锁定宽高比"保持图片比例
   - **目标大小**: 设置期望的文件大小（KB）

3. **生成处理后图片**
   - 点击"生成"按钮应用设置
   - 程序会自动优化质量以达到目标文件大小

4. **保存结果**
   - 点击"保存"按钮选择保存位置
   - 支持多种输出格式

### 高级功能

- **图片查看器**: 支持鼠标滚轮缩放、拖拽平移
- **一键重置**: 快速恢复到原始图片状态
- **实时预览**: 参数调整时实时显示效果
- **智能压缩**: 自动计算最佳质量以达到目标文件大小

## 项目结构

```
ImageHelper/
├── CMakeLists.txt          # CMake构建配置
├── main.cpp               # 程序入口点
├── mainwindow.h/.cpp      # 主窗口类
├── imageprocessor.h/.cpp  # 图片处理核心类
├── imageviewer.h/.cpp     # 图片查看器组件
└── README.md             # 项目说明文档
```

### 核心类说明

- **MainWindow**: 主窗口类，负责UI布局和用户交互
- **ImageProcessor**: 图片处理核心类，包含缩放、压缩算法
- **ImageViewer**: 自定义图片查看组件，支持缩放和平移

## 开发说明

### 架构设计

项目采用经典的MVC架构模式：
- **Model**: `ImageProcessor` 处理图片数据和算法
- **View**: `MainWindow` 和 `ImageViewer` 负责界面显示
- **Controller**: 主窗口中的信号槽机制协调数据和界面

### 关键技术特性

1. **信号槽机制**: 使用Qt的信号槽实现松耦合的组件通信
2. **定时器防抖**: 避免参数调节时的频繁计算
3. **线程安全**: 使用QMutex保证多线程环境下的数据安全
4. **二分查找算法**: 智能计算达到目标文件大小的最佳质量参数

### 贡献指南

1. Fork 项目
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

## 计划功能

- [ ] 批量处理多个文件
- [ ] 更多图片滤镜效果
- [ ] 拖拽文件支持
- [ ] 处理历史记录
- [ ] 配置文件保存用户偏好
- [ ] 多语言支持

## 许可证

本项目使用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情。

## 技术支持

如果您在使用过程中遇到问题或有功能建议，请：

1. 查看本README文档
2. 提交Issue到项目仓库
3. 联系开发团队

---

**版本**: 1.0.0  
**开发者**: CodeBuddy  
**最后更新**: 2024年