#pragma once

class Event {
private:
    bool state;
public:
    explicit Event(){
        this->state = false;
    }
    virtual bool listen() {
        this->state = true;
        return this->state;
    }
    virtual void emit(bool value) {
        this->state = value;
    }
};