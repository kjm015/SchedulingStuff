#include "Assign4.h"

using namespace std;

/**
 * This is the main function that drives the logic of the whole program.
 * It will read in an incoming file, prioritize the jobs listed in the file based on
 * the values provided, and will put each task in a respective queue to be processed.
 * Information gets printed as it is processed in the queues, and for each given number
 * of clock cycles.
 *
 * @author Kevin Miyata
 */
int main() {
    // Create runtime variables to track active processes and idle time
    int activeProcessCount = 0;
    int idleTimer = 0;

    cerr << "Hello, world!" << endl;

    // Read the incoming file
    readFile();

    while (exitCondition(timer)) {

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

        // Activate the first process
        activateProcess(activeProcess, readyQueue);

        // Check if the process is available
        if (activeProcess == nullptr) {
            idleTimer++;
        } else {
            // Retrieve the active process' burst info
            History current = getProcessHistory(activeProcess);

            // Move the active process into the priority queue
            moveToQueue(activeProcess, current.burstLetter, false);

            // Complete the queue
            completeBurst(activeProcess, current, activeProcess->cpuTimer, activeProcessCount, false);

            if (activeProcess != nullptr) {
                activeProcess->setCpuTotal(activeProcess->getCpuTotal() + 1);
                activeProcess->cpuTimer++;
            }
        }

        // Activate the input process
        activateProcess(activeInputProcess, inputQueue);

        if (activeInputProcess != nullptr) {
            History currentHistory = getProcessHistory(activeInputProcess);

            completeBurst(activeInputProcess, currentHistory, activeInputProcess->ioTimer, activeProcessCount, true);

            if (activeInputProcess != nullptr) {
                activeInputProcess->setInputTotal(activeInputProcess->getInputTotal() + 1);
                activeInputProcess->ioTimer++;
            }
        }

        // Activate the output process
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

    // Give job termination information
    cerr << endl << endl << "<-------------------------------------------------------->" << endl;
    cerr << "Program ended at time: " << timer << endl;
    cerr << "Total amount of CPU idle time: " << idleTimer << endl;
    cerr << "Number of processes terminated: " << terminatedProcessCount << endl;

    // Print final report
    printReport();

    return 0;
}
