//
// Created by Kirill Morozov on 22.01.2020.
//

#include "V1StoreView.h"

#include "ModelBase.h"

namespace io {
    namespace swagger {
        namespace server {
            namespace model {

                V1StoreView::V1StoreView() {
                    _id = -1;
                    _code = "";
                    _name = "";
                    _website_id = -1;
                    _store_group_id = -1;
                    _is_active = 0;
                }

                V1StoreView::~V1StoreView() {
                }

                void V1StoreView::validate() {
                    // TODO: implement validation
                }

                nlohmann::json V1StoreView::toJson() const {
                    nlohmann::json val = nlohmann::json::object();

                    val["id"] = ModelBase::toJson(_id);
                    val["code"] = ModelBase::toJson(_code);
                    val["name"] = ModelBase::toJson(_name);
                    val["website_id"] = ModelBase::toJson(_website_id);
                    val["store_group_id"] = ModelBase::toJson(_store_group_id);
                    val["is_active"] = ModelBase::toJson(_is_active);

                    return val;
                }

                void V1StoreView::fromJson(nlohmann::json &val) {
                    setId(val.at("id"));
                    setCode(val.at("code"));
                    setName(val.at("name"));
                    setWebsiteId(val.at("website_id"));
                    setStoreGroupId(val.at("store_group_id"));
                    setIsActive(val.at("is_active"));
                }


                int V1StoreView::getId() const {
                    return _id;
                }

                void V1StoreView::setId(int value) {
                    _id = value;
                }

                std::string V1StoreView::getCode() const {
                    return _code;
                }

                void V1StoreView::setCode(std::string value) {
                    _code = value;
                }

                std::string V1StoreView::getName() const {
                    return _name;
                }

                void V1StoreView::setName(std::string value) {
                    _name = value;

                }

                int V1StoreView::getWebsiteId() const {
                    return _website_id;
                }

                void V1StoreView::setWebsiteId(int value) {
                    _website_id = value;
                }

                int V1StoreView::getStoreGroupId() const {
                    return _store_group_id;
                }

                void V1StoreView::setStoreGroupId(int value) {
                    _store_group_id = value;
                }

                int V1StoreView::getIsActive() const {
                    return _is_active;
                }

                void V1StoreView::setIsActive(int value) {
                    _is_active = value;
                }

            }
        }
    }
}