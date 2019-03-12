#include "Assign4.h"

using namespace std;

int main() {
    cerr << "Hello, world!" << endl;

    readFile();

    cerr << "I read the file!" << endl;

    int activeProcessCount = 0;
    int idleTimer = 0;

    while (exitCondition(timer)) {

        cerr << "Starting the process loop!" << endl;

        // Pull processes from entry queue to ready queue and print result
        while (activeProcessCount < AT_ONCE and !entryQueue.empty()) {

            Process *movedProcess = entryQueue.front();

            if (movedProcess->getArrivalTime() > timer) {
                break;
            }

            entryQueue.pop();

            cerr << "Process " << movedProcess->getProcessName() << " with priority " << movedProcess->getPriority()
                 << " moved to the ready queue @ time " << timer
                 << endl;

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

            if (activeProcess != nullptr) {
                activeProcess->setCpuTotal(activeProcess->getCpuTotal() + 1);
                activeProcess->cpuTimer++;
            }
        }

        activateProcess(activeInputProcess, inputQueue);

        if (activeInputProcess != nullptr) {
            History currentHistory = getProcessHistory(activeInputProcess);

            completeBurst(activeInputProcess, currentHistory, activeInputProcess->ioTimer, activeProcessCount, true);

            if (activeInputProcess != nullptr) {
                activeInputProcess->setInputTotal(activeInputProcess->getInputTotal() + 1);
                activeInputProcess->ioTimer++;
            }
        }

        activateProcess(activeOutputProcess, outputQueue);

        if (activeOutputProcess != nullptr) {
            History currentHistory = getProcessHistory(activeOutputProcess);

            completeBurst(activeOutputProcess, currentHistory, activeOutputProcess->ioTimer, activeProcessCount, true);

            if (activeOutputProcess != nullptr) {
                activeOutputProcess->setOutputCount(activeOutputProcess->getOutputCount() + 1);
                activeOutputProcess->ioTimer++;
            }
        }

        timer++;
    }

    cerr << endl << endl << "<-------------------------------------------------------->" << endl;
    cerr << "Program ended at time: " << timer << endl;
    cerr << "Total amount of CPU idle time: " << idleTimer << endl;
    cerr << "Number of processes terminated: " << terminatedProcessCount << endl;

    printReport();

    return 0;
}
