//
// Created by Brittany Ryan on 2019-01-20.
//

#ifndef AREA_H
#define AREA_H

#include <string>
#include <vector>
#include "Room.h"
#include "Reset.h"

using model::Room;
using model::Reset;

namespace model {
    /**
    *  @class Area
    *
    *  @brief A class designed to represent an area.
    *
    *  The Area class contains all area information needed for the game
    *  including name, rooms, objects, npcs, and resets
    */
    class Area {

    private:
        std::string name;
        std::vector<Room> rooms;
        std::vector<NPC> npcs;
        std::vector<Object> objects;
        std::vector<Reset> resets;
        std::vector<Reset> saveResets;

        friend std::ostream& operator<<(std::ostream& os, const Area& rhs);

    public:
        //constructors
        Area();
        Area(std::string name);
        Area(std::string name, std::vector<Room> rooms);

        //getters and setters
        std::string getName() const;
        std::vector<Room>& getRooms();
        std::vector<NPC> getNpcs() const;
        std::vector<Object> getObjects() const;
        std::vector<Reset> getResets() const;
        std::vector<Reset> getSaveResets() const;
        void setName(const std::string &name);
        void setRooms(const std::vector<Room> &rooms);
        void setNpcs(const std::vector<NPC> &npcs);
        void setObjects(const std::vector<Object> &objects);
        void setResets(const std::vector<Reset> &resets);
        void setSaveResets(const std::vector<Reset> &saveResets);

        void addRoom(const Room &room);
        void addNpc(const NPC &npc);
        void addObject(const Object &object);
        void addReset(const Reset &reset);

        std::vector<Room>::iterator findRoomById(model::ID roomID);
        std::vector<NPC>::iterator findNpcById(model::ID npcID);
        std::vector<Object>::iterator findObjectById(model::ID objectID);

        bool npcExists(const model::ID &npcId);
        bool objectExists(const model::ID &objectId);
        void removeResets(const model::ID &roomId);

        bool operator==(const Area& area) const;
    };

    inline void from_json(const Json &json, Area &area) {
        area.setName(json.at("name").get<std::string>());
    }

    inline void to_json(Json &json, const Area &area) {
        json = {{"name", area.getName()}};
    }
}

#endif //AREA_H
