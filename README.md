# Qt UI Kit

一个现代化的 Qt 组件库，提供高质量的 UI 组件和主题系统，帮助开发者快速构建美观的桌面应用程序。

## ✨ 特性

- 🎨 **主题系统** - 支持浅色/深色主题切换，统一的设计令牌（颜色、字号、圆角、间距等）
- 🧩 **丰富组件** - 提供按钮、输入框、选择器、反馈组件等常用 UI 组件
- 📦 **即插即用** - 模块化设计，可按需引入组件
- 🎯 **类型安全** - 完整的 C++ 类型系统和 Qt 属性支持
- 📱 **响应式** - 支持不同尺寸和状态（hover/focus/active/disabled/loading）
- 🌍 **国际化友好** - 文本与样式分离，便于多语言支持

## 🚀 快速开始

### 环境要求

- CMake 3.16+
- Qt 5.15+ 或 Qt 6.x
- C++17 编译器（MSVC 2022 / MinGW / GCC / Clang）

### 构建项目

#### 使用 CMake Presets（推荐）

```bash
# 配置项目（选择一个预设）
cmake --preset msvc-x64      # MSVC 2022 + Qt 6
cmake --preset mingw-x64     # MinGW + Qt 6

# 构建
cmake --build --preset msvc-x64-debug
```

#### 手动配置

```bash
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/compiler ..
cmake --build .
```

### 运行示例

```bash
# 构建后运行 Gallery 演示程序
./build/src/gallery/qt-ui-gallery.exe
```

## 📦 组件列表

### 基础控件
- **Button** - 按钮组件（Primary/Secondary/Ghost/Danger/Icon）

### 表单组件
- **LineEdit** - 输入框
- **ComboBox** - 下拉选择框
- **Switch** - 开关选择器

### 反馈组件
- **MessageBar** - 消息条
- **Dialog** - 对话框
- **Toast** - 轻提示

### Shell 组件
- **AppShellWindow** - 应用外壳窗口
- **TitleBar** - 标题栏
- **NavigationPanel** - 导航面板

## 🎯 开发路线

### M0 - 基础零件 ✅ 进行中
- [x] 主题系统框架
- [x] 设计令牌（Design Tokens）
- [ ] 完善状态规范（hover/focus/active/disabled/loading）
- [ ] 统一图标体系

### M1 - 核心高频组件
- [ ] Button 家族完善
- [ ] Input 家族（搜索框/密码框/数字输入/文本域）
- [ ] Select 家族（多选/自动完成）
- [ ] Form 体系（FormItem、校验规则）
- [ ] 反馈组件完善

### M2 - 结构与数据展示
- [ ] 表格组件
- [ ] 列表与卡片
- [ ] 标签与徽标
- [ ] 分页器

### M3 - Shell 与业务化扩展
- [ ] 标题栏插件位
- [ ] 导航栏行为完善
- [ ] 音乐播放控件
- [ ] "汽水音乐"风格 Demo

详细规划请查看 [docs/todo.md](docs/todo.md)

## 🛠️ 开发指南

### 项目结构

```
qt-ui/
├── src/
│   ├── uikit/              # UI 组件库
│   │   ├── core/           # 核心系统（主题、令牌）
│   │   ├── controls/       # 基础控件
│   │   ├── form/           # 表单组件
│   │   ├── feedback/       # 反馈组件
│   │   ├── shell/          # Shell 组件
│   │   ├── styles/         # QSS 样式文件
│   │   └── resources/      # 资源文件
│   └── gallery/            # 演示程序
├── docs/                   # 文档
├── CMakeLists.txt
└── CMakePresets.json       # CMake 预设配置
```

### 使用组件

```cpp
#include <uikit/controls/ui_button.h>
#include <uikit/controls/ui_label.h>
#include <uikit/core/theme_manager.h>

// 创建按钮
uikit::UiButton* button = new uikit::UiButton("Click me", this);
button->setVariant(uikit::UiButton::Variant::Primary);
button->setSize(uikit::UiButton::Size::Large);

// 标题/正文层级（对应 base.qss 中 QLabel[uiTextRole=…]）
uikit::UiLabel* title = new uikit::UiLabel(QStringLiteral("标题"), this);
title->setTextRole(uikit::UiLabel::TextRole::H1);

// 切换主题（会写入 QSettings，便于下次启动恢复）
uikit::ThemeManager::instance()->setTheme(uikit::ThemeManager::ThemeType::Dark);
```

### 自定义主题

```cpp
// 浅色基底 + 自定义主色；第二参数为 true 时基于深色基底
auto tokens = uikit::ThemeTokenFactory::createThemeWithBrand(QColor(QStringLiteral("#1890ff")), false);
// 或使用预设：uikit::BrandPreset::Purple / Red / Green / Orange / Blue
// auto tokens = uikit::ThemeTokenFactory::createThemeWithBrand(uikit::BrandPreset::Purple, false);

uikit::ThemeManager::instance()->setCustomTokens(tokens);
uikit::ThemeManager::instance()->setTheme(uikit::ThemeManager::ThemeType::Custom);
```

## 🤝 贡献指南

欢迎贡献代码、报告问题或提出建议！

1. Fork 本仓库
2. 创建特性分支 (`git checkout -b feature/amazing-feature`)
3. 提交更改 (`git commit -m 'Add some amazing feature'`)
4. 推送到分支 (`git push origin feature/amazing-feature`)
5. 创建 Pull Request

### 代码规范

- 遵循 C++17 标准
- 使用 Qt 命名约定
- 组件需包含完整的属性、信号和状态
- 每个组件需提供最小示例和 API 文档

## 📄 许可证

本项目采用 MIT 许可证 - 详见 [LICENSE](LICENSE) 文件

## 🙏 致谢

灵感来源于：
- [Ant Design](https://ant.design/)
- [Material Design](https://material.io/)
- [Qt Widgets](https://doc.qt.io/qt-6/qtwidgets-index.html)

---

**注意**：本项目目前处于早期开发阶段，API 可能会发生变化。欢迎试用并提出反馈！
