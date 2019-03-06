//
// Created by Kevin Miyata on 2019-03-02.
//
#include "Process.h"

using namespace std;

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

Process::~Process() {
    delete this;
}