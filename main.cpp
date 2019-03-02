#include "main.h"

using namespace std;

int main() {
    readFile();

    int activeProcessCount = 0;
    int idleTimer = 0;

    while (condition(timer)) {

        // Pull processes from entry queue to ready queue and print result
        while (activeProcessCount < AT_ONCE and !entryQueue.empty()) {
            Process *movedProcess = entryQueue.front();
            if (movedProcess->getArrivalTime() > timer) {
                continue;
            }
            entryQueue.pop();
            cerr << "Process " << movedProcess->getProcessName() << " moved to the ready queue @ time" << timer << endl;
            movedProcess->setArrivalTime(timer);
            readyQueue.push(movedProcess);

            activeProcessCount++;
        }

        // Print process report once every fixed number of ticks
        if (timer % HOW_OFTEN == 0) {
            printReport();
        }

        activateProcess(activeProcess, readyQueue);

        if (activeProcess == nullptr) {
            idleTimer++;
        } else {
            History current = getProcessHistory(activeProcess);
            moveToQueue(activeProcess, current.burstLetter, false);
            completeBurst(activeProcess, current, activeProcess->cpuTimer, activeProcessCount, false);
        }

    }
    return 0;
}

void completeBurst(Process *&moveProcess, History moveProcessHistory, unsigned &proTimer, int &processesInUse,
                   bool isIoBurst) {
    // TODO: Refactor this to use class variables rather than reference calls
    if (moveProcess != nullptr) {
        if (proTimer == moveProcessHistory.burstValue) {
            proTimer = 0;
            moveProcess->setSub(moveProcess->getSub() + 1);
        }
        if (isIoBurst) {
            if (moveProcessHistory.burstLetter == INPUT_BURST_LETTER) {
                moveProcess->setInputCount(moveProcess->getInputTotal() + 1);
            } else if (moveProcessHistory.burstLetter == OUTPUT_BURST_LETTER) {
                moveProcess->setOutputCount(moveProcess->getOutputCount() + 1);
            }
            // TODO: refactor this so that we don't need the isBurst argument
            moveToQueue(moveProcess, moveProcessHistory.burstLetter, isIoBurst);

            // TODO: Restructure this method so that we don't exit early
            return;
        }

        moveProcessHistory = getProcessHistory(moveProcess);

        if (moveProcessHistory.burstLetter == "N") {
            moveProcess->setCpuCount(moveProcess->getCpuCount() + 1);
            terminateProcess(moveProcess);
            processesInUse--;
        } else {
            moveProcess->setCpuCount(moveProcess->getCpuCount() + 1);
            moveToQueue(moveProcess, moveProcessHistory.burstLetter, isIoBurst);
        }
    }
}

void setProcessHistory(Process *process, const char *line) {
    char buffer[256];
    int index = 0;

    strcpy(buffer, line);

    History *temp = process->history;

    char *burstInfo = strtok(buffer, " ");

    while (burstInfo != nullptr) {
        auto *temp2 = new History();
        temp2->burstLetter = burstInfo;

        burstInfo = strtok(nullptr, " ");
        temp2->burstValue = static_cast<unsigned int>(atoi(burstInfo));

        burstInfo = strtok(nullptr, " ");

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

    // TODO: replace with global constant
    ifstream inFile(IN_FILE_NAME);

    if (inFile.fail()) {
        cerr << "Could not open input text file!" << endl;
        return;
    }

    getline(inFile, line);
    while (inFile) {
        auto *temp = new Process();
        temp->setProcessId(startId);

        setProcess(temp, line.c_str());

        // TODO: replace with strcmp and string constant
        if (temp->getProcessName() == END_OF_FILE_MARKER) {
            // TODO: add destructor to Process and call it here
            delete (temp);
            temp = nullptr;
            break;
        }

        entryQueue.push(temp);

        getline(inFile, line);
        setProcessHistory(temp, line.c_str());
    }
    inFile.close();
}

void terminateProcess(Process *&terminator) {
    cerr << endl << "Process \"" << terminator->getProcessName() << "\" terminated at " << timer;
    terminator->printInfo();
    terminatedProcessCount++;

    // TODO: use process destructor here
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

void moveToQueue(Process *&currentProcess, string queueName, bool readied) {
    if (readied) {
        readyQueue.push(currentProcess);
    } else if (queueName == OUTPUT_BURST_LETTER) {
        // TODO: replace queue name comparator with constant string and strcmp()
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