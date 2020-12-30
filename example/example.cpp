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

using namespace std;
#define TEST_SCHEMA_FILE "/home/mcong/jsonParser/jsonParser/schema/test.json"
namespace testNameSpace
{
    class UCM_NudmUecmSingleNssai : public jsonParser::jsonParser<UCM_NudmUecmSingleNssai>
    {
    public:
        std::string mSst;
        std::string mSd;

        explicit UCM_NudmUecmSingleNssai() : mSst(),
                                             mSd()

        {
        }

        void staticjson_init(staticjson::ObjectHandler *h)
        {
            h->add_property("sst", &this->mSst, staticjson::Flags::Default);
            h->add_property("sd", &this->mSd, staticjson::Flags::Default);

            h->set_flags(staticjson::Flags::Default);
        }

        friend std::ostream &operator<<(std::ostream &o, const UCM_NudmUecmSingleNssai &obj)
        {
            o << "sst:" << obj.mSst << std::endl;
            o << "sd:" << obj.mSd << std::endl;

            return o;
        }
    };

    class Nssais : public jsonParser::jsonParser<Nssais>
    {
    public:
        std::vector<UCM_NudmUecmSingleNssai> mNssais;
        std::string mTest;
        int mIntTest;
        explicit Nssais() : mNssais() {}

        void staticjson_init(staticjson::ObjectHandler *h)
        {
            h->add_property("Nssais", &this->mNssais, staticjson::Flags::Default);
            h->add_property("test", &this->mTest, staticjson::Flags::Default);
            h->add_property("testInt", &this->mIntTest, staticjson::Flags::Default);
            h->set_flags(staticjson::Flags::Default | staticjson::Flags::DisallowUnknownKey);
        }
    };
} // namespace testNameSpace

int main()
{
    std::unique_ptr<simpleLogger> simpleLoggerUptr(new simpleLogger());
    INIT_LOGGER(simpleLoggerUptr);
    SET_LOG_LEVEL(debug);

    jsonParser::jsonParser<testNameSpace::Nssais>::loadApiSchema(TEST_SCHEMA_FILE);

    // std::string testStr = R"({"Nssais": [{"sst":"aa","sd":"aaa"},{"sst": "aa","sd":"abc"}],  "test":"adc","testInt":256})";
    std::string testStr = R"({"Nssais": [],  "test":"adc","testInt":255})";

    staticjson::ParseStatus result;

    testNameSpace::Nssais testObj;
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
}
