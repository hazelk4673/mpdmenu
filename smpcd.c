#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <stdlib.h>

#define TODO() printf("reached TODO at line %d", __LINE__);
#define VERSION_NUMBER 000
#define MQUEUE_MAX_MESSAGE_SIZE 200
#define MQUEUE_NAME "/SMPCD_MQ"

int
main(int argc, char* argv[]) {

  /* basic arg parsing, daemon doesn't do a whole lot except yell at mpd so don't need much arg parsing */
  if (argc >= 2) {
    if (strcmp(argv[1], "-v") || strcmp(argv[1], "--version")) { printf("smpcd: simple music player client daemon version %d\n", VERSION_NUMBER); }
    if (!strcmp(argv[1], "--help")) { printf("usage: smpcd start\n"); return 0; }
  }

  mqd_t message_queue = mq_open(MQUEUE_NAME, O_RDONLY);
  if (!message_queue) {
    perror("error: couldn't open message queue\n");
    return -1;
  }

  /* grab message queue attributes */
  struct mq_attr message_queue_attr;
  if (!mq_getattr(message_queue, &message_queue_attr)) { printf("couldn't get message queue attributes\n"); return -1; }

  /* init buffer for storing messages */
  char* buf = calloc (message_queue_attr.mq_msgsize, 1);
  if (!buf) { printf("failed to initialize message buffer\n"); return -1; }

  /* grab message from queue and its priority */
  unsigned int priority = 0;
  if (!mq_receive(message_queue, buf, message_queue_attr.mq_msgsize, &priority)) { printf("failed to receive message\n"); }
  else { printf("received message: %s priority: %u\n", buf, priority); }


  free(buf);
  buf = NULL;
  mq_close(message_queue);
}
