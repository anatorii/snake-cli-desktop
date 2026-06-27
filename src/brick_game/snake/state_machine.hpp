#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <functional>
#include <map>
#include <memory>

namespace s21 {

class EventData;

enum { Event_Ignore = 0xFE, Cannot_Happen = 0xFF };

using ActionFunc = std::function<void(EventData*)>;
using GuardFunc = std::function<bool(EventData*)>;
using EntryFunc = std::function<void(EventData*)>;
using ExitFunc = std::function<void(EventData*)>;

struct State {
    ActionFunc action;
    GuardFunc guard;
    EntryFunc entry;
    ExitFunc exit;
};

class EventData {
   public:
    virtual ~EventData() {};
};

using NoEventData = EventData;

class StateMachine {
   public:
    StateMachine();
    virtual ~StateMachine();
    short getCurrentState() { return currentState; };

   protected:
    short currentState;
    short newState;
    bool eventGenerated;
    EventData* pEventData;
    std::map<short, std::unique_ptr<State>> stateMap;
    void externalEvent(short newState, EventData* pData = nullptr);
    void internalEvent(short newState, EventData* pData = nullptr);
    void initStateMap();
    State* getState(short stateId);
    std::unique_ptr<State> createState(ActionFunc action, GuardFunc guard, EntryFunc entry, ExitFunc exit);

   private:
    void setCurrentState(short newState) { currentState = newState; };
    void handleEvent();
};

}  // namespace s21

#endif
