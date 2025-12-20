#pragma once
#include "common/precompiled.h"

/**
 * Template file: used to generate new files
 */

class TemplateManager
{
public:
    static TemplateManager* getInstance() ///< Singleton Pattern: Get a globally unique instance
    {
        static TemplateManager* m_pInstance;
        if (nullptr == m_pInstance)
            m_pInstance = new TemplateManager();
        return m_pInstance;
    }

    void loadTemplates(std::string path, std::string template_name); ///< Load template into template pool

    std::string renderByTemplate(std::string     template_name,
                                 Mustache::data& template_data); ///< Return the result after replacing the data,
                                                                 ///< template_data is the data to be replaced

private:
    // Private constructor
    TemplateManager() {}
    TemplateManager(const TemplateManager&);

    // Copy assignment function
    TemplateManager& operator=(const TemplateManager&);

    std::unordered_map<std::string, std::string>
        m_template_pool; ///< Template Pool: Template Name, Template File Content
};
