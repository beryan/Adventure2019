/*
* Reset.h
*
* Class Description: A class designed to represent reset information.
*
* Created on: January 21, 2019
* Author: Brittany Ryan, 301217765
*/

#ifndef RESET_H
#define RESET_H

#include <string>
#include "Types.h"
#include "json.hpp"

using json = nlohmann::json;

constexpr model::ID INVALID_ID = -1;

namespace model {

    class Reset {

    private:
        std::string action;
        model::ID id;
        int limit;
        model::ID room;
        int slot;

        friend std::ostream& operator<<(std::ostream& os, const Reset& r);

    public:
      	//constructors
      	Reset();
        Reset(std::string action, model::ID id, model::ID room);
        Reset(std::string action, model::ID id, int limit, model::ID room);
        Reset(std::string action, model::ID id, int limit, model::ID room, int slot);

        //getters and setters
        std::string getAction() const;
        model::ID getId() const;
        int getLimit() const;
        model::ID getRoom() const;
        int getSlot() const;
        void setAction(const std::string &action);
        void setId(const model::ID &id);
        void setLimit(int limit);
        void setRoom(model::ID room);
        void setSlot(int slot);
    };

    inline void from_json(const json &j, Reset &r) {
        r.setId(j.at("id").get<model::ID>());
        r.setAction(j.at("action").get<std::string>());
        if(j.find("limit") != j.end()) {
            r.setLimit(j.at("limit").get<int>());
        }
        if(j.find("room") != j.end()) {
            r.setRoom(j.at("room").get<model::ID>());
        }
        if(j.find("slot") != j.end()){
            r.setSlot(j.at("slot").get<int>());
        }

    }

    inline void to_json(json &j, const Reset &r) {

        j = {
                {"id", r.getId()},
                {"action", r.getAction()}
        };

        if(r.getLimit() != -1) {
            j.push_back({"limit", r.getLimit()});
        }

        if(r.getRoom() != -1) {
            j.push_back({"room", r.getRoom()});
        }

        if (r.getSlot() != -1){
            j.push_back({"slot", r.getSlot()});
        }
    }

}

#endif //RESET_H
