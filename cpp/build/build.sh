###
 # @Author: jielong.wang jielong.wang@akuvox.com
 # @Date: 2024-10-25 15:04:22
 # @LastEditors: jielong.wang jielong.wang@akuvox.com
 # @LastEditTime: 2024-10-28 13:58:03
 # @FilePath: /record_voice/build/build.sh
 # @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
### 
TARGET_ARCH=armeabi-v7a
ANDROID_NDK_PATH=/home/ai_team/jielong/test_S539/android-ndk-r19c
cmake ../ \
        -DCMAKE_SYSTEM_NAME=Android \
        -DCMAKE_SYSTEM_VERSION=23 \
        -DCMAKE_ANDROID_ARCH_ABI=${TARGET_ARCH} \
        -DCMAKE_ANDROID_STL_TYPE=c++_static \
        -DCMAKE_ANDROID_NDK=${ANDROID_NDK_PATH} \
        -DCMAKE_BUILD_TYPE=debug 

make -j4

rm -rf CMakeFiles
rm -rf CMakeCache.txt
rm -rf Makefile
rm -rf cmake_install.cmake