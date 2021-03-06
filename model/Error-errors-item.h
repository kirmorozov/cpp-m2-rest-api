/**
* MG M2 Lightning API
* Magento 2.3 API as standalone server
*
* OpenAPI spec version: 0.1.0
* Contact: apiteam@swagger.io
*
* NOTE: This class is auto generated by the swagger code generator program.
* https://github.com/swagger-api/swagger-codegen.git
* Do not edit the class manually.
*/
/*
 * Error_errors_item.h
 *
 * Error details
 */

#ifndef Error_errors_item_H_
#define Error_errors_item_H_


#include "ModelBase.h"

#include <string>

namespace io {
namespace swagger {
namespace server {
namespace model {

/// <summary>
/// Error details
/// </summary>
class  Error_errors_item
    : public ModelBase
{
public:
    Error_errors_item();
    virtual ~Error_errors_item();

    /////////////////////////////////////////////
    /// ModelBase overrides

    void validate() override;

    nlohmann::json toJson() const override;
    void fromJson(nlohmann::json& json) override;

    /////////////////////////////////////////////
    /// Error_errors_item members

    /// <summary>
    /// Error message
    /// </summary>
    std::string getMessage() const;
    void setMessage(std::string value);
    bool messageIsSet() const;
    void unsetMessage();
    /// <summary>
    /// 
    /// </summary>
    nlohmann::json getParameters() const;
    void setParameters(nlohmann::json value);
    bool parametersIsSet() const;
    void unsetParameters();

protected:
    std::string m_Message;
    bool m_MessageIsSet;
    nlohmann::json m_Parameters;
    bool m_ParametersIsSet;
};

}
}
}
}

#endif /* Error_errors_item_H_ */
