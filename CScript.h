#pragma once
#include "Event.h"

class CScriptFunc {
protected:
    CScriptFunc *parentG;
    CScriptFunc *parentT;
    CScriptFunc *gethR;
    CScriptFunc *thenR;
    Event *dropR;
    bool done;
    bool doneG;

    void doneAll() {
        this->done = true;
        if (this->parentG) {
            this->parentG->doneAll();
        }
    }

    void resetAll() {
        this->reset();
        if (this->thenR) {
            this->thenR->resetAll();
        }
        if (this->gethR) {
            this->gethR->resetAll();
        }
    }

    virtual bool action(){
        return true;
    };

    virtual void reset(){

    };

public:
    explicit CScriptFunc() {
        this->gethR = nullptr;
        this->thenR = nullptr;
        this->parentT = nullptr;
        this->parentG = nullptr;
        this->dropR = nullptr;
        this->done = false;
        this->doneG = true;
    }

    CScriptFunc *geth(CScriptFunc *next) {
        this->gethR = next;
        this->doneG = false;
        next->parentG = this;
        return next;
    }

    CScriptFunc *then(CScriptFunc *next) {
        this->thenR = next;
        next->parentT = this;
        return next;
    }

    CScriptFunc *drop(Event *drop) {
        this->dropR = drop;
        return this;
    }

    bool exec() {
        if (this->dropR && this->dropR->listen()){
            this->doneAll();
        }
        if (!this->done) {
            this->done = this->action();
        }
        if (!this->doneG) {
            this->doneG = this->gethR->exec();
        } else {
            if (this->done) {
                if (this->thenR) {
                    if (this->thenR->exec()) {
                        if (!this->parentT && this->doneG && this->done && !this->parentG) {
                            this->resetAll();
                        }
                        return true;
                    }
                } else {
                    return true;
                }
            }
        }
        return false;
    }
};
