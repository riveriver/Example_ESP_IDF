# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

# # 递归地在src目录下匹配所有文件，并将它们的路径存储在变量app_sources中
# FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*)
# # 注册组件，app_sources是上一步获取的源文件路径列表
# idf_component_register(SRCS ${app_sources})

# Make sure this new file main.c is registered as source file using idf_component_register function in src/CMakeLists.txt file
# 将指定的源文件添加到组件的构建系统中，以便在构建时进行编译和链接
# SRCS指定了组件的源文件列表，可以包含多个源文件
idf_component_register(SRCS "main.c")
