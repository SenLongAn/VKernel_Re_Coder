#pragma once

#include <unordered_map>

/**
 * GUID allocator
 */
namespace VKernel
{
    static const size_t s_invalid_guid = 0; ///< invaild guid

    template<typename T>
    class GuidAllocator
    {
    public:
        static bool isValidGuid(size_t guid) { return guid != s_invalid_guid; } ///< is it invalid guid

        size_t allocGuid(const T& t) ///< alloc Guid
        {
            // find elements
            auto find_it = m_elements_guid_map.find(t);
            if (find_it != m_elements_guid_map.end())
            {
                return find_it->second;
            }

            // add new guid
            for (size_t i = 0; i < m_guid_elements_map.size() + 1; i++)
            {
                size_t guid = i + 1;
                if (m_guid_elements_map.find(guid) == m_guid_elements_map.end())
                {
                    m_guid_elements_map.insert(std::make_pair(guid, t));
                    m_elements_guid_map.insert(std::make_pair(t, guid));
                    return guid;
                }
            }

            return s_invalid_guid;
        }

        bool getGuidRelatedElement(size_t guid, T& t) ///< Find an element by guid
        {
            auto find_it = m_guid_elements_map.find(guid);
            if (find_it != m_guid_elements_map.end())
            {
                t = find_it->second;
                return true;
            }
            return false;
        }

        bool getElementGuid(const T& t, size_t& guid) ///< Find an guid by element
        {
            auto find_it = m_elements_guid_map.find(t);
            if (find_it != m_elements_guid_map.end())
            {
                guid = find_it->second;
                return true;
            }
            return false;
        }

        bool hasElement(const T& t)
        {
            return m_elements_guid_map.find(t) != m_elements_guid_map.end();
        } ///< is hes element

        void freeGuid(size_t guid) ///< free guid
        {
            auto find_it = m_guid_elements_map.find(guid);
            if (find_it != m_guid_elements_map.end())
            {
                const auto& ele = find_it->second;
                m_elements_guid_map.erase(ele);
                m_guid_elements_map.erase(guid);
            }
        }

        void freeElement(const T& t) ///< free element
        {
            auto find_it = m_elements_guid_map.find(t);
            if (find_it != m_elements_guid_map.end())
            {
                const auto& guid = find_it->second;
                m_elements_guid_map.erase(t);
                m_guid_elements_map.erase(guid);
            }
        }

        std::vector<size_t> getAllocatedGuids() const ///< get all guild
        {
            std::vector<size_t> allocated_guids;
            for (const auto& ele : m_guid_elements_map)
            {
                allocated_guids.push_back(ele.first);
            }
            return allocated_guids;
        }

        void clear() ///< clear
        {
            m_elements_guid_map.clear();
            m_guid_elements_map.clear();
        }

    private:
        std::unordered_map<T, size_t> m_elements_guid_map; ///< element, guid
        std::unordered_map<size_t, T> m_guid_elements_map; ///< guid, element
    };

} // namespace VKernel
