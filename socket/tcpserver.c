#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
  // ソケット通信で使用するファイルディスクリプタを格納する変数
  int server_fd, new_socket;
  // IPアドレスを扱うための構造体、ソケットのエンドポイントアドレス情報を設定
  struct sockaddr_in address; // socketaddr_inだとエラー出るのなぜ
  // ソケットオプションを設定する際に使用
  int opt = 1;
  // accept()に渡すアドレス構造体のサイズを保持
  int addrlen = sizeof(address);
  char *message = "Hello, Client!\n";

  // TCP/IP通信をするソケットを作成
  // AF_INET:IPv4インターネットプロトコル、SOCK_STREAM:信頼性の高いバイトストリーム型のサービス（TCP）を指定するシグネチャ
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // setsocket():ソケットオプション設定をするシステムコール、
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY; // すべてのローカルインターフェースで待ち受け
  address.sin_port = htons(PORT);

  // ソケットにアドレスをバインド
  // bind()システムコール:ソケットにローカルアドレス(IP addressとポート番号)を割り当てるために使用される、成功すると0、エラーだと-1を返す
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // bind()のシステムコールとしての特徴：OSのネットワークスタックに直接指示を出す、この関数を通じてアプリケーションはOSに対し特定のポートに特定のIPアドレス

  // クライアントからの接続を待ち受ける、3は保留中の接続のキューの最大長
  if (listen(server_fd, 3) < 0)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  // クライアントからの接続を受け入れる
  // accept()システムコール：新しいソケットを生成
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
