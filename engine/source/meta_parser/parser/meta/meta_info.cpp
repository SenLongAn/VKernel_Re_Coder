#include "common/precompiled.h"

#include "parser/parser.h"

#include "meta_info.h"

MetaInfo::MetaInfo(const Cursor& cursor)
{
    for (auto& child : cursor.getChildren()) ///< Traverse all direct nodes
    {

        if (child.getKind() != CXCursor_AnnotateAttr) ///< If there is no annotation __attribute__
            continue;

        for (auto& prop : extractProperties(child)) ///< Otherwise, traverse all attributes of this node
            m_properties[prop.first] = prop.second; ///< Add to m_properties
    }
}

std::string MetaInfo::getProperty(const std::string& key) const
{
    auto search = m_properties.find(key);

    // use an empty string by default
    return search == m_properties.end() ? "" : search->second;
}

bool MetaInfo::getFlag(const std::string& key) const { return m_properties.find(key) != m_properties.end(); }

std::vector<MetaInfo::Property> MetaInfo::extractProperties(const Cursor& cursor) const
{
    std::vector<Property> ret_list; ///< std::vector<std::pair<std::string, std::string>>

    auto propertyList = cursor.getDisplayName();

    auto&& properties = Utils::split(propertyList, ","); ///< split

    static const std::string white_space_string = " \t\r\n";

    for (auto& property_item : properties) ///< Traverse all properties
    {
        auto&& item_details = Utils::split(property_item, ":");                 ///< Get pair: key, value
        auto&& temp_string  = Utils::trim(item_details[0], white_space_string); ///< Remove spaces
        if (temp_string.empty())
        {
            continue;
        }
        ret_list.emplace_back(temp_string,
                              item_details.size() > 1 ? Utils::trim(item_details[1], white_space_string) :
                                                        ""); ///< propertie: key, value
    }
    return ret_list; ///< vector<propertie: key, value>
}
