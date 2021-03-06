//
// Created by Brittany Ryan on 2019-01-21.
//

#include "Reset.h"
#include <iostream>

using model::Reset;

namespace model {

    //constructors
    Reset::Reset()
        : action(""),
        id(-1),
        limit(-1),
        room(-1),
        slot(-1)
        { }

    Reset::Reset(std::string action, model::ID id, model::ID room)
            : action(std::move(action)),
              id(id),
              limit(-1),
              room(room),
              slot(-1)
    { }

    Reset::Reset(std::string action, model::ID id, int limit, model::ID room)
            : action(std::move(action)),
              id(id),
              limit(limit),
              room(room),
              slot(-1)
    { }

    Reset::Reset(std::string action, model::ID id, int limit, model::ID room, int slot)
        : action(std::move(action)),
        id(id),
        limit(limit),
        room(room),
        slot(slot)
        { }

    //getters and setters
    std::string Reset::getAction() const {
        return action;
    }

    model::ID Reset::getId() const {
        return id;
    }

    int Reset::getLimit() const {
        return limit;
    }

    model::ID Reset::getRoom() const {
        return room;
    }

    int Reset::getSlot() const {
        return slot;
    }

    void Reset::setAction(const std::string &action) {
        this->action = action;
    }

    void Reset::setId(const model::ID &id) {
        this->id = id;
    }

    void Reset::setLimit(int limit) {
        this->limit = limit;
    }

    void Reset::setRoom(int room) {
        this->room = room;
    }

    void Reset::setSlot(int slot) {
        this->slot = slot;
    }

    //print reset
    std::ostream& operator<<(std::ostream& os, const Reset& r) {
        if (r.action == "object" || r.action == "npc") {
            os << "- " << r.action << " [" << r.id << "] in room [" << r.room << "]";
            if (r.action == "npc") {
                os << " x[" << r.limit << "]";
            }
            os << std::endl;
        }
        return os;
    }
}
