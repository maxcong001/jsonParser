#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include "staticjson/document.hpp"
#include "staticjson/staticjson.hpp"

#include "rapidjson/schema.h"
#include "rapidjson/prettywriter.h"
#include "logger/logger.hpp"
namespace jsonParser
{
    enum class jsonRet
    {
        SUCCESS,
        FAIL,
    };
    template <typename T>
    class jsonParser
    {
    public:
        explicit jsonParser() {}

        ~jsonParser()
        {
        }

        static void readJsonFile(const std::string &filename, std::string &jsonBuffer)
        {
            std::ifstream infile(filename);
            getline(infile, jsonBuffer, std::string::traits_type::to_char_type(std::string::traits_type::eof()));
        }

        static void loadApiSchema(const std::string &jsonFile)
        {
            std::string jsonBuffer;
            staticjson::ParseStatus result;
            readJsonFile(jsonFile.c_str(), jsonBuffer);
            if (jsonRet::FAIL == loadSchemaValidator(jsonBuffer, result))
            {
                std::string errMsg = "Not able to load schema: " + jsonFile;
                throw std::logic_error(errMsg);
            }
        }

        void encode(std::string &jsonBuffer)
        {
            auto derived = static_cast<T *>(this);
            jsonBuffer = staticjson::to_json_string(*derived);
        }

        static jsonRet loadSchemaValidator(const std::string &jsonSchema, staticjson::ParseStatus &result)
        {
            rapidjson::Document tempDoc;
            if (!staticjson::from_json_string(jsonSchema.c_str(), &tempDoc, &result))
            {
                return jsonRet::FAIL;
            }
            schemaDocument = new (rapidjson::SchemaDocument)(tempDoc);
            schemaValidator = new (rapidjson::SchemaValidator)(*schemaDocument);
            return jsonRet::SUCCESS;
        }

        static void deleteSchemaValidator()
        {
            if (nullptr != schemaValidator)
            {
                delete schemaValidator;
                schemaValidator = nullptr;
            }
            if (nullptr != schemaDocument)
            {
                delete schemaDocument;
                schemaDocument = nullptr;
            }
        }

        template <typename Object>
        std::pair<jsonRet, std::string> decode(Object &obj,
                                               const std::string &jsonBuffer,
                                               staticjson::ParseStatus &result, std::string validatorReport = std::string(""))
        {
            auto ret = std::make_pair(jsonRet::FAIL, "");
            if (NULL == schemaValidator)
            {
                if (!staticjson::from_json_string(jsonBuffer.c_str(), &obj, &result))
                {
                    __LOG(error, "no validation schema, validation fail");
                    return ret;
                }
                return std::make_pair(jsonRet::SUCCESS, "");
            }
            rapidjson::Document d;
            schemaValidator->Reset();
            if (!staticjson::from_json_string(jsonBuffer.c_str(), &d, &result))
            {
                __LOG(error, "with validation schema, Json string validation fail");
                return ret;
            }
            if (!d.Accept(*schemaValidator))
            {
                rapidjson::StringBuffer sb;
                sb.Clear();
                rapidjson::PrettyWriter<rapidjson::StringBuffer> w(sb);
                schemaValidator->GetError().Accept(w);
                std::string validReport = sb.GetString();
                __LOG(info, "validation report is : " << validReport);
                return std::make_pair(jsonRet::FAIL, validReport);
            }
            if (!staticjson::from_json_document(d, &obj, &result))
            {
                __LOG(error, "with validation schema, document validation fail");
                return ret;
            }
            return std::make_pair(jsonRet::SUCCESS, "");
        }

        std::pair<jsonRet, std::string> decode(const std::string &jsonBuffer,
                                               staticjson::ParseStatus &result)
        {
            auto derived = static_cast<T *>(this);
            return decode<T>(*derived, jsonBuffer, result);
        }

    private:
        static rapidjson::SchemaValidator *schemaValidator;
        static rapidjson::SchemaDocument *schemaDocument;
    };

    template <typename T>
    rapidjson::SchemaValidator *jsonParser<T>::schemaValidator = NULL;
    template <typename T>
    rapidjson::SchemaDocument *jsonParser<T>::schemaDocument = NULL;

} // namespace jsonParser
