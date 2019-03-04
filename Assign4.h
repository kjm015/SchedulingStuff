//
// Created by Kevin Miyata on 2019-02-24.
//

#ifndef ASSIGN4_MAIN_H
#define ASSIGN4_MAIN_H

#include <queue>
#include <fstream>
#include <cstring>
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

void moveToQueue(Process *&, const string &, bool);

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


void completeBurst(Process *&moveProcess, History history, unsigned &proTimer, int &processesInUse, bool isIoBurst) {
    // TODO: Refactor this to use class variables rather than reference calls
    if (moveProcess != nullptr) {
        if (proTimer == history.burstValue) {
            proTimer = 0;
            moveProcess->setSub(moveProcess->getSub() + 1);
        }
        if (isIoBurst) {
            if (history.burstLetter == INPUT_BURST_LETTER) {
                moveProcess->setInputCount(moveProcess->getInputTotal() + 1);
            } else if (history.burstLetter == OUTPUT_BURST_LETTER) {
                moveProcess->setOutputCount(moveProcess->getOutputCount() + 1);
            }
            // TODO: refactor this so that we don't need the isBurst argument
            moveToQueue(moveProcess, history.burstLetter, isIoBurst);

            // TODO: Restructure this method so that we don't exit early
            return;
        }

        history = getProcessHistory(moveProcess);

        if (history.burstLetter == "N") {
            moveProcess->setCpuCount(moveProcess->getCpuCount() + 1);
            terminateProcess(moveProcess);
            processesInUse--;
        } else {
            moveProcess->setCpuCount(moveProcess->getCpuCount() + 1);
            moveToQueue(moveProcess, history.burstLetter, isIoBurst);
        }
    }
}

void setProcess(Process *process, const char *line) {
    char buffer[256];
    string processName;
    char *data;

    strcpy(buffer, line);

    processName = strtok(buffer, " ");
    process->setProcessName(processName);

    data = strtok(nullptr, " ");
    process->setPriority(static_cast<unsigned int>(atoi(data)));

    data = strtok(nullptr, " ");
    process->setArrivalTime(static_cast<unsigned int>(atoi(data)));
}

void setProcessHistory(Process *process, const char *line) {
    char buffer[256];
    int index = 0;

    strcpy(buffer, line);

    cerr << "\tSetting temporary process history structure!" << endl;
    History *temp = process->history;

    cerr << "\tTokenizing buffer!" << endl;
    char *burstInfo = strtok(buffer, " ");

    while (burstInfo != nullptr) {
        cerr << "\t\tReading burst info!" << endl;

        cerr << "\t\tCreating temporary History and assigning burst info letter" << endl;
        auto *temp2 = new History();
        temp2->burstLetter = burstInfo;

        cerr << "\t\tTokenizing burst info" << endl;
        burstInfo = strtok(nullptr, " ");

        cerr << "Assigning burst value to burst info" << endl;
        temp2->burstValue = static_cast<unsigned int>(atoi(burstInfo));

        cerr << "Tokenizing burst info..." << endl;
        burstInfo = strtok(nullptr, " ");

        cerr << "\t\tSetting element of temporary history element with the other temporary history!" << endl;
        temp[index] = *temp2;
        index++;
    }
}

History getProcessHistory(Process *history) {
    return history->history[history->getSub()];
}

