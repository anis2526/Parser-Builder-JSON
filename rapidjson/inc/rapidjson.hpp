/**
 * @file rapidjson.hpp
 * @author Meimoun Moalla (meimoun.moalla@technica-engineering.de)
 * @brief rapidjson helper
 * @version 0.1
 * @date 2021-04-21
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _UTILITIES_RAPIDJSON_HPP_
#define _UTILITIES_RAPIDJSON_HPP_


#include <unistd.h>
#include <string>
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include "filewritestream.h"
#include "prettywriter.h"
#include "filereadstream.h"
#include "error/en.h"

namespace utilities {


class rapidjson
{
public:
    /**
     * @brief Enumeration of types
     */
    enum class TYPE: uint16_t
    {
        UKNOWN  = 0x0000,
        OBJECT  = 0x0001,
        ARRAY   = 0x0002,
        SINT32  = 0x0004,
        SINT64  = 0x0008,
        UINT32  = 0x0010,
        UINT64  = 0x0020,
        FLOAT   = 0x0040,
        DOUBLE  = 0x0080,
        BOOL    = 0x0100,
        STRING  = 0x0200,
        SINT    = SINT32 | SINT64,
        UINT    = UINT32 | UINT64,
        INT32   = SINT32 | UINT32,
        INT64   = SINT64 | UINT64,
        INT     = SINT   | UINT
    };

    /**
     * @brief Enumeration of json processing
     */
    struct stProcess
    {
        /**
         * @brief enum of the type of the traitment
         * 
         */
        enum class TYPE
        {
            SET      = 0,
            GET      = 1,
            ADD      = 2,
            PUSHBACK = 3,
            REMOVE   = 4,
            SIZE     = 5,
            EXIST    = 6,
            EMPTY    = 7,
            SETINT   = 8,
            SETUINT  = 9,
            SETINT64 = 10,
            SETUINT64= 11
        };

        TYPE eType{TYPE::SET};

        /**
        * @brief Clear data structure
        */
        void clear()
        {
            eType = TYPE::SET;
        }
    };

    /**
     * @brief Enumeration of json Errors
     */
    enum class CHECK_ERROR: uint8_t
    {
        NO_ERROR        = 0,
        UNABLE_TO_GET   = 1,
        BAD_FORMAT      = 2,
        UNEXPECTED_NODE = 3,
        MISSING_NODE    = 4,
        EMPTY_ARRAY     = 5,
        TOO_MANY_ITEMS  = 6,
        TOO_FIEW_ITEMS  = 7,
        BAD_VALUE       = 8,
        OUT_OF_RANGE    = 9
    };

    /**
     * @brief Task callback function type
     */
    typedef std::function<int(rapidjson& json, const std::string& strNode, std::vector<std::pair<std::string, CHECK_ERROR>>& vecCheckRet)> tCallback;

    /**
     * @brief node Configuration structure
     */
    struct stNodeConfig
    {
        bool                                 bOptional    {false};
        bool                                 bNull        {false};
        bool                                 bEmpty       {false};
        TYPE                                 eType        {TYPE::UKNOWN};
        tCallback                            callback     {nullptr};
        std::map<std::string, stNodeConfig>* mapNodeItems {nullptr};
    };

    /**
     * @brief Construct a new rapidjson object
     * @param strFilePath JSON file path
     * @param cNodePathSeparator Node path separator
     */
    rapidjson(const std::string& strFilePath = "", const char& cNodePathSeparator = '.');

    /**
     * @brief Destroy the rapidjson object
     */
    virtual ~rapidjson();

    /**
     * @brief Get JSON file path
     * @return std::string Processing result (File path)
     */
    std::string path();
    
    /**
     * @brief load the json file
     * 
     * @param strFilePath JSON file path
     * @return int Processing result
     */
    int load(const std::string& strFilePath = "");

    /**
     * @brief write and save the json file
     * 
     * @param strFilePath JSON file path
     * @return int int Processing result
     */
    int save(const std::string& strFilePath = "");

    /**
     * @brief Get json string
     * 
     * @param bPrettyWriter JSON format writer status 
     * @return std::string Processing result
     */
    std::string get(const bool& bPrettyWriter = false);

    /**
     * @brief Set json string
     * 
     * @param strData JSON data
     * @return std::string Processing result
     */
    int set(const std::string& strData);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param strValue Value to retrieve
     * @param strNode JSON node path
     * @return int Processing result
     */
    int get(std::string& strValue, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param iValue Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(int& iValue, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param ulValue Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(uint64_t& ulValue, const std::string& strNode);
    
    /**
     * @brief Get JSON object value based its path
     * 
     * @param uiValue Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(uint32_t& uiValue, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param usValue Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(uint16_t& usValue, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param ucValue Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(uint8_t& ucValue, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param lValue Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(int64_t& lValue, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param sValue Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(int16_t& sValue, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param lValue Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(int8_t& cValue, const std::string& strNode);
    

    /**
     * @brief Get JSON object value based its path
     * 
     * @param fValue Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(float& fValue, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param dValue Value to retrieve
     * @param strNode JSON node path
     * @return int Processing result
     */
    int get(double& dValue, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param bValue Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(bool& bValue, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(std::vector<std::string>& vecValues, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(std::vector<uint64_t>& vecValues, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(std::vector<uint32_t>& vecValues, const std::string& strNode);

        /**
     * @brief Get JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(std::vector<uint16_t>& vecValues, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(std::vector<uint8_t>& vecValues, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(std::vector<int>& vecValues, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(std::vector<int64_t>& vecValues, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(std::vector<int16_t>& vecValues, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(std::vector<int8_t>& vecValues, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(std::vector<float>& vecValues, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path
     * @return int Processing result
     */
    int get(std::vector<double>& vecValues, const std::string& strNode);

    /**
     * @brief Get JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int get(std::vector<bool>& vecValues, const std::string& strNode);

    /**
     * @brief Get the type of specific node
     * 
     * @param strNode JSON node path 
     * @return JSON_TYPE type of the node
     */
    TYPE getType(const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param strValue Value to retrieve
     * @param strNode JSON node path
     * @return int Processing result
     */
    int set(const std::string& strValue, const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param cValue Value to retrieve
     * @param strNode JSON node path
     * @return int Processing result
     */
    int set(char* cValue, const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param iValue Value to retrieve
     * @param strNode JSON node path
     * @return int Processing result
     */

    int set(const int& iValue, const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param ulValue Value to retrieve
     * @param strNode JSON node path
     * @return int Processing result
     */
    int set(const uint64_t& ulValue, const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param uiValue Value to retrieve
     * @param strNode JSON node path
     * @return int Processing result
     */
    int set(const uint32_t& uiValue, const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param usValue Value to retrieve
     * @param strNode JSON node path
     * @return int Processing result
     */
    int set(const uint16_t& usValue, const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param cValue Value to retrieve
     * @param strNode JSON node path
     * @return int Processing result
     */
    int set(const uint8_t& cValue, const std::string& strNode);

    /**
     * @brief set JSON object value based its path
     * 
     * @param lValue Value to retrieve
     * @param strNode JSON node path
     * @return int Processing result
     */
    int set(const int64_t& lValue, const std::string& strNode);
    
    /**
     * @brief set JSON object value based its path
     * 
     * @param sValue Value to retrieve
     * @param strNode JSON node path
     * @return int Processing result
     */
    int set(const int16_t& sValue, const std::string& strNode);

    /**
     * @brief set JSON object value based its path
     * 
     * @param cValue Value to retrieve
     * @param strNode JSON node path
     * @return int Processing result
     */
    int set(const int8_t& cValue, const std::string& strNode);

    /**
     * @brief set JSON object value based its path
     * 
     * @param fValue Value to retrieve
     * @param strNode JSON node path
     * @return int Processing result
     */
    int set(const float& fValue, const std::string& strNode);

    /**
     * @brief set JSON object value based its path
     * 
     * @param fValue Value to retrieve
     * @param strNode JSON node path
     * @return int Processing result
     */
    int set(const double& fValue, const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param bValue Value to retrieve
     * @param strNode JSON node path
     * @return int Processing result
     */
    int set(const bool& bValue, const std::string& strNode);

     /**
     * @brief Set JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int set(const std::vector<std::string>& vecValues, const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int set(const std::vector<uint64_t>& vecValues, const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int set(const std::vector<uint32_t>& vecValues, const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int set(const std::vector<uint16_t>& vecValues, const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int set(const std::vector<uint8_t>& vecValues, const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int set(const std::vector<int>& vecValues, const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int set(const std::vector<int64_t>& vecValues, const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int set(const std::vector<int16_t>& vecValues, const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int set(const std::vector<int8_t>& vecValues, const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int set(const std::vector<float>& vecValues, const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path
     * @return int Processing result
     */
    int set(const std::vector<double>& vecValues, const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @param vecValues Value to retrieve
     * @param strNode JSON node path 
     * @return int Processing result
     */
    int set(const std::vector<bool>& vecValues, const std::string& strNode);

    /**
     * @brief remove JSON object value based its path
     * 
     * @param strNode JSON node path
     * @return int int Processing result
     */
    int remove(const std::string& strNode);

    /**
     * @brief Verify JSON object emty or not  based its path
     * 
     * @param strNode JSON node path
     * @return bool Processing result
     */
    bool empty(const std::string& strNode);

    /**
     * @brief Verify JSON object exist or not  based its path
     * 
     * @param strNode JSON node path
     * @return bool Processing result
     */
    bool exist(const std::string& strNode);
    
    /**
     * @brief Get the Size object
     * 
     * @param strNode JSON node path
     * @return uint32_t Processing result
     */
    uint32_t size(const std::string& strNode);

    /**
     * @brief Check configuration
     *
     * @param strConfig Configuration string
     * @param mapNodes Nodes map 
     * @param vecCheckRet Check result vector
     * @return int Processing result
     */
    int check(const std::string& strConfig, const std::map<std::string, stNodeConfig>& mapNodes, std::vector<std::pair<std::string, CHECK_ERROR>>& vecCheckRet);
    
private:
    /**
     * @brief JSON file path
     */
    std::string m_strFilePath;

    /**
     * @brief rapidjson document
     */
    ::rapidjson::Document m_docJsonFile;

    /**
     * @brief rapid json document for child object
     */
    
    ::rapidjson::Document m_childDocJsonFile;

    /**
     * @brief get the allocator of the json document
     */
    ::rapidjson::Document::AllocatorType& m_allocator = m_docJsonFile.GetAllocator();

    /**
     * @brief Node path separator
     */
    const char m_cNodePathSeparator;

    /**
     * @brief child object
     */
    ::rapidjson::Value m_childObject;

    /**
     * @brief Check node
     * 
     * @param strNode JSON node path
     * @param nodeConfig Node configuration 
     * @param vecCheckRet Check result vector
     * @return int Processing result
     */
    int check(const std::string& strNode, const stNodeConfig& nodeConfig, std::vector<std::pair<std::string, CHECK_ERROR>>& vecCheckRet);

    /**
     * @brief Get the elemnts of specific node
     * 
     * @param strNode JSON node path 
     * @return std::vector<std::string> vector of elements names
     */
    std::vector<std::string> getElements(const std::string& strNode);

    /**
     * @brief Set JSON object value based its path
     * 
     * @tparam T_VALUE 
     * @param jsonObject JSON object
     * @param tValue Value to retrieve
     * @param strNode JSON node path
     * @return int Processing result
     */
    template<typename T_VALUE>
    inline int set(::rapidjson::Value& jsonObject, const T_VALUE& tValue, const std::string& strNode);

    /**
     * @brief processing of JSON object value based its path
     * 
     * @tparam T_VALUE 
     * @param jsonObject object of the json file
     * @param tValue Value to retrieve
     * @param strNode JSON node path
     * @param eType Type of the process
     * @return int rocessing result
     */
    template<typename T_VALUE>
    inline int process(::rapidjson::Value& jsonObject, T_VALUE& tValue, const std::string& strNode, const stProcess::TYPE& eType);

    /**
     * @brief processing of JSON object value based its path
     * 
     * @param jsonObject object of the json file
     * @param strNode JSON node path
     * @return JSON_TYPE type of node
     */
    inline TYPE process(::rapidjson::Value& jsonObject, const std::string& strNode);

    /**
     * @brief Get the type of a specific node
     * 
     * @param jsonObject object of the json file
     * @return JSON_TYPE type of node
     */
    inline TYPE type(::rapidjson::Value& jsonObject);

    /**
     * @brief Get the elemnt of a specific type
     * @tparam T_VALUE 
     * @param jsonObject object of the json file
     * @param tValue Value to retrieve
     * @return int rocessing result
     */
    template<typename T_VALUE>
    inline int element(::rapidjson::Value& jsonObject, T_VALUE& tValue);

    /**
     * @brief processing of JSON object value based its path
     * 
     * @param jsonObject object of the json file
     * @param strNode JSON node path
     * @return std::vector<std::string> list of elemnts
     */
    inline std::vector<std::string> processList(::rapidjson::Value& jsonObject, const std::string& strNode);

    /**
     * @brief get JSON object value based its path
     * 
     * @tparam T_VALUE 
     * @param jsonObject object of the json file
     * @param tValue Value to retrieve
     * @param strNode JSON node path
     * @return int rocessing result
     */
    template<typename T_VALUE>
    inline int get(::rapidjson::Value& jsonObject, T_VALUE& tValue, const std::string& strNode);
    /**
     * @brief remove JSON object value based its path
     * 
     * @param jsonObject object of the json file
     * @param strNode  Value to retrieve
     * @return int processing result
     */
    inline int remove(::rapidjson::Value& jsonObject, const std::string& strNode);

    /**
     * @brief Get the Size object
     * 
     * @param jsonObject object of the json file
     * @param strNode Value to retrieve
     * @return uint32_t processing result
     */
    inline uint32_t size(::rapidjson::Value& jsonObject, const std::string& strNode);

    /**
     * @brief Cast data 
     * 
     * @tparam T_TYPEA 
     * @param memberName 
     * @param tType data type
     * @param jsonObject json array 
     * @param eType type of process
     * @return int processing result
     */
    template<typename T_TYPEA>
    inline int castData(::rapidjson::Value& memberName , const T_TYPEA& tType, ::rapidjson::Value& jsonObject, const stProcess::TYPE& eType);
    
    /**
     * @brief Get the Element object
     * 
     * @tparam T_VALUE 
     * @param tValue data value
     * @param jsonObject json array 
     * @param uiArrayElemnt the number of the array element
     * @return int processing result
     */
    template<typename T_VALUE>
    inline int getElement(T_VALUE& tValue, ::rapidjson::Value& jsonObject, const uint64_t& uiArrayElemnt);

    /**
     * @brief Set the Element object
     * 
     * @tparam T_VALUE 
     * @param tValue data value
     * @param jsonObject json array 
     * @param uiArrayElemnt the number of the array element
     * @param eType type of process
     * @return int processing result
     */
    template<typename T_VALUE>
    inline int setElement(::rapidjson::Value& memberName , const T_VALUE& tValue, ::rapidjson::Value& jsonObject, const stProcess::TYPE& eType, const stProcess::TYPE& eDataType);

    /**
     * @brief Get the vector of elements 
     * 
     * @tparam T_VALUE 
     * @param vecValue data value
     * @param strNode path of the node
     * @return int processing result
     */
    template<typename T_VALUE>
    inline int getVector(std::vector<T_VALUE>& vecValue, const std::string& strNode);

    /**
     * @brief Set the vector of elements 
     * 
     * @tparam T_VALUE 
     * @param vecValue data value
     * @param strNode path of the node
     * @return int processing result
     */
    template<typename T_VALUE>
    inline int setVector(const std::vector<T_VALUE>& vecValue, const std::string& strNode);

    /**
     * @brief Split a given path
     * 
     * @param strPath original path
     * @param strNode node path
     * @param strChildren children
     * @param uiArrayElemnt array number
     * @return int processing result
     */
    inline int split(const std::string& strPath, std::string& strNode, std::string& strChildren, uint64_t& uiArrayElemnt);
};


} // namespace utilities


#endif //_UTILITIES_RAPIDJSON_HPP_
