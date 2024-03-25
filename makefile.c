#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // open()関数のためのヘッダ
#include <unistd.h> // close()関数のためのへッダ
#include <string.h> // strlen()関数のためのヘッダ

int main()
{
  // ファイルを作成し、ファイルディスクリプタ(プログラムがファイルやデバイスとの間でデータの読み書きを行うための識別子)を作成;open()はシステムコール
  int fd = open("from_makefile.txt", O_CREAT | O_WRONLY, 0644); // ファイルのパーミッション指定について、所有者には読み書き権限、他ユーザーには読み取り権限のみがある
  if (fd == -1)
  {
    perror("open");
    exit(1);
  }

  // ファイルに書き込むテキストを定義；ポインタを使用することで別領域にコピーすることなく扱うことができる
  // 追記：文字列リテラルはプログラうの静的データ領域に格納される
  char *text = "Hello, World!\n";

  // ファイルにテキストを書き込む
  // 第三引数に書き込む長さを指定する理由：write() 関数が書き込むデータの長さを知る必要があるためです。write() 関数は、第二引数に渡されたバッファのデータを、第三引数で指定された長さだけファイルに書き込みます。したがって、正確なデータの長さを指定することで、不必要なデータの書き込みやメモリのオーバーランなどを防ぎます。
  if (write(fd, text, strlen(text)) == -1)
  {
    perror("write");
    exit(EXIT_FAILURE);
  }

  // ファイルディスクリプタをクローズ
  if (close(fd) == -1)
  {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return 0;
}
