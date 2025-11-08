#ifndef DISPATCHER_H
#define DISPATCHER_H

typedef enum {
    EVENT_LED_CMD
} event_type_t;

typedef struct {
    void *fifo_reserved;  // tarvitaan Zephyr FIFOon
    event_type_t type;
    char msg[20];         // tämä kenttä tarvitaan dispatcherissa
} dispatcher_event_t;

void dispatcher_post(dispatcher_event_t *evt);
void dispatcher_init(void);
void on_task_done(char which);

#endif
