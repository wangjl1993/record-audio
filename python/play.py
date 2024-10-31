'''
Author: jielong.wang jielong.wang@akuvox.com
Date: 2024-10-25 14:49:28
LastEditors: jielong.wang jielong.wang@akuvox.com
LastEditTime: 2024-10-31 15:55:06
FilePath: /python/play.py
Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
'''
import socket
import time
from pathlib import Path
from playsound import playsound

IP = "192.168.13.109"
PORT = 7412

def notify_android(message):
    server_address = (IP, PORT)
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        sock.sendto(message.encode(), server_address)
    finally:
        sock.close()


if __name__ == "__main__":

    audio_root = Path(rf"W:\rk2.1\asr_test_dataset\wav")

    for wav_file in audio_root.glob('*.wav'):
        notify_android("START")
        time.sleep(0.01)
        wav_file = str(wav_file).replace("\\", "/")
        playsound(wav_file)
        notify_android(f"STOP {Path(wav_file).stem}")
        time.sleep(1)
        
