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

    unsigned sub = 0;

    unsigned int cpuTotal = 0;

    unsigned int inputTotal = 0;

    unsigned int outputTotal = 0;

    unsigned int cpuCount = 0;

    unsigned int inputCount = 0;

    unsigned int outputCount = 0;

public:
    History history[ARRAY_SIZE];
    unsigned int cpuTimer = 0;

    ~Process();

    unsigned int ioTimer = 0;

    void printInfo();

    const string &getProcessName() const;

    unsigned int getPriority() const;

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

    unsigned int getSub() const;

    void setSub(unsigned int sub);

    void setProcessName(const string &processName);

    void setPriority(unsigned int priority);

};

#endif //ASSIGN4_PROCESS_H
