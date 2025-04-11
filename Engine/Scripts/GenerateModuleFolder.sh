#!/bin/bash

# 获取脚本所在目录的绝对路径
script_path="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# 检查参数
if [ $# -eq 0 ]; then
    echo "用法: $0 <目标路径>"
    echo "例如: $0 Runtime/Platform/Input"
    exit 1
fi

# 获取目标路径参数
target_path="$1"

# 创建模块目录结构
echo "正在创建模块目录: $target_path"
mkdir -p "${script_path}/../Source/${target_path}/"{Include,Src}

# 创建 .gitkeep 文件
touch "${script_path}/../Source/${target_path}/Include/.gitkeep"
touch "${script_path}/../Source/${target_path}/Src/.gitkeep"

echo "模块目录创建完成！"
echo "位置: ${script_path}/../Source/${target_path}"
