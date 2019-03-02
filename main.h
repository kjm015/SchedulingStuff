//
// Created by Kevin Miyata on 2019-02-24.
//

#ifndef ASSIGN4_MAIN_H
#define ASSIGN4_MAIN_H

#include <queue>
#include <fstream>
#include "Process.h"

using namespace std;

const unsigned MAX_TIME = 500;
const unsigned QUEUE_SIZE = 20;
const unsigned AT_ONCE = 5;
const unsigned HOW_OFTEN = 25;

const char *IN_FILE_NAME = "data4.txt";
const char *INPUT_BURST_LETTER = "I";
const char *OUTPUT_BURST_LETTER = "O";
const char *END_OF_FILE_MARKER = "STOPHERE";

class Comparator {
public:
    bool operator()(Process *left, Process *right) {
        return (left->getPriority() < right->getPriority());
    }
};

bool condition(int);

void printProcessContent();

void printQueueContent(queue<Process *>);

void printPriorityQueue(priority_queue<Process *, vector<Process *>, Comparator>);

void printReport();

void moveToQueue(Process *&, string, bool);

void completeBurst(Process *&, History, unsigned &, int &, bool);

void activateProcess(Process *&, priority_queue<Process *, vector<Process *>, Comparator> &);

void terminateProcess(Process *&);

void readFile();

void setProcess(Process *, const char *);

void setProcessHistory(Process *, const char *);

History getProcessHistory(Process *);

Process *activeProcess = nullptr;
Process *activeInputProcess = nullptr;
Process *activeOutputProcess = nullptr;

queue<Process *> entryQueue;
priority_queue<Process *, vector<Process *>, Comparator> readyQueue;
priority_queue<Process *, vector<Process *>, Comparator> inputQueue;
priority_queue<Process *, vector<Process *>, Comparator> outputQueue;

int terminatedProcessCount = 0;
unsigned timer = 0;

#endif //ASSIGN4_MAIN_H
