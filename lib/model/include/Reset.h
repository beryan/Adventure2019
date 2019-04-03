//
// Created by Brittany Ryan on 2019-01-21.
//

#ifndef RESET_H
#define RESET_H

#include <string>
#include "Types.h"
#include "json.hpp"

using Json = nlohmann::json;

constexpr model::ID INVALID_ID = -1;

namespace model {
    /**
    *  @class Reset
    *
    *  @brief A class designed to represent reset information.
    *
    *  The Reset class contains all information needed to reset
    *  objects and npcs into rooms
    */
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

    inline void from_json(const Json &json, Reset &reset) {
        reset.setId(json.at("id").get<model::ID>());
        reset.setAction(json.at("action").get<std::string>());
        if(json.find("limit") != json.end()) {
            reset.setLimit(json.at("limit").get<int>());
        }
        if(json.find("room") != json.end()) {
            reset.setRoom(json.at("room").get<model::ID>());
        }
        if(json.find("slot") != json.end()){
            reset.setSlot(json.at("slot").get<int>());
        }

    }

    inline void to_json(Json &json, const Reset &reset) {

        json = {
                {"id", reset.getId()},
                {"action", reset.getAction()}
        };

        if(reset.getLimit() != -1) {
            json.push_back({"limit", reset.getLimit()});
        }

        if(reset.getRoom() != -1) {
            json.push_back({"room", reset.getRoom()});
        }

        if (reset.getSlot() != -1){
            json.push_back({"slot", reset.getSlot()});
        }
    }

}

#endif //RESET_H
