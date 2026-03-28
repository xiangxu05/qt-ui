cmake_minimum_required(VERSION 3.16)

project(qt-ui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(UIKIT_SOURCES
    src/uikit/core/theme_tokens.cpp
    src/uikit/core/theme_manager.cpp
    src/uikit/controls/ui_button.cpp
    src/uikit/controls/ui_label.cpp
    src/uikit/form/ui_line_edit.cpp
    src/uikit/form/ui_combo_box.cpp
    src/uikit/form/ui_switch.cpp
    src/uikit/feedback/ui_message_bar.cpp
    src/uikit/feedback/ui_dialog.cpp
    src/uikit/feedback/ui_toast.cpp
    src/uikit/shell/title_bar.cpp
    src/uikit/shell/navigation_panel.cpp
    src/uikit/shell/app_shell_window.cpp
    src/uikit/resources/uikit.qrc
)

set(UIKIT_HEADERS
    src/uikit/core/theme_tokens.h
    src/uikit/core/theme_manager.h
    src/uikit/controls/ui_button.h
    src/uikit/controls/ui_label.h
    src/uikit/form/ui_line_edit.h
    src/uikit/form/ui_combo_box.h
    src/uikit/form/ui_switch.h
    src/uikit/feedback/ui_message_bar.h
    src/uikit/feedback/ui_dialog.h
    src/uikit/feedback/ui_toast.h
    src/uikit/shell/title_bar.h
    src/uikit/shell/navigation_panel.h
    src/uikit/shell/app_shell_window.h
    src/uikit/utils/icon_defs.h
    src/uikit/types/common_types.h
)

add_library(uikit STATIC
    ${UIKIT_SOURCES}
    ${UIKIT_HEADERS}
)

target_link_libraries(uikit PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
if(WIN32)
    target_link_libraries(uikit PRIVATE dwmapi)
endif()
target_include_directories(uikit
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(GALLERY_SOURCES
    src/gallery/main.cpp
    src/gallery/demo_gallery_widget.cpp
    src/gallery/demo_gallery_widget.h
    src/gallery/state_demo_widget.cpp
    src/gallery/state_demo_widget.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qt-ui
        MANUAL_FINALIZATION
        ${GALLERY_SOURCES}
    )
else()
    add_executable(qt-ui
        ${GALLERY_SOURCES}
    )
endif()

target_link_libraries(qt-ui PRIVATE uikit Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(qt-ui PROPERTIES
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS qt-ui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qt-ui)
endif()
