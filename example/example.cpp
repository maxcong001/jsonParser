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
#define TEST_SCHEMA_FILE "/home/mcong/static_json/schema/test.json"
namespace testNameSpace
{
    class test : public jsonParser::jsonParser<test>
    {
    public:
        uint32_t testUint;
        std::string testString;

        explicit test() : testUint(), testString() {}

        void staticjson_init(staticjson::ObjectHandler *h)
        {
            h->add_property("testUintKey", &this->testUint, staticjson::Flags::Default);
            h->add_property("testStringKey", &this->testString, staticjson::Flags::Optional);
            h->set_flags(staticjson::Flags::Default | staticjson::Flags::DisallowUnknownKey);
        }
    };
} // namespace testNameSpace

int main()
{
    std::unique_ptr<simpleLogger> simpleLoggerUptr(new simpleLogger());
    INIT_LOGGER(simpleLoggerUptr);
    SET_LOG_LEVEL(debug);

    jsonParser::jsonParser<testNameSpace::test>::loadApiSchema(TEST_SCHEMA_FILE);

    std::string testStr = R"({"testUintKey":10,"testStringKey":"teststringvalue"})";

    staticjson::ParseStatus result;

    testNameSpace::test testObj;
    testObj.decode(testStr, result);

    std::cout<<"testUintKey is : "<<testObj.testUint<<", testStringKey is : "<<testObj.testString<<std::endl;
}
