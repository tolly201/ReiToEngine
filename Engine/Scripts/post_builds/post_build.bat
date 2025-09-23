@echo off

if not exit "%cd\bin\assets\shaders" mkdir "%cd%\bin\assets\shaders"

echo "Compiling shaders..."

echo "assets/shaders/Builtin.Objecthader.vert.glsl -> bin/assets/shaders/Builtin.Objecthader.vert.spv"
%VULKAN_SDK%\bin\glslc -fshader-stage=vert assets/shaders/Builtin.Objecthader.vert.gls -o bin/assets/shaders/Builtin.Objecthader.vert.spv

IF %ERRORLEVEL% NEQ 0 (echo "Shader compilation failed!" && exit)

echo "assets/shaders/Builtin.Objecthader.frag.glsl -> bin/assets/shaders/Builtin.Objecthader.frag.spv"
%VULKAN_SDK%\bin\glslc -fshader-stage=frag assets/shaders/Builtin.Objecthader.frag.glsl -o bin/assets/shaders/Builtin.Objecthader.frag.spv

IF %ERRORLEVEL% NEQ 0 (echo "Shader compilation failed!" && exit)

echo "Copying assets..."

echo xcopy "assets" "bin\assets" /h /i /c /k /e /r /y
xcopy "assets" "bin\assets" /h /i /c /k /e /r /y

echo "Done."
