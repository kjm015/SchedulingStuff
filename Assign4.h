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

const unsigned BUFFER_SIZE = 256;

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

bool exitCondition(int);

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

/**
 * This function takes in a process, sets the process' history, increments a timer, increments a process counter, and
 * terminates the process if it is flagged with a certain burst letter.
 *
 * @param moveProcess - the process to be moved
 * @param history - the History structure to be injected into the moved process
 * @param proTimer - the process timer to be altered
 * @param processesInUse - the active process counter to be altered
 * @param isIoBurst - whether or not the current process is an input/output process
 */
void completeBurst(Process *&moveProcess, History history, unsigned &proTimer, int &processesInUse, bool isIoBurst) {
    // Check if the current process is active and if the burst timer is correct
    if (moveProcess != nullptr and proTimer == history.burstValue) {
        // Default process timer to zero
        proTimer = 0;

        // Increment the process' burst number
        moveProcess->setSub(moveProcess->getSub() + 1);

        // Check if the process is an i/o process
        if (isIoBurst) {
            if (history.burstLetter == INPUT_BURST_LETTER) {
                // Increment input counter if matched to burst letter
                moveProcess->setInputCount(moveProcess->getInputTotal() + 1);
            } else if (history.burstLetter == OUTPUT_BURST_LETTER) {
                // Increment output counter if matched to output letter
                moveProcess->setOutputCount(moveProcess->getOutputCount() + 1);
            }

            // Move the process into the active queue
            moveToQueue(moveProcess, history.burstLetter, true);
        } else {
            // Otherwise, get the burst info for the process
            history = getProcessHistory(moveProcess);

            // Check for end of process marker
            if (history.burstLetter == "N") {
                // Terminate process and increase CPU count
                moveProcess->setCpuCount(moveProcess->getCpuCount() + 1);
                terminateProcess(moveProcess);
                processesInUse--;
            } else {
                // Increment CPU count and move process to the corresponding queue
                moveProcess->setCpuCount(moveProcess->getCpuCount() + 1);
                moveToQueue(moveProcess, history.burstLetter, false);
            }
        }
    }
}

/**
 * This function handles the logic relating to initializing a process. It will take the process info,
 * process name, priority, and arrival time, and set them to their respective values.
 *
 * @param line - line of input text to be processed at once
 * @param process - the process to retrieve information from
 */
void setProcess(Process *process, const char *line) {
    char buffer[BUFFER_SIZE];
    string processName;
    char *data;

    strcpy(buffer, line);

    processName = strtok(buffer, " ");
    process->setProcessName(processName);

    data = strtok(nullptr, " ");
    process->setPriority(atoi(data));

    data = strtok(nullptr, " ");
    process->setArrivalTime(atoi(data));
}


/**
 * This function sets the process history for a given process. It will take in a line of data and populate the
 * process' history structure with the data that it pulls from the line.
 *
 * @param process - the process that will have its history set
 * @param line - the line of information that will be used to set the process history.
 */
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

        cerr << "\t\tTokenizing burst info: " << temp2->burstLetter << endl;
        burstInfo = strtok(nullptr, " ");

        cerr << "\t\tAssigning burst value to burst info: " << burstInfo << endl;
        temp2->burstValue = atoi(burstInfo);

        cerr << "\t\tTokenizing burst info..." << endl;
        burstInfo = strtok(nullptr, " ");

        cerr << "\t\tSetting element of temporary history element with the other temporary history!" << endl;
        temp[index] = *temp2;
        index++;
    }
}

/**
 * This function simply will return the history of a given History structure pointer.
 *
 * @param history - a pointer to a history structure
 * @return a history stucture
 */
History getProcessHistory(Process *history) {
    return history->history[history->getSub()];
}

/**
 * This function will read the file given the file name specified in the IN_FILE_NAME constant above.
 * It parses through the entirety of the file, scanning for keywords and end-of-file markers. In doing so,
 * it initializes new processes to handle the data that gets read in. The function closes the file once
 * it has finished.
 */
void readFile() {
    string line;
    int startId = 100;

    ifstream inFile(IN_FILE_NAME);

    // Check if the file is corrupt or inaccessible
    if (inFile.fail()) {
        cerr << "\tCould not open input text file!" << endl;
        return;
    }

    getline(inFile, line);
    while (inFile) {

        auto *temp = new Process();
        temp->setProcessId(startId);

        // Set the current process
        setProcess(temp, line.c_str());

        if (temp->getProcessName() == END_OF_FILE_MARKER) {
            cerr << "\t\tReached end of file!" << endl;
            break;
        }

        // Push new process to the entry queue
        entryQueue.push(temp);

        // Retrieve the next line
        getline(inFile, line);

        // Set the process history
        setProcessHistory(temp, line.c_str());

        if (!inFile.eof()) {
            // Retrieve the next line if not the end of the file
            getline(inFile, line);
        }
    }
    // Close the file when finished
    inFile.close();
}

/**
 * This function will terminate a process.
 * It will print out the process' info once it does so.
 *
 * @param terminator - the process to be terminated.
 */
void terminateProcess(Process *&terminator) {
    cerr << endl << "Process \"" << terminator->getProcessName() << "\" terminated at " << timer;
    terminator->printInfo();

    terminatedProcessCount++;

    delete terminator;
    terminator = nullptr;
}

/**
 * This function will return the conditional as to whether or not
 * the current program should terminate.  In order for this condition to
 * be met, the maximum timer must not be exceeded, and none of the queues can be empty.
 *
 * @param time - the current time
 *
 * @return whether or not the program should end
 */
bool exitCondition(int time) {
    return (time < MAX_TIME)
           and (!entryQueue.empty()
                or !readyQueue.empty()
                or !inputQueue.empty()
                or !outputQueue.empty()
                or activeProcess != nullptr
                or activeInputProcess != nullptr
                or activeOutputProcess != nullptr);
}

/**
 * This function moves a given process to a given active priority queue.
 *
 * @param process - the process to be moved to active queue
 * @param activatorQueue - the queue to be moved to
 */
void activateProcess(Process *&process, priority_queue<Process *, vector<Process *>, Comparator> &activatorQueue) {
    if (process == nullptr and !activatorQueue.empty()) {
        process = activatorQueue.top();
        activatorQueue.pop();
    }
}

/**
 * This function moves a given process to a non-prioritized queue. Based on the queue's name and readiness,
 * the process may be moved to a variety of different queues.
 *
 * @param currentProcess - the process to move into a queue
 * @param isReady - whether or not the process should be moved to the ready queue
 * @param queueName - the name of the queue to be moved to
 */
void moveToQueue(Process *&currentProcess, const string &queueName, bool isReady) {
    if (isReady) {
        readyQueue.push(currentProcess);
    } else if (queueName == OUTPUT_BURST_LETTER) {
        outputQueue.push(currentProcess);
    } else if (queueName == INPUT_BURST_LETTER) {
        inputQueue.push(currentProcess);
    }
    currentProcess = nullptr;
}

/**
 * This function prints the contents of a given queue, including the processes that
 * are contained within it. It will also print if the queue is empty.
 */
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

/**
 * This function prints the contents of a given priority queue, including the processes that
 * are contained within it. It will also print if the priority queue is empty.
 */
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

/**
 * This function prints out the contents of each active process.
 * This occurs every given number of clock cycles, and displays
 * relevant information regarding what each process is doing at
 * any given time.
 */
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

/**
 * This method prints the report of the processes and queues.
 * That's pretty much it.
 */
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
