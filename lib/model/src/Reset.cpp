/*
* Reset.cpp
*
* Class Description: A class designed to represent reset information.
*
* Created on: January 21, 2019
* Author: Brittany Ryan, 301217765
*/

#include "Reset.h"

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

    Reset::Reset(std::string action, model::ID id, int limit, int room, int slot)
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

    int Reset::getRoom() const {
      return room;
    }

    int Reset::getSlot() const {
      return slot;
    }

    void Reset::setAction(std::string action) {
      this->action = std::move(action);
    }

    void Reset::setId(model::ID id) {
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

}
