// Das Internet ist für uns alle Neuland
#ifndef BAKEENGINE_ABSTRACTAPPLICATION_H
#define BAKEENGINE_ABSTRACTAPPLICATION_H


class AbstractApplication {
public:
    virtual void run() = 0;
    virtual void terminate() = 0;
};


#endif
