//
// Created by Kirill Morozov on 22.01.2020.
//

#include "V1StoreGroup.h"

#include "ModelBase.h"

namespace io::swagger::server::model {

    V1StoreGroup::V1StoreGroup() {
        _id = -1;
        _code = "";
        _name = "";
        _website_id = -1;
        _root_category_id = -1;
        _default_store_id = -1;
    }

    V1StoreGroup::~V1StoreGroup() {
    }

    void V1StoreGroup::validate() {
        // TODO: implement validation
    }

    nlohmann::json V1StoreGroup::toJson() const {
        nlohmann::json val = nlohmann::json::object();

        val["id"] = ModelBase::toJson(_id);
        val["code"] = ModelBase::toJson(_code);
        val["name"] = ModelBase::toJson(_name);
        val["website_id"] = ModelBase::toJson(_website_id);
        val["root_category_id"] = ModelBase::toJson(_root_category_id);
        val["default_store_id"] = ModelBase::toJson(_default_store_id);

        return val;
    }

    void V1StoreGroup::fromJson(nlohmann::json &val) {
        setId(val.at("id"));
        setCode(val.at("code"));
        setName(val.at("name"));
        setWebsiteId(val.at("website_id"));
        setRootCategoryId(val.at("root_category_id"));
        setDefaultStoreId(val.at("default_store_id"));
    }


    int V1StoreGroup::getId() const {
        return _id;
    }

    void V1StoreGroup::setId(int value) {
        _id = value;
    }

    std::string V1StoreGroup::getCode() const {
        return _code;
    }

    void V1StoreGroup::setCode(std::string value) {
        _code = value;
    }

    std::string V1StoreGroup::getName() const {
        return _name;
    }

    void V1StoreGroup::setName(std::string value) {
        _name = value;

    }

    int V1StoreGroup::getWebsiteId() const {
        return _website_id;
    }

    void V1StoreGroup::setWebsiteId(int value) {
        _website_id = value;
    }

    int V1StoreGroup::getRootCategoryId() const {
        return _root_category_id;
    }

    void V1StoreGroup::setRootCategoryId(int value) {
        _root_category_id = value;
    }

    int V1StoreGroup::getDefaultStoreId() const {
        return _default_store_id;
    }

    void V1StoreGroup::setDefaultStoreId(int value) {
        _default_store_id = value;
    }

}