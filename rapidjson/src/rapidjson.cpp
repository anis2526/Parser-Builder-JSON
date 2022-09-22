/**
 * @file rapidjson.cpp
 * @author Meimoun Moalla (meimoun.moalla@technica-engineering.de)
 * @brief rapidjson helper
 * @version 0.1
 * @date 2021-04-21
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "rapidjson.hpp"
#include "string.hpp"
#include "file.hpp"


namespace utilities {


#define IS_OBJECT        UINT64_MAX
#define FILE_BUFFER_SIZE 128 * 1024


/**
 * @brief Construct a new rapidjson::rapidjson object
 * 
 * @param strFilePath JSON file path
 * @param cNodePathSeparator Node path separator
 */
rapidjson::rapidjson(const std::string& strFilePath, const char& cNodePathSeparator):
    m_strFilePath       {strFilePath},
    m_cNodePathSeparator{cNodePathSeparator}
{
    m_docJsonFile.SetObject();
}

/**
 * @brief Destroy the rapidjson:: rapidjson object
 */
rapidjson::~rapidjson()
{
}

/**
 * @brief Get JSON file path
 * 
 * @return std::string Processing result (File path)
 */
std::string rapidjson::path()
{
    return m_strFilePath;
}

/**
 * @brief load the json file
 * 
 * @param strFilePath JSON file path
 * @return int Processing result
 */
int rapidjson::load(const std::string& strFilePath)
{
    int iRet = 0;
    // Retrieve JSON file name
    std::string strFile = strFilePath.empty()? m_strFilePath: strFilePath;
    // Check if file exist
    if(!fs::file::exist(strFile))
    {
        // file not exist
        iRet = -1; 
    }
    // Read file stream
    if(!iRet)
    {
        FILE* pFile = fopen(strFile.c_str(), "r");
        if(pFile)
        {
            char* pcReadBuffer = new char[FILE_BUFFER_SIZE];
            ::rapidjson::FileReadStream fileStream(pFile, pcReadBuffer, FILE_BUFFER_SIZE);
            if(m_docJsonFile.ParseStream(fileStream).HasParseError() == true)
            {
                iRet = -1;
            }
            // Delete allocated buffer
            delete[] pcReadBuffer;
            // Close file
            fclose(pFile);
        }
        else
        {
            iRet = -1;
        }
    }

    // Processing result
    return iRet;
}

/**
 * @brief write and save the json file
 * 
 * @param strFilePath JSON file path
 * @return int int Processing result
 */
int rapidjson::save(const std::string& strFilePath)
{
    int iRet = 0;
    // Retrieve JSON file name
    std::string strFile = strFilePath.empty()? m_strFilePath: strFilePath;
    if(!iRet)
    {
        FILE* pFile = fopen(strFile.c_str(), "w");
        if(pFile)
        {
            char* pcWriteBuffer = new char[FILE_BUFFER_SIZE];
            ::rapidjson::FileWriteStream fileWriterStream(pFile, pcWriteBuffer, FILE_BUFFER_SIZE);
            ::rapidjson::PrettyWriter<::rapidjson::FileWriteStream> writer(fileWriterStream);
            if(!m_docJsonFile.Accept(writer))
            {
                iRet = -1;
            }
            delete[] pcWriteBuffer;
            iRet = fclose(pFile);
        }
        else
        {
            iRet = -2;
        }
    }
    
    // Processing result
    return iRet;
}

/**
 * @brief Get json string
 * 
 * @param bPrettyWriter JSON format writer status
 * @return std::string Processing result
 */
std::string rapidjson::get(const bool& bPrettyWriter)
{
    std::string strRet;
    ::rapidjson::StringBuffer jsonBuffer;
    
    if(bPrettyWriter)
    {
        ::rapidjson::PrettyWriter<::rapidjson::StringBuffer> writer(jsonBuffer);
        if(m_docJsonFile.Accept(writer))
        {
            strRet.assign(jsonBuffer.GetString(), jsonBuffer.GetLength());
        }
    } 
    else
    {
        ::rapidjson::Writer<::rapidjson::StringBuffer> writer(jsonBuffer);
        if(m_docJsonFile.Accept(writer))
        {
            strRet.assign(jsonBuffer.GetString(), jsonBuffer.GetLength());
        }
    }
    
    // Return processing result
    return strRet;
}

/**
 * @brief Set json string
 * 
 * @param strData JSON data
 * @return std::string Processing result
 */
