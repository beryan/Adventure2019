//
// Created by Waswa Olunga on 2019-01-18.
//

#ifndef OBJECT_H
#define OBJECT_H

#include "Types.h"
#include "ExtraInfo.h"
#include "Slot.h"
#include "json.hpp"

using model::Slot;
using Json = nlohmann::json;

namespace model {
    /**
     *  @class Object
     *
     *  @brief A class describing an object.
     *
     *  The Object class contains all the properties of
     *  an object that is part of the inventory of a player
     *  or an object that is part of a room.
     */

    class Object {
    public:
        Object();

        Object(
                model::ID id,
                std::string shortDescription
        );

        Object(
                model::ID id,
                std::string shortDescription,
                std::vector<std::string> longDescription,
                std::vector<std::string> keywords,
                Slot slot
        );

        Object(
                model::ID id,
                std::string shortDescription,
                std::vector<std::string> longDescription,
                std::vector<std::string> keywords,
                Slot slot,
                std::vector<ExtraInfo> extraObjectInfo
        );

        model::ID getId() const;

        void setId(model::ID id);

        std::string getShortDescription() const;

        void setShortDescription(std::string shortDescription);

        std::vector<std::string> getLongDescription() const;

        void setLongDescription(std::vector<std::string> longDescription);

        std::vector<std::string> getKeywords() const;

        void setKeywords(std::vector<std::string> keywords);

        Slot getSlot() const;

        void setSlot(Slot slot);

        std::vector<ExtraInfo> getExtraObjectInfo() const;

        void setExtraObjectInfo(std::vector<ExtraInfo> extraObjectInfo);

        bool canBeEquipped() const;

        bool containsKeyword(const std::string &keyword) const;

        bool operator==(const Object &object) const;

        bool operator<(const Object &object) const;

        Object& operator=(const Object &object);

        static constexpr model::ID DEFAULT_ID = -1;

    private:
        model::ID id;

        std::string name;

        std::string shortDescription;

        std::vector<std::string> longDescription;

        std::vector<std::string> keywords;

        Slot slot;

        std::vector<ExtraInfo> extraObjectInfo;

        friend std::ostream&operator<<(std::ostream& os, const Object& obj);
    };

    inline void from_json(const Json &json, Object &object) {
        object.setId(json.at("id").get<model::ID>());
        object.setShortDescription(json.at("shortdesc").get<std::string>());
        object.setLongDescription(json.at("longdesc").get<std::vector<std::string>>());
        object.setKeywords(json.at("keywords").get<std::vector<std::string>>());

        if (json.count("slot")) {
            Slot slot;
            auto slotString = json.at("slot").get<std::string>();
            slot = model::getSlotFromString(slotString);
            object.setSlot(slot);
        }
    }

    inline void to_json(Json &json, const Object &object) {
        json = {
                {"id",        object.getId()},
                {"shortdesc", object.getShortDescription()},
                {"keywords",  object.getKeywords()},
                {"longdesc",  object.getLongDescription()},
                {"slot",  model::getStringFromSlot(object.getSlot())}
        };
    }

}

#endif //OBJECT_H