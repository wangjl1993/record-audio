/*
 * @Author: jielong.wang jielong.wang@akuvox.com
 * @Date: 2024-10-25 14:47:11
 * @LastEditors: jielong.wang jielong.wang@akuvox.com
 * @LastEditTime: 2024-10-31 15:43:29
 * @FilePath: /record_voice/demo.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "record.h"



int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];
    const int port = 7412;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 本地监听
    server_addr.sin_port = htons(port);

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    while (true) {
        int n = recv(sockfd, buffer, sizeof(buffer), 0);
        buffer[n] = '\0';

        if (strcmp(buffer, "START") == 0) {
            startRecording();
        } else if (strncmp(buffer, "STOP", 4) == 0) {
            std::string filename = buffer + 5;
            std::cout << "Stop recording and save to " << filename << std::endl;
            stopRecording(filename);
        }
    }

    close(sockfd);
    return 0;
}
