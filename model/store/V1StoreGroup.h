//
// Created by Kirill Morozov on 22.01.2020.
//

#ifndef MG_M2_API_V1StoreGroup_H
#define MG_M2_API_V1StoreGroup_H

#include "ModelBase.h"
//#include "ModelBase.cpp"

#include <string>

namespace io::swagger::server::model {

/// <summary>
///
/// </summary>
    class V1StoreGroup
            : public ModelBase {
    public:
        V1StoreGroup();

        virtual ~V1StoreGroup();

        /////////////////////////////////////////////
        /// ModelBase overrides

        void validate() override;

        nlohmann::json toJson() const override;

        void fromJson(nlohmann::json &json) override;

        /////////////////////////////////////////////
        /// V1StoreGroup members

        /// <summary>
        ///
        /// </summary>
        int getId() const;

        void setId(int value);

        /// <summary>
        ///
        /// </summary>
        std::string getCode() const;

        void setCode(std::string value);

        /// <summary>
        ///
        /// </summary>
        std::string getName() const;

        void setName(std::string value);

        /// <summary>
        ///
        /// </summary>
        int getWebsiteId() const;

        void setWebsiteId(int value);

        /// <summary>
        ///
        /// </summary>
        int getRootCategoryId() const;

        void setRootCategoryId(int value);

        /// <summary>
        ///
        /// </summary>
        int getDefaultStoreId() const;

        void setDefaultStoreId(int value);

    protected:
        int _id;
        std::string _code;
        std::string _name;
        int _website_id;
        int _root_category_id;
        int _default_store_id;
    };

}

#endif //MG_M2_API_V1StoreGroup_H
