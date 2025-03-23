## Producer Consumer OR Bounded Buffer Problem

### THE PROBLEM STATEMENT:

---

1. Producer will produce and consumer will consume with synchronization of a common buffer.
2. Until producer thread produces any data consumer thread can't consume.
3. Threads will use condition_variable to notify each other.
4. We need mutex if we use condition_variable because CV waits on mutex.
5. This is one of the example of producer consumer there are many.

### PRODUCER thread steps:

6. lock mutex, if success then go ahead otherwise wait for mutex to get free.
7. check if buffer is full and if it is full then unlock mutex and sleep, if not then go ahead and produce.
8. insert item in buffer.
9. unlock mutex.
10. notify consumer.

### CONSUMER thread steps:

11. lock mutex, if success then go ahead otherwise wait for mutex to get free.
12. check if buffer is empty and if it is, then unlock the mutex and sleep, if not thean go ahead and consume.
13. consume item from buffer.
14. unlock mutex.
15. notify producer.

### IMP:

    Producer and Consumer have to notify each other upon comletion of their job.
