#! bash


mkdir -p bin/assets
mkdir bin/assets/shaders

echo "Compiling shaders"

echo "assets/shaders/Builtin.Objecthader.vert.glsl -> bin/assets/shaders/Builtin.Objecthader.vert.spv"
%VULKAN_SDK%/bin/glslc -fshader-stage=vert assets/shaders/Builtin.Objecthader.vert.gls -o bin/assets/shaders/Builtin.Objecthader.vert.spv
ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]; then
  echo "Shader compilation failed!"
  echo "Error:"$ERRORLEVEL &&exit
fi

echo "assets/shaders/Builtin.Objecthader.frag.glsl -> bin/assets/shaders/Builtin.Objecthader.frag.spv"
%VULKAN_SDK%\bin\glslc -fshader-stage=frag assets/shaders/Builtin.Objecthader.frag.glsl -o bin/assets/shaders/Builtin.Objecthader.frag.spv
ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]; then
  echo "Shader compilation failed!"
  echo "Error:"$ERRORLEVEL &&exit
fi


echo "Copying assets..."

echo copy -R "assets" "bin"
copy -R "assets" "bin"

echo "Done."


