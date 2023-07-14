#ifndef EPIDEMASETTINGS_H
#define EPIDEMASETTINGS_H

class EpidemiaSettings
{
public:
    EpidemiaSettings(int peopleCount,int chanseInfected,int chanseDead,int chanseAsymptotic,bool quarNeeds,int timeRecovered, int timeQuarCheck);
    EpidemiaSettings();

    int getPeopleCount();
    int getChanseInfected();
    int getChanseeDead();
    int getChanseAsymptotic();
    bool getQuarNeeds();
    int getTimeRecovered();
    int getTimeQuarCheck();

private:
   int peopleCount;
   int chanseInfected;
   int chanseDead;
   int chanseAsymptotic;
   bool quarNeeds;

   int timeRecovered;
   int timeQuarCheck;

};

#endif // EPIDEMASETTINGS_H
