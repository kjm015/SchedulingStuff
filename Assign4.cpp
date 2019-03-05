#include "Assign4.h"

using namespace std;

int main() {
    cerr << "Hello, world!" << endl;

    readFile();

    cerr << "I read the file!" << endl;

    int activeProcessCount = 0;
    int idleTimer = 0;

    while (condition(timer)) {

        cerr << "Starting the process loop!" << endl;

        // Pull processes from entry queue to ready queue and print result
        while (activeProcessCount < AT_ONCE and !entryQueue.empty()) {
            Process *movedProcess = entryQueue.front();
            if (movedProcess->getArrivalTime() > timer) {
                break;
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

        timer++;
    }
    return 0;
}
