/*
 * Copyright (c) 2016-20017 Max Cong <savagecm@qq.com>
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <iostream>
#include <vector>
#include <chrono>
#include <map>
#include <thread>
#include <string>
#include <tuple>

#include "jsonParser/jsonParser.hpp"
#include "logger/simple_logger.hpp"
#include "rapidjson/encodings.h"

using namespace std;
#define TEST_SCHEMA_FILE "/home/mcong/jsonParser/schema/test.json"
namespace testNameSpace
{

    class AccessTokenClaims : public jsonParser::jsonParser<AccessTokenClaims>
    {
    public:
        rapidjson::Document aud;

        explicit AccessTokenClaims() {}

        void staticjson_init(staticjson::ObjectHandler *h)
        {
            std::cout << "staticjson_init AccessTokenClaims is called" << std::endl;

            h->add_property("aud", &this->aud, staticjson::Flags::Default);
            h->set_flags(staticjson::Flags::Default | staticjson::Flags::DisallowUnknownKey);
        }
    };
} // namespace testNameSpace

int main()
{
    INIT_LOGGER(new simpleLogger());
    SET_LOG_LEVEL(debug);

    jsonParser::jsonParser<testNameSpace::AccessTokenClaims>::loadApiSchema(TEST_SCHEMA_FILE);

    // std::string testStr = R"({"AccessTokenClaims": [{"sst":"aa","sd":"aaa"},{"sst": "aa","sd":"abc"}],"test":"adc","testInt":226})";
    //std::string testStr = R"({"AccessTokenClaims": [],  "test":"adc","testInt":255})";
    std::string testStr = R"({"aud":["f81d4fae-7dec-11d0-a765-00a0c91e6bf6","f81d4fae-7dec-11d0-a765-00a0c91e6bf6"]})";

    staticjson::ParseStatus result;

    testNameSpace::AccessTokenClaims testObj;
    std::string report;
    auto ret = testObj.decode(testStr, result);
    std::cout << "validation report is :" << ret.second << std::endl;
    std::cout << "parse result: " << (int)ret.first << std::endl;
    std::cout << "parse result description: " << result.description() << std::endl;
    std::cout << "offset :" << result.offset() << std::endl;
    std::cout << "error code " << result.error_code() << std::endl;
    std::cout << "has error  " << result.has_error() << std::endl;

    //const staticjson::ErrorStack m_stack = result.error_stack();

    if (ret.first == jsonParser::jsonRet::SUCCESS)
    {
        //std::cout << "testUintKey is : " << testObj.mNssais[0] << std::endl;
    }

    if (testObj.aud.IsArray())
    {
        for (auto it = testObj.aud.Begin(); it != testObj.aud.End(); it++)
        {
            std::string outString(it->GetString(), it->GetStringLength());
            std::cout << "test! " << outString << std::endl;
        }
    }
}
