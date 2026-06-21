#include "state_machine.hpp"

StateMachine::StateMachine() : currentState(0), newState(0), eventGenerated(false), pEventData(nullptr) {}

StateMachine::~StateMachine() {}

void StateMachine::externalEvent(short newState, EventData* pData) {
    if (newState != Event_Ignore) {
        internalEvent(newState, pData);
        handleEvent();
    }
}

void StateMachine::internalEvent(short newState, EventData* pData) {
    if (pData == nullptr) {
        pData = new NoEventData();
    }
    this->eventGenerated = true;
    this->newState = newState;
    this->pEventData = pData;
}

void StateMachine::handleEvent() {
    EventData* pDataTemp = nullptr;

    while (eventGenerated) {
        State* newStateObj = getState(newState);
        State* currentStateObj = getState(currentState);

        pDataTemp = pEventData;

        pEventData = nullptr;

        eventGenerated = false;

        bool guardResult = true;
        if (newStateObj->guard != nullptr) {
            guardResult = newStateObj->guard(pDataTemp);
        }

        if (guardResult == true) {
            if (newState != currentState) {
                if (currentStateObj->exit != nullptr) {
                    currentStateObj->exit(pDataTemp);
                }
                if (newStateObj->entry != nullptr) {
                    newStateObj->entry(pDataTemp);
                }
            }
            setCurrentState(newState);
            newStateObj->action(pDataTemp);
        }

        if (pDataTemp) {
            delete pDataTemp;
            pDataTemp = NULL;
        }
    }
}

void StateMachine::initStateMap() {}

std::unique_ptr<State> StateMachine::createState(ActionFunc action = nullptr, GuardFunc guard = nullptr,
                                                 EntryFunc entry = nullptr, ExitFunc exit = nullptr) {
    auto state = std::make_unique<State>();
    state->action = action;
    state->guard = guard;
    state->entry = entry;
    state->exit = exit;
    return state;
}

State* StateMachine::getState(short stateId) {
    auto it = stateMap.find(stateId);
    if (it != stateMap.end()) {
        return it->second.get();
    }
    return nullptr;
}
