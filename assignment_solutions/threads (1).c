#include <iostream>
#include <pthread.h>
#include <string>
#include <vector>

using namespace std;

const int NUM_THREADS = 4; // we want to create 4 threads and not want to change in future so const them

struct ThreadData {
  int threadId; // Thread IDs
  string message;  // message want to print is declaring
};

vector<string> messages(NUM_THREADS);

void *printMessage(void *threadArg) {
  ThreadData *data = (ThreadData *)threadArg;
  int threadId = data->threadId;
  string message = data->message;

  cout << "Thread " << threadId << " says: " << message << endl;

  messages[threadId] = message;

  pthread_exit(NULL);
}

int main() {
  pthread_t threads[NUM_THREADS];
  ThreadData threadData[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++) {
    cout << "Creating thread " << i << endl;
    threadData[i].threadId = i;
    threadData[i].message = "Hello from thread " + to_string(i);

    int rc = pthread_create(&threads[i], NULL, printMessage, (void *)&threadData[i]);
    if (rc) {
      cout << "Error: unable to create thread, " << rc << endl;
      exit(-1);
    }
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    int rc = pthread_join(threads[i], NULL);
    if (rc) {
      cout << "Error: unable to join thread, " << rc << endl;
      exit(-1);
    }
  }

  cout << "All threads completed, messages exchanged:" << endl;
  for (int i = 0; i < NUM_THREADS; i++) {
    cout << messages[i] << endl;
  }

  pthread_exit(NULL);
}
