###
 # @Author: jielong.wang jielong.wang@akuvox.com
 # @Date: 2024-10-25 15:31:14
 # @LastEditors: jielong.wang jielong.wang@akuvox.com
 # @LastEditTime: 2024-10-28 18:13:56
 # @FilePath: /record_voice/cpp/build/push_to_device.sh
 # @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
### 


adb connect 192.168.13.109:5654
adb -s 192.168.13.109:5654 root
adb connect 192.168.13.109:5654
adb -s 192.168.13.109:5654 remount

adb -s 192.168.13.109:5654 push record_demo /data/record_voice
adb -s 192.168.13.109:5654 push ../thirdlib/lame/Android/armeabi-v7a/liblame.so /data/record_voice/lib