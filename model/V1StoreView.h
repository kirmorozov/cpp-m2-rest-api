//
// Created by Kirill Morozov on 22.01.2020.
//

#ifndef MG_M2_API_V1STOREVIEW_H
#define MG_M2_API_V1STOREVIEW_H

#include "ModelBase.h"
//#include "ModelBase.cpp"

#include <string>

namespace io {
namespace swagger {
namespace server {
namespace model {

/// <summary>
///
/// </summary>
    class  V1StoreView
            : public ModelBase
    {
    public:
        V1StoreView();
        virtual ~V1StoreView();

        /////////////////////////////////////////////
        /// ModelBase overrides

        void validate() override;

        nlohmann::json toJson() const override;
        void fromJson(nlohmann::json& json) override;

        /////////////////////////////////////////////
        /// V1StoreView members

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
        int getStoreGroupId() const;
        void setStoreGroupId(int value);
        /// <summary>
        ///
        /// </summary>
        int getIsActive() const;
        void setIsActive(int value);

    protected:
        int _id;
        std::string _code;
        std::string _name;
        int _website_id;
        int _store_group_id;
        int _is_active;
    };

}
}
}
}

#endif //MG_M2_API_V1STOREVIEW_H