void readFile() {
    string line;
    int startId = 100;

    ifstream inFile(IN_FILE_NAME);

    cerr << "\tI retrieved the infile name!" << endl;

    if (inFile.fail()) {
        cerr << "\tCould not open input text file!" << endl;
        return;
    }

    getline(inFile, line);
    while (!inFile.eof()) {
        cerr << "\t\tI'm reading the file!" << endl;

        auto *temp = new Process();
        temp->setProcessId(startId);

        cerr << "\t\tI'm about to set the current process!" << endl;
        setProcess(temp, line.c_str());

        if (temp->getProcessName() == END_OF_FILE_MARKER) {
            cerr << "\t\tReached end of file!" << endl;
            delete temp;
            break;
        }

        cerr << "\t\tPushing new process into the entry queue!" << endl;
        entryQueue.push(temp);

        cerr << "\t\tGetting the next line!" << endl;
        getline(inFile, line);

        // TODO: The program segmentation faults here. Fix this!
        cerr << "\t\tSetting the process history for this process!" << endl;
        setProcessHistory(temp, line.c_str());
    }
    cerr << "\tI'm closing the file!" << endl;
    inFile.close();
}

void terminateProcess(Process *&terminator) {
    cerr << endl << "Process \"" << terminator->getProcessName() << "\" terminated at " << timer;
    terminator->printInfo();
    terminatedProcessCount++;

    delete terminator;
    terminator = nullptr;
}

bool condition(int time) {
    return (time < MAX_TIME)
           and (!entryQueue.empty()
                or !readyQueue.empty()
                or !inputQueue.empty()
                or !outputQueue.empty()
                or activeProcess != nullptr
                or activeInputProcess != nullptr
                or activeOutputProcess != nullptr);
}

void activateProcess(Process *&process, priority_queue<Process *, vector<Process *>, Comparator> &activatorQueue) {
    if (process == nullptr and !activatorQueue.empty()) {
        process = activatorQueue.top();
        activatorQueue.pop();
    }
}

void moveToQueue(Process *&currentProcess, const string &queueName, bool readied) {
    if (readied) {
        readyQueue.push(currentProcess);
    } else if (queueName == OUTPUT_BURST_LETTER) {
        outputQueue.push(currentProcess);
    } else if (queueName == INPUT_BURST_LETTER) {
        inputQueue.push(currentProcess);
    }
    currentProcess = nullptr;
}

void printQueueContent(queue<Process *> q) {
    if (q.empty()) {
        cerr << "Empty queue!" << endl;
    } else {
        while (!q.empty()) {
            Process *temp = q.front();
            cerr << temp->getProcessName() << " (ID: " << temp->getProcessId() << ")" << endl;
            q.pop();
        }
    }
}

void printPriorityQueue(priority_queue<Process *, vector<Process *>, Comparator> pQueue) {
    if (pQueue.empty()) {
        cerr << "Empty priority queue!" << endl;
    } else {
        while (!pQueue.empty()) {
            Process *temp = pQueue.top();
            cerr << temp->getProcessName() << " (ID: " << temp->getProcessId() << ")" << endl;
            pQueue.pop();
        }
    }
}

void printProcessContent() {
    cerr << endl << "<----------------------------------------->" << endl;
    cerr << endl << "***** Status @ time: " << timer << " *****" << endl;

    cerr << endl << "Active process: ";
    if (activeProcess != nullptr) {
        cerr << activeProcess->getProcessName() << " (ID: " << activeProcess->getProcessId() << ")";
    }

    cerr << endl << "Active input process: ";
    if (activeInputProcess != nullptr) {
        cerr << activeInputProcess->getProcessName() << " (ID: " << activeInputProcess->getProcessId() << ")";
    }

    cerr << endl << "Active output process: ";
    if (activeOutputProcess != nullptr) {
        cerr << activeOutputProcess->getProcessName() << " (ID: " << activeOutputProcess->getProcessId() << ")";
    }
}

void printReport() {
    printProcessContent();

    cerr << endl << "Entry queue: ";
    printQueueContent(entryQueue);

    cerr << endl << "Ready queue: ";
    printPriorityQueue(readyQueue);

    cerr << endl << "Input queue: ";
    printPriorityQueue(inputQueue);

    cerr << endl << "Output queue: ";
    printPriorityQueue(outputQueue);

    cerr << endl << "<----------------------------------------->" << endl;
}

#endif //ASSIGN4_MAIN_H
