#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
  int server_fd, new_socket;
  struct sockaddr_in address; // socketaddr_inだとエラー出るのなぜ
  int opt = 1;
  int addrlen = sizeof(address);
  char *message = "Hello, Client!\n";

  // ソケット作成
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // ソケットオプション設定
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY; // すべてのローカルインターフェースで待ち受け
  address.sin_port = htons(PORT);

  // ソケットにアドレスをバインド
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // クライアントからの接続を待ち受ける
  if (listen(server_fd, 3) < 0)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  // クライアントからの接続を受け入れる
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
  {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  // メッセージを送信
  send(new_socket, message, strlen(message), 0);
  printf("Hello message sent\n");

  // 接続を終了
  close(new_socket);
  close(server_fd);

  return 0;
}
