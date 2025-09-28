1. tasks.json是编译配置，在command一栏gcc用于编译C，g++用于编译C++.在args的{files}配置项里，应填入会用到的.cpp文件，包括引用的.h对应的.cpp和main.cpp

   ```json
   {
       "tasks": [
           {
               "type": "cppbuild",
               "label": "C/C++: g++.exe 生成活动文件",
               "command": "E:\\MinGW\\bin\\g++.exe",//C++的g++所在路径
               "args": [
                   "-fdiagnostics-color=always",
                   "-g",
                   "-I",
                   "${workspaceFolder}/include",  // 更新头文件路径,不能省.
                   "${workspaceFolder}/src/main.cpp",//参与编译的cpp
                   "${workspaceFolder}/src/tgaimage.cpp",
                   "${workspaceFolder}/src/model.cpp",
                   "-o",
                   "${fileDirname}\\${fileBasenameNoExtension}.exe"
                   //${workspaceFolder}是项目的绝对路径，即 盘符+项目名
               ],
               "options": {
                   "cwd": "${fileDirname}"
               },
               "problemMatcher": [
                   "$gcc"
               ],
               "group": {
                   "kind": "build",
                   "isDefault": true
               },
               "detail": "调试器生成的任务。"
           }
       ],
       "version": "2.0.0"
   }
   ```

   

2. 在c_cpp_properties里，includePath里，双星号表示该文件夹下所有文件及二级文件夹里的所有文件，单星号表示下面所有文件.

   ```json
   {
       "configurations": [
           {
               "name": "Win32",
               "includePath": [
                   "${workspaceFolder}/include",//可以省略.
                   "${workspaceFolder}/**"
               ],
               "defines": [
                   "_DEBUG",
                   "UNICODE",
                   "_UNICODE"
               ],
               "compilerPath": "E:\\MinGW\\bin\\g++.exe",
               "cStandard": "c17",
               "cppStandard": "gnu++14",
               "intelliSenseMode": "windows-gcc-x64"
           }
       ],
       "version": 4
   }
   ```

3. ./表示上级目录 ../表示上上级目录.