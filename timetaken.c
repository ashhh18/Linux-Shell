#include "timetaken.h"

struct commandTime *initTime()
{
    struct commandTime* cT=(struct commandTime *)malloc(sizeof(struct commandTime));
    cT->timeTaken=0;
    cT->timeToShow=0;
    strcpy(cT->commandName,"\0");
    return cT;
}