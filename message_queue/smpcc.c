#include <stdio.h>
#include <string.h>
#include <mqueue.h>

#define TODO() printf("reached TODO at line %d", __LINE__);
#define VERSION_NUMBER 000
#define MQUEUE_MAX_MESSAGE_SIZE 200
#define MQUEUE_NAME "/SMPCD_MQ"

int
main(int argc, char* argv[]) {

  /* basic arg parsing, daemon doesn't do a whole lot except yell at mpd so don't need much arg parsing */
  if (argc >= 2) {
    if (strcmp(argv[1], "-v") || strcmp(argv[1], "--version")) { printf("smpcd: simple music player client daemon version %d\n", VERSION_NUMBER); }
    if (!strcmp(argv[1], "--help")) { printf("usage: smpcd start"); return 0; }
  }

  mqd_t message_queue = mq_open(MQUEUE_NAME, O_CREAT | O_EXCL | O_WRONLY, 0600, NULL);

  if (!message_queue) {
    perror("error: couldn't open message queue");
    return -1;
  }

  mq_send(message_queue, "Hi there", 9, 10);
  mq_close(message_queue);
}