int rapidjson::set(const std::string& strData)
{
    // Return processing result
    return m_docJsonFile.Parse(strData.c_str()).HasParseError()? -1: 0;
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param strValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::get(std::string& strValue, const std::string& strNode)
{   // Processing result
    return process<std::string>(m_docJsonFile, strValue, strNode, stProcess::TYPE::GET);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param iValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::get(int& iValue, const std::string& strNode)
{   // Processing result
    return process<int>(m_docJsonFile, iValue, strNode, stProcess::TYPE::GET);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param uiValue Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::get(uint64_t& ulValue, const std::string& strNode)
{   // Processing result
    return process<uint64_t>(m_docJsonFile, ulValue, strNode, stProcess::TYPE::GET);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param uiValue Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::get(uint32_t& uiValue, const std::string& strNode)
{  // Processing result
    return process<uint32_t>(m_docJsonFile, uiValue, strNode, stProcess::TYPE::GET);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param uValue Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::get(uint16_t& uValue, const std::string& strNode)
{   // Processing result
    return process<uint16_t>(m_docJsonFile, uValue, strNode, stProcess::TYPE::GET);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param ucValue Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::get(uint8_t& ucValue, const std::string& strNode)
{   // Processing result
    return process<uint8_t>(m_docJsonFile, ucValue, strNode, stProcess::TYPE::GET);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param lValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::get(int64_t& lValue, const std::string& strNode)
{   // Processing result
    return process<int64_t>(m_docJsonFile, lValue, strNode, stProcess::TYPE::GET);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param sValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::get(int16_t& sValue, const std::string& strNode)
{   // Processing result
    return process<int16_t>(m_docJsonFile, sValue, strNode, stProcess::TYPE::GET);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param cValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::get(int8_t& cValue, const std::string& strNode)
{   // Processing result
    return process<int8_t>(m_docJsonFile, cValue, strNode, stProcess::TYPE::GET);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param fValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::get(float& fValue, const std::string& strNode)
{   // Processing result
    return process<float>(m_docJsonFile, fValue, strNode, stProcess::TYPE::GET);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param dValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::get(double& dValue, const std::string& strNode)
{   // Processing result
    return process<double>(m_docJsonFile, dValue, strNode, stProcess::TYPE::GET);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param bValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::get(bool& bValue, const std::string& strNode)
{   // Processing result
    return process<bool>(m_docJsonFile, bValue, strNode, stProcess::TYPE::GET);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::get(std::vector<std::string>& vecValues, const std::string& strNode)
{
    return getVector(vecValues, strNode);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::get(std::vector<uint64_t>& vecValues, const std::string& strNode)
{
    return getVector(vecValues, strNode);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::get(std::vector<uint32_t>& vecValues, const std::string& strNode)
{
    return getVector(vecValues, strNode);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::get(std::vector<uint16_t>& vecValues, const std::string& strNode)
{
    return getVector(vecValues, strNode);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::get(std::vector<uint8_t>& vecValues, const std::string& strNode)
{
    return getVector(vecValues, strNode);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::get(std::vector<int>& vecValues, const std::string& strNode)
{
    return getVector(vecValues, strNode);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::get(std::vector<int64_t>& vecValues, const std::string& strNode)
{
    return getVector(vecValues, strNode);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::get(std::vector<int16_t>& vecValues, const std::string& strNode)
{
    return getVector(vecValues, strNode);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::get(std::vector<int8_t>& vecValues, const std::string& strNode)
{
    return getVector(vecValues, strNode);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::get(std::vector<float>& vecValues, const std::string& strNode)
{
    return getVector(vecValues, strNode);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::get(std::vector<double>& vecValues, const std::string& strNode)
{
    return getVector(vecValues, strNode);
}

/**
 * @brief Get JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::get(std::vector<bool>& vecValues, const std::string& strNode)
{
    return getVector(vecValues, strNode);
}

/**
 * @brief Get the type of specific node
 * 
 * @param strNode JSON node path 
 * @return TYPE type of the node
 */
rapidjson::TYPE rapidjson::getType(const std::string& strNode)
{   // Processing result
    return process(m_docJsonFile, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param cValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::set(char* cValue, const std::string& strNode)
{    // Processing result
    std::string strValue = std::string(cValue);
    return set<std::string>(m_docJsonFile, strValue, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param strValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::set(const std::string& strValue, const std::string& strNode)
{  // Processing result
    return set<std::string>(m_docJsonFile, strValue, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param iValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::set(const int& iValue, const std::string& strNode)
{   
    return set<int>(m_docJsonFile, iValue, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param ulValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::set(const uint64_t& ulValue, const std::string& strNode)
{   
    return set<uint64_t>(m_docJsonFile, ulValue, strNode);
}
/**
 * @brief Set JSON object value based its path
 * 
 * @param uiValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::set(const uint32_t& uiValue, const std::string& strNode)
{   
    return set<uint32_t>(m_docJsonFile, uiValue, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param usValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::set(const uint16_t& usValue, const std::string& strNode)
{   
    return set<uint16_t>(m_docJsonFile, usValue, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param cValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::set(const uint8_t& cValue, const std::string& strNode)
{   
    return set<uint8_t>(m_docJsonFile, cValue, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param lValue Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::set(const int64_t& lValue, const std::string& strNode)
{   
    return set<int64_t>(m_docJsonFile, lValue, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param sValue Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::set(const int16_t& sValue, const std::string& strNode)
{   
    return set<int16_t>(m_docJsonFile, sValue, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param cValue Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::set(const int8_t& cValue, const std::string& strNode)
{   
    return set<int8_t>(m_docJsonFile, cValue, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param fValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::set(const float& fValue, const std::string& strNode)
{   
    return set<float>(m_docJsonFile, fValue, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param fValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::set(const double& fValue, const std::string& strNode)
{   
    return set<double>(m_docJsonFile, fValue, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param bValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::set(const bool& bValue, const std::string& strNode)
{   
    return set<bool>(m_docJsonFile, bValue, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::set(const std::vector<std::string>& vecValues, const std::string& strNode)
{
    return setVector(vecValues, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::set(const std::vector<uint64_t>& vecValues, const std::string& strNode)
{
    return setVector(vecValues, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::set(const std::vector<uint32_t>& vecValues, const std::string& strNode)
{
    return setVector(vecValues, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::set(const std::vector<uint16_t>& vecValues, const std::string& strNode)
{
    return setVector(vecValues, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::set(const std::vector<uint8_t>& vecValues, const std::string& strNode)
{
    return setVector(vecValues, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::set(const std::vector<int>& vecValues, const std::string& strNode)
{
    return setVector(vecValues, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::set(const std::vector<int64_t>& vecValues, const std::string& strNode)
{
    return setVector(vecValues, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::set(const std::vector<int16_t>& vecValues, const std::string& strNode)
{
    return setVector(vecValues, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::set(const std::vector<int8_t>& vecValues, const std::string& strNode)
{
    return setVector(vecValues, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::set(const std::vector<float>& vecValues, const std::string& strNode)
{
    return setVector(vecValues, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::set(const std::vector<double>& vecValues, const std::string& strNode)
{
    return setVector(vecValues, strNode);
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param vecValues Value to retrieve
 * @param strNode JSON node path 
 * @return int Processing result
 */
int rapidjson::set(const std::vector<bool>& vecValues, const std::string& strNode)
{
    return setVector(vecValues, strNode);
}
/**
 * @brief Remove object
 * 
 * @param strNode JSON node path
 * @return int Processing result
 */
int rapidjson::remove(const std::string& strNode)
{
    int iRemove = 0;
    return process(m_docJsonFile, iRemove, strNode, stProcess::TYPE::REMOVE);
}

/**
 * @brief check if object is empty
 * 
 * @param strNode JSON node path
 * @return bool Processing result
 */
bool rapidjson::empty(const std::string& strNode)
{
    bool bRet  = false;
    int iEmpty = 0;
    if(process(m_docJsonFile, iEmpty, strNode, stProcess::TYPE::EMPTY))
    {
        bRet = true;
    }
    return bRet;
}

/**
 * @brief check if  object exists
 * 
 * @param strNode JSON node path
 * @return bool Processing result
 */
bool rapidjson::exist(const std::string& strNode)
{
    bool bRet  = false;
    int iExist = 0;
    if(!process(m_docJsonFile, iExist, strNode, stProcess::TYPE::EXIST))
    {
        bRet = true;
    }
    return bRet;
}

/**
 * @brief Get the size of the array based its path
 * 
 * @param strNode JSON node path
 * @return uint32_t Processing result
 */
uint32_t rapidjson::size(const std::string& strNode)
{
    return size(m_docJsonFile, strNode);
}

/**
 * @brief Check configuration
 *
 * @param strConfig Configuration string
 * @param mapNodes Nodes map
 * @param vecCheckRet Check result vector
 * @return int Processing result
 */
int rapidjson::check(const std::string& strConfig, const std::map<std::string, stNodeConfig>& mapNodes, std::vector<std::pair<std::string, CHECK_ERROR>>& vecCheckRet)
{
    int iRet = 0;
    // Initialization
    vecCheckRet.clear();
    
    // Check if strConfig is empty
    if(strConfig.empty())
    { 
        iRet = -1;
    }
    if(!iRet)
    {
        // Load string configuration in a json object
        iRet = set(strConfig);
    }
    if(!iRet)
    {
        // Loop over all list items
        for(const std::pair<std::string, stNodeConfig> &pairMapNodes: mapNodes)
        {
           // Get node 
           const std::string&  strPath    = pairMapNodes.first;
           const stNodeConfig& nodeConfig = pairMapNodes.second;
           // Check node
           iRet = check(strPath, nodeConfig, vecCheckRet);
        }
        // Check if Check result vector is empty
        if(!vecCheckRet.empty())
        {
            iRet = -2;
        }
    }

    // Return processing result
    return iRet;
}

/**
 * @brief Check node
 * 
 * @param strNode JSON node path
 * @param nodeConfig Node configuration 
 * @param vecCheckRet Check result vector
 * @return int Processing result
 */
int rapidjson::check(const std::string& strNode, const stNodeConfig& nodeConfig, std::vector<std::pair<std::string, CHECK_ERROR>>& vecCheckRet)
{
    int         iRet    = 0;
    std::string strPath = strNode; 

    // Check if node exist
    if(exist(strPath))
    {// Node exist
        // Check if node is null 
        if(!empty(strPath))
        {// Node is not null 
            // Check node type
            if(!(static_cast<uint16_t>(getType(strPath)) & static_cast<uint16_t>(nodeConfig.eType)))
            {
                // Set error
                vecCheckRet.push_back({strPath, CHECK_ERROR::BAD_FORMAT});
                iRet = -1;
            }
            // Check if Node is Empty (case: string/array)
            if(!iRet)
            {
                // Switch node type
                switch(nodeConfig.eType)
                {
                    // Case string node   
                    case TYPE::STRING:
                    {// Node is empty but should not be empty
                        std::string strElement;
                        if((!get(strElement, strPath)) && 
                           (strElement.empty())        &&
                           (!nodeConfig.bEmpty))
                        {
                            // set error
                            vecCheckRet.push_back({strPath, CHECK_ERROR::BAD_VALUE});
                            iRet = -1;
                        }
                        break;
                    }
                    // Case array node 
                    case TYPE::ARRAY:
                    {// Node is empty but should not be empty
                        uint32_t uiSize = size(strPath);
                        if((uiSize == 0) && 
                           (!nodeConfig.bEmpty))
                        {
                            // Set error
                            vecCheckRet.push_back({strPath, CHECK_ERROR::EMPTY_ARRAY});
                            iRet = -1;
                        }
                        if((!iRet) &&
                           (nodeConfig.mapNodeItems != nullptr))
                        {
                            int iRetItem = 0;
                            // Loop over all items
                            for(uint32_t uiCnt = 0; uiCnt < uiSize; ++uiCnt)
                            { 
                                strPath = strNode + "[" + std::to_string(uiCnt) + "]";

                                if(getType(strPath) != TYPE::OBJECT)
                                {
                                    vecCheckRet.push_back({strPath, CHECK_ERROR::BAD_FORMAT});
                                    iRetItem = -1;
                                    iRet = -1;
                                }
                                if(!iRetItem)
                                {
                                    std::string strPathNodeItem = strPath; 
                                    // Loop over all list items
                                    for(const std::pair<std::string, stNodeConfig>& pairMapNodeItem: *nodeConfig.mapNodeItems)
                                    {
                                        // Get node 
                                        const std::string& strNodeItem = pairMapNodeItem.first;
                                        // Get node config
                                        const stNodeConfig& nodeConfigItem = pairMapNodeItem.second;
                                        // Check node items
                                        strPathNodeItem = strPath + "." + strNodeItem;
                                        iRet = check(strPathNodeItem, nodeConfigItem, vecCheckRet);
                                    }
                                }
                                else
                                {
                                    // Reset iRetItem
                                    iRetItem = 0;
                                }
                            }
                        }
                        break;
                    }
                    case TYPE::OBJECT:
                    {
                        if(nodeConfig.mapNodeItems != nullptr)
                        {
                            std::string strPathNodeItem = strPath; 
                            // Loop over all list items
                            for(const std::pair<std::string, stNodeConfig>& pairMapNodeItem: *nodeConfig.mapNodeItems)
                            {
                                // Get node 
                                const std::string& strNodeItem = pairMapNodeItem.first;
                                // Get node config
                                const stNodeConfig& nodeConfigItem = pairMapNodeItem.second;
                                // Check node items
                                strPathNodeItem = strPath + "." + strNodeItem;
                                iRet = check(strPathNodeItem, nodeConfigItem, vecCheckRet);
                            }
                        }
                        break;
                    }
                    default: 
                        break;
                }
            }
            if((!iRet) && 
               (nodeConfig.callback != nullptr))
            {
                // Call the callback function
                iRet = nodeConfig.callback(*this, strPath, vecCheckRet);
            }
        }
        else if((!nodeConfig.bNull))
        {// Node is null but should not be null 
            // Set error
            vecCheckRet.push_back({strPath, CHECK_ERROR::UNABLE_TO_GET});
            iRet = -1;
        }   
    }
    else if(!nodeConfig.bOptional)
    {// Node dos not exist but mandatory
        // Set error
        vecCheckRet.push_back({strPath, CHECK_ERROR::MISSING_NODE});
        iRet = -1;
    }

    // Return processing result
    return iRet;
}

/**
 * @brief Get the elemnts of specific node
 * 
 * @param strNode JSON node path 
 * @return std::vector<std::string> vector of elements names
 */
std::vector<std::string> rapidjson::getElements(const std::string& strNode)
{   // Processing result
    return processList(m_docJsonFile, strNode);   
}

/**
 * @brief Set JSON object value based its path
 * 
 * @param jsonObject object of the json file
 * @param strValue Value to retrieve
 * @param strNode JSON node path
 * @return int Processing result
 */
template<typename T_VALUE>
int rapidjson::set(::rapidjson::Value& jsonObject, const T_VALUE& tValue, const std::string& strNode)
{
    int iRet = 0;
    ::rapidjson::Document docJsonFile;
    // Split path into 'node' & 'children'
    std::string strNodePath;
    std::string strChildren;
    uint64_t ulArrayElemnt = 0;
    split(strNode, strNodePath, strChildren, ulArrayElemnt);
    // Check node name
    if(strNodePath.empty())
    {
        iRet = -1;
    }
    // 
    if(!iRet)
    {   
        ::rapidjson::Value memberName(strNodePath.c_str(), strNodePath.size(), m_allocator);
        // Process node
        if(strChildren.empty())
        {// Node has no childs (Terminal node)
            // check if is the object exist or not
            ::rapidjson::Document childDocJsonFile;
            if (jsonObject.ObjectEmpty() || jsonObject.IsNull())
            {   // add the object
                if (ulArrayElemnt == IS_OBJECT)
                {
                    jsonObject.SetObject();
                    iRet = castData(memberName, tValue, jsonObject,stProcess::TYPE::ADD);
                }
                else
                {
                    ::rapidjson::Value arrchildObject(::rapidjson::kArrayType);
                    iRet = castData(memberName, tValue, jsonObject,stProcess::TYPE::PUSHBACK);
                    jsonObject.AddMember(memberName, arrchildObject, m_allocator);
                }
            }
            else
            {// check if this object is a member or not
                if (jsonObject.HasMember(strNodePath.c_str()))
                {   // Set the object
                    if (ulArrayElemnt == IS_OBJECT)
                    {
                        ::rapidjson::Value& childObject = jsonObject[strNodePath.c_str()];
                        iRet = castData(memberName, tValue, childObject,stProcess::TYPE::SET);
                    }
                    else
                    {   
                        if (jsonObject[strNodePath.c_str()].Size() > ulArrayElemnt)
                        {
                            ::rapidjson::Value& childObject = jsonObject[strNodePath.c_str()][ulArrayElemnt];
                            iRet = castData(memberName, tValue, childObject,stProcess::TYPE::SET);
                        }
                        else
                        {
                            ::rapidjson::Value& childObject = jsonObject[strNodePath.c_str()];
                            iRet = castData(memberName, tValue, childObject,stProcess::TYPE::PUSHBACK);
                        }
                    } 
                }
                else
                {   // add the object if it is not a member
                    if (ulArrayElemnt == IS_OBJECT)
                    {
                        childDocJsonFile.SetObject();
                        iRet = castData(memberName, tValue, jsonObject,stProcess::TYPE::ADD);
                    }
                    else
                    {
                        ::rapidjson::Value arrchildObject(::rapidjson::kArrayType);
                        iRet = castData(memberName, tValue, arrchildObject,stProcess::TYPE::PUSHBACK);
                        jsonObject.AddMember(memberName,arrchildObject, m_allocator);
                    }
                }  
            }
        }
        else
        {// Node has children
            if (jsonObject.ObjectEmpty() || jsonObject.IsNull())
            {  // If the children is emty set the object and vlaue
                if (ulArrayElemnt == IS_OBJECT)
                {
                    m_childDocJsonFile.SetObject();
                    ::rapidjson::Value childObject(::rapidjson::kObjectType);
                    iRet = set(childObject, tValue, strChildren);
                    jsonObject.AddMember(memberName, childObject, m_childDocJsonFile.GetAllocator());
                }
                else
                {
                    m_childDocJsonFile.SetObject();
                    ::rapidjson::Value member(strNodePath.c_str(), strNodePath.size(),m_childDocJsonFile.GetAllocator());
                    ::rapidjson::Value childObject(::rapidjson::kObjectType);
                    iRet = set(childObject, tValue, strChildren);
                    ::rapidjson::Value arrchildObject(::rapidjson::kArrayType);
                    arrchildObject.PushBack(childObject, m_childDocJsonFile.GetAllocator());
                    jsonObject.AddMember(member,arrchildObject, m_childDocJsonFile.GetAllocator());
                }
            }
            else
            {
                if (ulArrayElemnt == IS_OBJECT)
                {
                    if (jsonObject.HasMember(strNodePath.c_str()))
                    {   // Set the chilf id the object is member
                        ::rapidjson::Value& childObject = jsonObject[strNodePath.c_str()];
                        if (childObject.IsObject())
                        {
                            iRet = set(childObject, tValue, strChildren);
                        }
                        else
                        {
                            iRet = -1;
                        }    
                    }
                    else
                    {   // add the object if it is not a member
                        m_childDocJsonFile.SetObject();
                        ::rapidjson::Value childObject(::rapidjson::kObjectType);
                        iRet = set(childObject, tValue, strChildren);
                        jsonObject.AddMember(memberName, childObject, m_allocator);
                    } 
                }
                else
                {
                    if (jsonObject.HasMember(strNodePath.c_str()))
                    {   // Set the chilf id the object is member
                        if (jsonObject[strNodePath.c_str()].Size() > ulArrayElemnt)
                        {
                            ::rapidjson::Value& childObject = jsonObject[strNodePath.c_str()][ulArrayElemnt];
                            if (childObject.IsObject())
                            {
                                iRet = set(childObject, tValue, strChildren);
                            }
                            else
                            {
                                iRet = -1;
                            }
                        }
                        else
                        {
                            ::rapidjson::Value& arrchildObject = jsonObject[strNodePath.c_str()];
                            if (arrchildObject.IsArray() )
                            { 
                                ::rapidjson::Value childObject(::rapidjson::kObjectType);
                                iRet = set(childObject, tValue, strChildren);
                                arrchildObject.PushBack(childObject , m_allocator);
                            }
                            else 
                            {
                                iRet = -1;
                            }
                        }   
                    }
                    else
                    {   // add the object if it is not a member
                        m_childDocJsonFile.SetObject();
                        ::rapidjson::Value member(strNodePath.c_str(), strNodePath.size(),m_childDocJsonFile.GetAllocator());
                        ::rapidjson::Value childObject(::rapidjson::kObjectType);
                        iRet = set(childObject, tValue, strChildren);
                        ::rapidjson::Value arrchildObject(::rapidjson::kArrayType);
                        arrchildObject.PushBack(childObject,m_childDocJsonFile.GetAllocator());
                        jsonObject.AddMember(member,arrchildObject, m_allocator);
                    } 
                }
            }
        }
    }

    // Processing result
    return iRet;
}

/**
 * @brief Get the Size object
 * 
 * @param jsonObject object of the json file
 * @param strNode Value to retrieve
 * @return uint32_t processing result
 */
uint32_t rapidjson::size(::rapidjson::Value& jsonObject, const std::string& strNode)
{
    uint32_t uiRet = 0;
    // Split path into 'node' & 'children'
    std::string strNodePath;
    std::string strChildren;
    uint64_t ulArrayElemnt = 0;
    split(strNode, strNodePath, strChildren, ulArrayElemnt);

    // Check node name
    if(strNodePath.empty())
    {
        uiRet = 0;
    }
    // 
    if(!uiRet)
    {   
        ::rapidjson::Value memberName(strNodePath.c_str(), strNodePath.size(), m_allocator);
        // Process node
        if(strChildren.empty())
        {// Node has no childs (Terminal node)
            // check if the object is empty
            if (jsonObject.ObjectEmpty() || jsonObject.IsNull() )
            {   
                uiRet = 0;
            }
            else
            {   // check if has member 
                if (jsonObject.HasMember(strNodePath.c_str()) )
                {    
                    if (jsonObject[strNodePath.c_str()].IsArray())
                    {  
                        uiRet = jsonObject[strNodePath.c_str()].Size();
                    }
                    else
                    {
                        uiRet = 0;

                    } 
                }
                else
                {   // object not existing
                    uiRet = 0;
                } 
            }
        }
        else
        {// Node has children
            if (jsonObject.ObjectEmpty() || jsonObject.IsNull())
            {  
                uiRet = 0;
            }
            else
            {
                if (jsonObject.HasMember(strNodePath.c_str()))
                {
                    if (jsonObject[strNodePath.c_str()].IsArray())
                    {   // Get the object if exist
                        ::rapidjson::Value& childObject = jsonObject[strNodePath.c_str()][ulArrayElemnt];
                        uiRet = size(childObject, strChildren);
                    }
                    else
                    {   // Object not exists
                        ::rapidjson::Value& childObject = jsonObject[strNodePath.c_str()];
                        uiRet = size(childObject, strChildren);
                    }
                }
                else
                {
                    uiRet = 0;

                }  
            }
        }
    }

    // Processing result
    return uiRet;
}

/**
 * @brief processing of JSON object value based its path
 * 
 * @tparam T_VALUE 
 * @param jsonObject object of the json file
 * @param tValue Value to retrieve
 * @param strNode JSON node path
 * @param eType Type of the proccess
 * @return int rocessing result
 */
template<typename T_VALUE>
int rapidjson::process(::rapidjson::Value& jsonObject, T_VALUE& tValue, const std::string& strNode, const stProcess::TYPE& eType)
{
    int iRet = 0;
    bool bRet = true;
    // Split path into 'node' & 'children'
    std::string strNodePath;
    std::string strChildren;
    uint64_t ulArrayElemnt = 0;
    split(strNode, strNodePath, strChildren, ulArrayElemnt);
    // Check node name
    if(strNodePath.empty())
    {
        iRet = -1;
    }
    // 
    if(!iRet)
    {   
        ::rapidjson::Value memberName(strNodePath.c_str(), strNodePath.size(), m_allocator);
        // Process node
        if(strChildren.empty())
        {// Node has no childs (Terminal node)
            //check if it is an empty object or null object
            if(jsonObject.ObjectEmpty() || jsonObject.IsNull())
            {   
                iRet = -1;
            }
            else
            {
                if (jsonObject.HasMember(strNodePath.c_str()))
                {   // Remove member  
                    switch (eType)
                    {
                        case stProcess::TYPE::REMOVE:
                            if (ulArrayElemnt == IS_OBJECT)
                            {
                                bRet = jsonObject.RemoveMember(memberName);
                            }
                            else
                            {
                                bRet = jsonObject[ulArrayElemnt].RemoveMember(strNodePath.c_str());
                            }
                            if (bRet == false)
                            {   // Error in reming member
                                iRet = -1;
                            } 
                            break;
                        case stProcess::TYPE::GET:
                            iRet = getElement<T_VALUE>(tValue, jsonObject[strNodePath.c_str()], ulArrayElemnt);
                            break;
                        case stProcess::TYPE::EMPTY:
                            if (jsonObject[strNodePath.c_str()].IsNull())
                            {   // Error in reming member
                                iRet = -1;
                            } 
                            break;
                        case stProcess::TYPE::EXIST:
                            break;
                        default:
                            iRet = -1;
                            break;
                    } 
                }
                else
                {   // Has not member 
                    iRet = -1;
                } 
            }
        }
        else
        {// Node has children
            if(jsonObject.ObjectEmpty() || jsonObject.IsNull())
            {  
                iRet = -1; 
            }
            else
            {  
                if (jsonObject.HasMember(strNodePath.c_str()))
                { // Remove existing member object
                    if (ulArrayElemnt == IS_OBJECT)
                    {   
                        iRet = process(jsonObject[strNodePath.c_str()],tValue, strChildren, eType);
                    }
                    else
                    { 
                        iRet = process(jsonObject[strNodePath.c_str()][ulArrayElemnt], tValue, strChildren, eType);  
                    }
                }
                else
                {   // Not existing object to remove
                    iRet = -1;
                }  
            }
        }
    }

    // Processing result
    return iRet;
}

/**
 * @brief processing of JSON object value based its path
 * 
 * @param jsonObject object of the json file
 * @param strNode JSON node path
 * @return TYPE type of node
 */
rapidjson::TYPE rapidjson::process(::rapidjson::Value& jsonObject, const std::string& strNode)
{
    std::string strNodePath;
    std::string strChildren;
    uint64_t ulArrayElemnt = 0;
    split(strNode, strNodePath, strChildren, ulArrayElemnt);

    int iExist = 0;
    rapidjson::TYPE eType = rapidjson::TYPE::UKNOWN;

    if(strChildren.empty()  && !process(jsonObject,iExist,strNode, stProcess::TYPE::EXIST))
    {   
        if (ulArrayElemnt == IS_OBJECT)
        { 
            eType = type(jsonObject[strNodePath.c_str()]);
        }
        else
        {
            eType = type(jsonObject[strNodePath.c_str()][ulArrayElemnt]); 
        }  
    }
    else if (!strNodePath.empty() && !process(jsonObject,iExist,strNode, stProcess::TYPE::EXIST))
    { 
        if (ulArrayElemnt == IS_OBJECT)
        {   // Process if is object      
            eType = process(jsonObject[strNodePath.c_str()],strChildren);
        }
        else
        {   // Process if is array 
            eType = process(jsonObject[strNodePath.c_str()][ulArrayElemnt],strChildren);
        }   
    }
    // Return processing result (type of the node)
    return eType;
}

/**
 * @brief processing of JSON object value based its path
 * 
 * @param jsonObject object of the json file
 * @param strNode JSON node path
 * @return std::vector<std::string> list of elemnts
 */
std::vector<std::string> rapidjson::processList(::rapidjson::Value& jsonObject, const std::string& strNode)
{
    std::string strNodePath;
    std::string strChildren;
    uint64_t ulArrayElemnt = 0;
    split(strNode, strNodePath, strChildren, ulArrayElemnt);
    std::vector<std::string> vecElements;

    // Get the elemnts of the node if it is object and not empty
    if(strChildren.empty())
    {
        if (ulArrayElemnt == IS_OBJECT)
        {
            for (auto itr = jsonObject[strNodePath.c_str()].MemberBegin(); itr != jsonObject[strNodePath.c_str()].MemberEnd(); itr++)
            {
                if (itr->name.IsString())
                {  
                    vecElements.push_back(itr->name.GetString());
                }
            } 
        }
        else
        {
            if (jsonObject[strNodePath.c_str()][ulArrayElemnt].IsObject())
            {
                for (auto itr = jsonObject[strNodePath.c_str()][ulArrayElemnt].MemberBegin(); itr != jsonObject[strNodePath.c_str()][ulArrayElemnt].MemberEnd(); itr++)
                {
                    if (itr->name.IsString())
                    {  
                        vecElements.push_back(itr->name.GetString());
                    }
                } 
            }
        }    
    }
    else
    {
        if (ulArrayElemnt == IS_OBJECT)
        {   // Process if is object      
            vecElements = processList(jsonObject[strNodePath.c_str()], strChildren);
        }
        else
        {   // Process if is array 
            vecElements = processList(jsonObject[strNodePath.c_str()][ulArrayElemnt], strChildren);
        }
    }
    // Return processing result (list of the elments in this node)
    return vecElements;
}


/**
 * @brief set / add object with datatype
 * 
 * @tparam T_TYPEA 
 * @param memberName Json object member value
 * @param tType data type
 * @param jsonObject json array 
 * @param eType typr of process
 * @return int processing result
 */
template<typename T_TYPEA>
int rapidjson::castData(::rapidjson::Value& memberName, const T_TYPEA& tType, ::rapidjson::Value& jsonObject, const stProcess::TYPE& eType)
{
    int iRet = 0;

    if(std::is_same<T_TYPEA, std::string>::value)
    {   //set string
        const std::string& strValue = reinterpret_cast<const std::string&>(tType);
        ::rapidjson::Value memberValue(::rapidjson::kStringType);
        switch (eType)
        {
            case stProcess::TYPE::SET:
                jsonObject.SetString(strValue.c_str(),m_allocator);
                break;
            case stProcess::TYPE::PUSHBACK:
                memberValue.SetString(strValue.c_str(), m_allocator);
                jsonObject.PushBack(memberValue, m_childDocJsonFile.GetAllocator());
                break;
            case stProcess::TYPE::ADD:
                memberValue.SetString(strValue.c_str(), m_allocator);
                jsonObject.AddMember(memberName, memberValue, m_allocator);
                break;

            default:
                iRet = -1;
                break;
        }
    }
    else if(std::is_same<T_TYPEA, bool>::value)
    {   // set bool
        const bool& bValue = reinterpret_cast<const bool&>(tType);
        switch (eType)
        {
            case stProcess::TYPE::SET:
                jsonObject.SetBool(bValue);
                break;
            case stProcess::TYPE::PUSHBACK:
                jsonObject.PushBack(bValue, m_allocator);
                break;
            case stProcess::TYPE::ADD:
                jsonObject.AddMember(memberName, bValue, m_allocator);
                break;

            default:
                iRet = -1;
                break;
        }
    }
    else if(std::is_integral<T_TYPEA>::value)
    {// set int
        if(sizeof(T_TYPEA) <= 4)
        {// Up to 4 bytes
            if (std::is_same<T_TYPEA, int>::value)
            {
                const int& iValue = reinterpret_cast<const int&>(tType);
                iRet = setElement(memberName, iValue, jsonObject, eType, stProcess::TYPE::SETINT);
            }
            else if (std::is_same<T_TYPEA, int16_t>::value)
            {
                const int16_t& sValue = reinterpret_cast<const int16_t&>(tType);
                iRet = setElement(memberName, sValue, jsonObject, eType, stProcess::TYPE::SETINT);
            }
            else if (std::is_same<T_TYPEA, int8_t>::value)
            {
                const int8_t& cValue = reinterpret_cast<const int8_t&>(tType);
                iRet = setElement(memberName, cValue, jsonObject, eType, stProcess::TYPE::SETINT);
            }
            else if(std::is_same<T_TYPEA, uint32_t>::value)
            {// Unsigned
                const uint32_t& uiValue = reinterpret_cast<const uint32_t&>(tType);
                iRet = setElement(memberName, uiValue, jsonObject, eType, stProcess::TYPE::SETUINT);
            }
            else if(std::is_same<T_TYPEA, uint16_t>::value)
            {// Unsigned
                const uint16_t& usValue = reinterpret_cast<const uint16_t&>(tType);
                iRet = setElement(memberName, usValue, jsonObject, eType, stProcess::TYPE::SETUINT);
            }
            else if(std::is_same<T_TYPEA, uint8_t>::value)
            {// Unsigned
                const uint8_t& ucValue = reinterpret_cast<const uint8_t&>(tType);
                iRet = setElement(memberName, ucValue, jsonObject, eType, stProcess::TYPE::SETUINT);
            }
            else
            {
                iRet = -2;
            }
        }
        else
        {// Up to 8 bytes
            if(std::is_same<T_TYPEA, int64_t>::value)
            {// Signed
                const int64_t& lValue = reinterpret_cast<const int64_t&>(tType);
                iRet = setElement(memberName, lValue, jsonObject, eType, stProcess::TYPE::SETINT64);
            }
            else if (std::is_same<T_TYPEA, uint64_t>::value)
            {
                const uint64_t& ulValue = reinterpret_cast<const uint64_t&>(tType);
                iRet = setElement(memberName, ulValue, jsonObject, eType, stProcess::TYPE::SETUINT64);
            }
            else
            {
                iRet = -2;
            }
        }
    }
    else if(std::is_same<T_TYPEA, float>::value)
    {   // set float
        const float& fValue = reinterpret_cast<const float&>(tType);
        switch (eType)
        {
            case stProcess::TYPE::SET:
                jsonObject.SetFloat(fValue);
                break;
            case stProcess::TYPE::PUSHBACK:
                jsonObject.PushBack(fValue, m_allocator);
                break;
            case stProcess::TYPE::ADD:
                jsonObject.AddMember(memberName, fValue, m_allocator);
                break;

            default:
                iRet = -1;
                break;
        }
    }
    else if(std::is_same<T_TYPEA, double>::value)
    {   // set float
        const double& dValue = reinterpret_cast<const double&>(tType);
        switch (eType)
        {
            case stProcess::TYPE::SET:
                jsonObject.SetDouble(dValue);
                break;
            case stProcess::TYPE::PUSHBACK:
                jsonObject.PushBack(dValue, m_allocator);
                break;
            case stProcess::TYPE::ADD:
                jsonObject.AddMember(memberName, dValue, m_allocator);
                break;

            default:
                iRet = -1;
                break;
        }
    }
    else
    {   
        iRet = -1;
    }

    // Return processing result
    return iRet;
}

/**
 * @brief Get the Element object
 * 
 * @tparam T_VALUE 
 * @param tValue data value
 * @param jsonObject json array 
 * @param ulArrayElemnt the number of the array element
 * @return int processing result
 */
template<typename T_VALUE>
int rapidjson::getElement(T_VALUE& tValue, ::rapidjson::Value& jsonObject, const uint64_t& ulArrayElemnt)
{
    int iRet = 0;

    if (jsonObject.IsArray() && jsonObject.Size() > ulArrayElemnt)
    {   // Get elemnts if it is an array 
        iRet = element(jsonObject[ulArrayElemnt], tValue);
    }
    else if(jsonObject.IsObject())
    {
        iRet = -1;
    }
    else
    {
        // Get elemnts if it is a datatypa
        iRet = element(jsonObject, tValue);
    }
    // Return processing result
    return iRet;
}

/**
 * @brief Get the type of a specific node
 * 
 * @param jsonObject object of the json file
 * @return JSON_TYPE type of node
 */
rapidjson::TYPE rapidjson::type(::rapidjson::Value& jsonObject)
{
    rapidjson::TYPE eType = rapidjson::TYPE::UKNOWN;

    // Get the type of the node 
    switch (jsonObject.GetType())
    {
        case ::rapidjson::kArrayType:
            eType = rapidjson::TYPE::ARRAY;
            break;
        case ::rapidjson::kObjectType:
            eType = rapidjson::TYPE::OBJECT;
            break;
        case ::rapidjson::kTrueType:
        case ::rapidjson::kFalseType:
            eType = rapidjson::TYPE::BOOL;
            break;
        case ::rapidjson::kStringType:
            eType = rapidjson::TYPE::STRING;
            break;
        case ::rapidjson::kNumberType:
            if(jsonObject.IsInt())
            {
                eType = rapidjson::TYPE::SINT32;
            }
            else if (jsonObject.IsUint())
            {
                eType = rapidjson::TYPE::UINT32;
            }
            else if (jsonObject.IsInt64())
            {
                eType = rapidjson::TYPE::SINT64;
            }
            else if (jsonObject.IsUint64())
            {
                eType = rapidjson::TYPE::UINT64;
            }
            else if (jsonObject.IsFloat())
            {
                eType = rapidjson::TYPE::FLOAT;
            }
            else if (jsonObject.IsDouble())
            {
                eType = rapidjson::TYPE::DOUBLE;
            }
            
            break;
        
        default:
            break;
    }

    // Return processing result (node type)
    return eType;

}

/**
 * @brief Get the elemnt of a specific type
 * @tparam T_VALUE 
 * @param jsonObject object of the json file
 * @param tValue Value to retrieve
 * @return  int processing result
 */
template<typename T_VALUE>
int rapidjson::element(::rapidjson::Value& jsonObject, T_VALUE& tValue)
{
    int iRet = 0;
    // Get the type of the node in the json file
    TYPE eType = type(jsonObject);
    // Get elemnt depends on its data type
    switch (eType)
    {
        case rapidjson::TYPE::STRING:
          {
            if(std::is_same<T_VALUE, std::string>::value)
            {   // Get string element
                std::string& strValue = reinterpret_cast<std::string&>(tValue);   
                strValue = jsonObject.GetString();
            }
            else
            {
                iRet = -2;
            }
            break;
          }
        case rapidjson::TYPE::BOOL:
            if(std::is_same<T_VALUE, bool>::value)
            {   // Get bool element
                tValue = jsonObject.GetBool();
            }
            else
            {
                iRet = -2;
            }
            break;
        case rapidjson::TYPE::FLOAT:
            if (jsonObject.IsFloat() && std::is_same<T_VALUE, float>::value)
            {   // Get float element
                tValue = jsonObject.GetFloat();
            }
            else if (jsonObject.IsDouble() && std::is_same<T_VALUE, double>::value)
            {   // Get double element
                tValue = jsonObject.GetDouble();
            }
            else
            {
                iRet = -2;
            }
            break;
        case rapidjson::TYPE::SINT32:
            {
                int32_t  iValue = jsonObject.GetInt();
                uint32_t uiValue = 0;
                if(jsonObject.GetInt() >= 0)
                {
                    uiValue = jsonObject.GetUint();
                }
                if(std::is_same<T_VALUE, uint32_t>::value)
                {  
                    if((iValue >= 0) && (uiValue <= UINT32_MAX))
                    {   // Get uint23_t element
                        tValue = static_cast<uint32_t>(uiValue);
                    }
                    else
                    {
                        iRet = -2;
                    }   
                }
                else if (std::is_same<T_VALUE, uint16_t>::value)
                {   
                    if((iValue >= 0) && (uiValue <= UINT16_MAX))
                    {
                        // Get uint16 element
                        tValue = static_cast<uint16_t>(uiValue);
                    }
                    else
                    {
                        iRet = -2;
                    }   
                }
                else if (std::is_same<T_VALUE, uint8_t>::value)
                {   
                    if((iValue >= 0) && (uiValue <= UINT8_MAX))
                    {
                        // Get uint16 element
                        tValue = static_cast<uint8_t>(uiValue);
                    }
                    else
                    {
                        iRet = -2;
                    }   
                }
                else if(std::is_same<T_VALUE, int>::value)
                {  
                    if(iValue <= INT_MAX && iValue >= INT_MIN)
                    {
                        // Get int element
                        tValue = static_cast<int>(iValue);
                    }
                    else
                    {
                        iRet = -2;
                    }   
                }
                else if (std::is_same<T_VALUE, int8_t>::value)
                {   
                    if (iValue <= INT8_MAX && iValue >= INT8_MIN)
                    {
                        // Get int16_t element
                        tValue = static_cast<int8_t>(iValue);
                    }
                    else
                    {
                        iRet = -2;
                    }   
                }
                else if (std::is_same<T_VALUE, int16_t>::value)
                {   
                    if (iValue <= INT16_MAX && iValue >= INT16_MIN)
                    {
                        // Get int16_t element
                        tValue = static_cast<int16_t>(iValue);
                    }
                    else
                    {
                        iRet = -2;
                    }   
                }
                else if (std::is_same<T_VALUE, int32_t>::value)
                {  
                    if (iValue <= INT32_MAX && iValue >= INT32_MIN)
                    {
                        // Get int32_t element
                        tValue = static_cast<int32_t>(iValue);
                    }
                    else
                    {
                        iRet = -2;
                    }  
                }
                else if (std::is_same<T_VALUE, int64_t>::value)
                {   // Get int64 element
                    tValue = jsonObject.GetInt64();
                }
                else if (std::is_same<T_VALUE, uint64_t>::value)
                {   
                    if (jsonObject.GetInt64() >= 0)
                    {
                        // Get uint16 element
                        tValue = jsonObject.GetUint64();
                    }
                    else
                    {
                        iRet = -2;
                    }
                }
                else if (std::is_same<T_VALUE, float>::value)
                {   // Get float element
                    tValue = jsonObject.GetFloat();
                }
                else if (std::is_same<T_VALUE, double>::value)
                {   // Get double element
                    tValue = jsonObject.GetDouble();
                }
                else
                {
                    iRet = -2;
                }
                break;
            }
        case rapidjson::TYPE::SINT64:
            if (std::is_same<T_VALUE, int64_t>::value)
            {   // Get int64 element
                tValue = jsonObject.GetInt64();
            }
            else if (std::is_same<T_VALUE, uint64_t>::value)
            {   
                if (jsonObject.GetInt64() >= 0)
                {
                    // Get uint16 element
                    tValue = jsonObject.GetUint64();
                }
                else
                {
                    iRet = -2;
                }
            }
            else
            {
                iRet = -2;
            }
            break;
        case rapidjson::TYPE::UINT32:
           {
                uint32_t uiValue  = jsonObject.GetUint();
                if(std::is_same<T_VALUE, uint32_t>::value)
                {  
                    if(uiValue <= UINT32_MAX)
                    {   // Get uint23_t element
                        tValue = static_cast<uint32_t>(uiValue);
                    }
                    else
                    {
                        iRet = -2;
                    }   
                }
                else if (std::is_same<T_VALUE, uint16_t>::value)
                {   
                    if(uiValue <= UINT16_MAX)
                    {
                        // Get uint16 element
                        tValue = static_cast<uint16_t>(uiValue);
                    }
                    else
                    {
                        iRet = -2;
                    }   
                }
                else if (std::is_same<T_VALUE, uint8_t>::value)
                {   
                    if(uiValue <= UINT8_MAX)
                    {
                        // Get uint16 element
                        tValue = static_cast<uint8_t>(uiValue);
                    }
                    else
                    {
                        iRet = -2;
                    }   
                }
                break;
           }
        case rapidjson::TYPE::UINT64:
            tValue = jsonObject.GetUint64();
            break;
        case rapidjson::TYPE::DOUBLE:
            tValue = jsonObject.GetDouble();
            break;
        default:
            iRet = -1;
            break;
    }
    // Return processing result
    return iRet;
}

/**
 * @brief Set the Element object
 * 
 * @tparam T_VALUE 
 * @param tValue data value
 * @param jsonObject json array 
 * @param ulArrayElemnt the number of the array element
 * @param eType type of process
 * @return int processing result
 */
template<typename T_VALUE>
int rapidjson::setElement(::rapidjson::Value& memberName , const T_VALUE& tValue, ::rapidjson::Value& jsonObject, const rapidjson::stProcess::TYPE& eType, const stProcess::TYPE& eDataType)
{

    int iRet = 0;
    switch (eType)
    {
        case stProcess::TYPE::SET:
            if (eDataType == rapidjson::stProcess::TYPE::SETINT)
            {
                jsonObject.SetInt(tValue);
            }
            else if (eDataType == rapidjson::stProcess::TYPE::SETUINT)
            {
                jsonObject.SetUint(tValue);
            }
            else if (eDataType == rapidjson::stProcess::TYPE::SETINT64)
            {
                jsonObject.SetInt64(tValue);
            }
            else if (eDataType == rapidjson::stProcess::TYPE::SETUINT64)
            {
                jsonObject.SetUint64(tValue);
            }
            else
            {
                iRet = -2;
            }
            break;
        case stProcess::TYPE::PUSHBACK:
            jsonObject.PushBack(tValue, m_allocator);
            break;
        case stProcess::TYPE::ADD:
            jsonObject.AddMember(memberName, tValue, m_allocator);
            break;

        default:
            iRet = -1;
            break;
    }
    // Return processing result (node type)
    return iRet;  
}

/**
 * @brief Get the vector of elements 
 * 
 * @tparam T_VALUE 
 * @param vecValue data value
 * @param jsonObject json array 
 * @param ulArrayElemnt the number of the array element
 * @return int processing result
 */
template<typename T_VALUE>
int rapidjson::getVector(std::vector<T_VALUE>& vecValue, const std::string& strNodeFirst)
{
    int         iRet    = 0;
    uint32_t    uiSize  = size(strNodeFirst);
    std::string strPath = strNodeFirst;
    T_VALUE     tValue;

    // Loop over all items
    for(uint32_t uiCnt = 0; uiCnt < uiSize; ++uiCnt)
    {
        // Get item
        strPath = strNodeFirst + "[" + std::to_string(uiCnt) + "]";
        iRet = get(tValue, strPath);
        if(iRet)
        {
            break;
        }
        // Add item to the list
        vecValue.push_back(tValue);
    }

    // Return process
    return iRet;

}

/**
 * @brief Set the vector of elements 
 * 
 * @tparam T_VALUE 
 * @param vecValue data value
 * @param strNode path of the node
 * @return int processing result
 */
template<typename T_VALUE>
int rapidjson::setVector(const std::vector<T_VALUE>& vecValue, const std::string& strNode)
{
    int         iRet    = 0;
    uint32_t    uiSize  = vecValue.size();
    std::string strPath = strNode;

    // Loop over all items
    for(uint32_t uiCnt = 0; uiCnt < uiSize; ++uiCnt)
    {
        // Get item
        strPath = strNode + "[" + std::to_string(uiCnt) + "]";
        iRet = set(vecValue[uiCnt], strPath);
        if(iRet)
        {
            break;
        }
    }
    
    // Return process
    return iRet;
}

/**
 * @brief Split a given path
 * 
 * @param strPath original path
 * @param strNode node path
 * @param strChildren children
 * @param ulArrayElemnt array number
 * @return int processing result
 */
int rapidjson::split(const std::string& strPath, std::string& strNode, std::string& strChildren, uint64_t& ulArrayElemnt)
{
    std::pair<std::string, std::string> pairNode = string::split(strPath, m_cNodePathSeparator);
    const std::string& strName     = pairNode.first;
    strChildren = pairNode.second;

    std::pair<std::string, uint64_t> pairArray = string::array(strName, '[',']');
    strNode  = pairArray.first;
    ulArrayElemnt = pairArray.second;

    return 0;
}


} // namespace utilities
