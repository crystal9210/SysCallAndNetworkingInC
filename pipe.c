#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
  // パイプのためのファイルディスクリプタ配列を用意、[0]:読み込み側、[1]:書き込み側のディスクリプタ
  int parent_to_child_fd[2];
  int child_to_parent_fd[2];
  pid_t pid;                               // プロセスIDを格納する変う宣言
  char message_parent[] = "Hello, child!"; // 親プロセスから子プロセスへのメッセージ
  char message_child[] = "Hello, parent!"; // 子から親へのメッセージ

  char buffer[50]; // なんのため？

  // パイプを作成、pipe()システムコール：パイプが正常に作成された場合は0を返し、失敗したときは-1を返す
  // 親から子への通信用パイプ
  if (pipe(parent_to_child_fd) == -1)
  {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  // 子から親への通信用パイプ
  if (pipe(child_to_parent_fd) == -1)
  {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  // フォークして子プロセスを生成
  // fork()：フォークシステムコールを使用、現在のプロセスを複製し、新しい子プロセスを生成する、生成された子プロセスは生成時点からのコードパスを走るため、無限回プロセスが複製されないようになっている
  // 子プロセスが再帰的にfork()を呼び出すことなく、そのまま0を返すように設計されている
  pid = fork();

  if (pid < 0)
  {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  // fork() の返り値が0かどうかで、そのプロセスが子プロセスか親プロセスか判断；親プロセスの場合、fork() 関数は新しいプロセスのプロセスIDを返すため、親プロセスではpid > 0の条件が成立。
  // 一方、子プロセスの場合、fork() 関数は新しくプロセスを生成する処理を走らせることなく0を返すため、子プロセスではpid == 0の条件が成立。
  if (pid == 0)
  {                               // 子プロセス
    close(parent_to_child_fd[1]); // 親から子への書き込みエンドを閉じる
    close(child_to_parent_fd[0]); // 子から親への読み込みエンドを閉じる

    // 親からのメッセージを受信
    read(parent_to_child_fd[0], buffer, sizeof(buffer));
    printf("Child (PID %d) received: %s\n", getpid(), buffer);

    // 親へメッセージを送信
    write(child_to_parent_fd[1], message_child, sizeof(message_child));

    close(parent_to_child_fd[0]); // 読み終わったら閉じる
    close(child_to_parent_fd[1]); // 書き終わったら閉じる
    exit(EXIT_SUCCESS);
  }
  else
  {                               // 親プロセス
    close(parent_to_child_fd[0]); // 親から子への読み込みエンドを閉じる
    close(child_to_parent_fd[1]); // 子から親への書き込みエンドを閉じる

    // 子プロセスへメッセージを送信
    write(parent_to_child_fd[1], message_parent, sizeof(message_parent));

    // 子からのメッセージを受信
    read(child_to_parent_fd[0], buffer, sizeof(buffer));
    printf("Parent (PID %d) received: %s\n", getpid(), buffer);

    close(parent_to_child_fd[1]); // 書き終わったら閉じる
    close(child_to_parent_fd[0]); // 読み終わったら閉じる

    wait(NULL); // 子プロセスの終了を待つ
  }

  return 0;
}
