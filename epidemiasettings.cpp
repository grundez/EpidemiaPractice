#include "epidemiasettings.h"

EpidemiaSettings::EpidemiaSettings(int peopleCount, int chanseInfected, int chanseDead, int chanseAsymptotic, bool quarNeeds, int timeRecovered, int timeQuarCheck)
{

}

EpidemiaSettings::EpidemiaSettings(){

}

int EpidemiaSettings::getPeopleCount()
{
    return peopleCount;
}

int EpidemiaSettings::getChanseAsymptotic()
{
    return chanseAsymptotic;
}

int EpidemiaSettings::getChanseInfected()
{
    return chanseInfected;
}

int EpidemiaSettings::getChanseeDead()
{
    return chanseDead;
}

bool EpidemiaSettings::getQuarNeeds(){
    return quarNeeds;
}

int EpidemiaSettings::getTimeRecovered()
{
    return timeRecovered;
}

int EpidemiaSettings::getTimeQuarCheck()
{
    return timeQuarCheck;
}
