#ifndef BINARY_SEMS_H
#define BINARY_SEMS_H

int bsUseSemUndo;
int bsRetryOnEintr;
int bsNoWait;

int initSemAvailable(int semId,int semNum);
int initSemInUse(int semId,int semNum);

int reserveSem(int semId,int semNum);
int releaseSem(int semId,int semNum);

#endif
