//
// Created by Kevin Miyata on 2019-02-24.
//

#ifndef ASSIGN4_PROCESS_H
#define ASSIGN4_PROCESS_H

#include <iostream>
#include <cstring>
#include <utility>

using namespace std;

const unsigned ARRAY_SIZE = 10;
extern unsigned timer;

struct History {
    unsigned burstValue;
    string burstLetter;
};

class Process {
private:
    string processName;

    unsigned priority, arrivalTime = 0;

    int processId;

    History history[ARRAY_SIZE];

    unsigned sub = 0;

    unsigned int cpuTimer = 0;

    unsigned int ioTimer = 0;

    unsigned int cpuTotal = 0;

    unsigned int inputTotal = 0;

    unsigned int outputTotal = 0;

    unsigned int cpuCount = 0;

    unsigned int inputCount = 0;

    unsigned int outputCount = 0;

public:
    void printInfo();

    const string &getProcessName() const;

    unsigned int getPriority() const;

    unsigned int getCpuTimer() const;

    void setCpuTimer(unsigned int cpuTimer);

    unsigned int getIoTimer() const;

    void setIoTimer(unsigned int ioTimer);

    unsigned int getCpuTotal() const;

    void setCpuTotal(unsigned int cpuTotal);

    unsigned int getInputTotal() const;

    void setInputTotal(unsigned int inputTotal);

    unsigned int getOutputTotal() const;

    void setOutputTotal(unsigned int outputTotal);

    unsigned int getCpuCount() const;

    void setCpuCount(unsigned int cpuCount);

    unsigned int getInputCount() const;

    void setInputCount(unsigned int inputCount);

    unsigned int getOutputCount() const;

    void setOutputCount(unsigned int outputCount);

    unsigned int getArrivalTime() const;

    void setArrivalTime(unsigned int arrivalTime);

    int getProcessId() const;

    void setProcessId(int processId);

    const History *getHistory() const;

    unsigned int getSub() const;

    void setSub(unsigned int sub);

    void setProcessName(const string &processName);

    void setPriority(unsigned int priority);

};

const string &Process::getProcessName() const {
    return processName;
}

unsigned int Process::getPriority() const {
    return priority;
}

unsigned int Process::getArrivalTime() const {
    return arrivalTime;
}

void Process::setArrivalTime(unsigned int arrivalTime) {
    Process::arrivalTime = arrivalTime;
}

int Process::getProcessId() const {
    return processId;
}

void Process::setProcessId(int processId) {
    Process::processId = processId;
}

const History *Process::getHistory() const {
    return history;
}

unsigned int Process::getSub() const {
    return sub;
}

void Process::setSub(unsigned int sub) {
    Process::sub = sub;
}

void Process::setProcessName(const string &processName) {
    Process::processName = processName;
}

void Process::setPriority(unsigned int priority) {
    Process::priority = priority;
}

unsigned int Process::getCpuTimer() const {
    return cpuTimer;
}

void Process::setCpuTimer(unsigned int cpuTimer) {
    Process::cpuTimer = cpuTimer;
}

unsigned int Process::getIoTimer() const {
    return ioTimer;
}

void Process::setIoTimer(unsigned int ioTimer) {
    Process::ioTimer = ioTimer;
}

unsigned int Process::getCpuTotal() const {
    return cpuTotal;
}

void Process::setCpuTotal(unsigned int cpuTotal) {
    Process::cpuTotal = cpuTotal;
}

unsigned int Process::getInputTotal() const {
    return inputTotal;
}

void Process::setInputTotal(unsigned int inputTotal) {
    Process::inputTotal = inputTotal;
}

unsigned int Process::getOutputTotal() const {
    return outputTotal;
}

void Process::setOutputTotal(unsigned int outputTotal) {
    Process::outputTotal = outputTotal;
}

unsigned int Process::getCpuCount() const {
    return cpuCount;
}

void Process::setCpuCount(unsigned int cpuCount) {
    Process::cpuCount = cpuCount;
}

unsigned int Process::getInputCount() const {
    return inputCount;
}

void Process::setInputCount(unsigned int inputCount) {
    Process::inputCount = inputCount;
}

unsigned int Process::getOutputCount() const {
    return outputCount;
}

void Process::setOutputCount(unsigned int outputCount) {
    Process::outputCount = outputCount;
}

void Process::printInfo() {
    cerr << endl << "Name: " << this->getProcessName() << endl;
    cerr << "Process ID: " << this->getProcessId() << endl;
    cerr << "Priority: " << this->getPriority() << endl;
    cerr << "Start time: " << this->getArrivalTime() << endl;
    cerr << "End time: " << timer << endl;
    cerr << "Time spent active: " << this->getCpuTotal() << " in " << this->getCpuCount() << " CPU bursts" << endl;
    cerr << "Input time spent active: " << this->getInputTotal() << " in " << this->getInputCount() << " input bursts"
         << endl;
    cerr << "Output time spent active: " << this->getOutputTotal() << " in " << this->getOutputCount()
         << " output bursts" << endl;
    cerr << "Time spent waiting: "
         << (timer - this->getArrivalTime() - this->getCpuTotal() - this->getOutputTotal() - this->getInputTotal())
         << endl;
}

#endif //ASSIGN4_PROCESS_H
