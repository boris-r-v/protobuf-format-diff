#include "../json/json.hpp"
#include "../program_option.h"
#include "boost/program_options/parsers.hpp"

#include <iostream>
#include <fstream>
#include <format>

using nlohmann::json;
using namespace std;

void confirm(bool value, const string & what)
{
    if (value)
        cerr << "OK: " << what << endl;
    else
        throw std::runtime_error(what);
}

template<class T>
struct erase_guard{
    T& obj;
    erase_guard(T& o) :obj(o){}
    ~erase_guard(){ obj.erase();}
};

void verify( json & expected)
{
    using po_t = program_option::opt_value_type;
    auto cases = expected["cases"];
    confirm( cases.is_array(), "\"cases\" json-field is the array");
    program_option po;

    po.add( "-f1", "path to base proto file", po_t::param, po_t::mandatory );
    po.add( "-f2", "path to second proto file", po_t::param, po_t::mandatory );
    po.add( "-n", "if set will be using number of fields instad of name", po_t::allown );
    po.add( "-s", "message or enum name to compare with package name, if skipped - will be comparing whole proto file", po_t::param );
    po.add( "-i","path to protobuf include folder", po_t::param );

    for (auto& one: cases ){
        erase_guard<program_option> eg(po);
        std::string test_name = one["name"];
        std::cout <<"*****************  test_name: \""<< test_name << "\"  *****************"<< std::endl;
        confirm( one["map"].is_object(), "\"map\" json-field is the object");
        confirm( one.count( "cmd_line" ), "\"cmd_line\" is countaining run command");
        confirm( one["cmd_parse_result"].is_string(), "\"cmd_parse_result\" json-field is the string");


        string cmd_line = one["cmd_line"];
        auto parts = boost::program_options::split_unix(cmd_line);
        std::vector<char*> cstrings ;
        for(auto& str : parts){
            cstrings.push_back(const_cast<char*> (str.c_str()));
        }
        int argc = (int)cstrings.size();
        char** argv = cstrings.data();
        std::string cmd_parse_result = one["cmd_parse_result"];
        try{
            po.parse( argc, argv );
        }
        catch(std::exception const& e ){
                std::cerr << "Exception: " << e.what() << std::endl;
            confirm( cmd_parse_result == "fail" , std::format( "parse test cmd-line fail, but marked in test.json as <{}>", cmd_parse_result ) );
            continue;
        }
        confirm( cmd_parse_result == "success" , std::format( "parse test cmd-line success, but marked in test.json as <{}>", cmd_parse_result ));

        for (auto& [key, value] : one["map"].items() ){
            string val = po.get(key).value;
            confirm( val == value, std::format("param {}:'{}' equal to '{}'", string(key), val, string(value) ) );
        }
    }
}
int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        cerr << "Expected argument: <test directory> " << endl;
        return 1;
    }

    string test_path(argv[1]);

    json expected;
    try
    {
        string diff_path = test_path + "/test-op.json";
        ifstream diff_file(diff_path);
        if (!diff_file.is_open())
        {
            cerr << "Failed to open test-op.json file: " << diff_path << endl;
            return 1;
        }

        diff_file >> expected;
    }
    catch (json::exception & e)
    {
        cerr << "Failed to parse test-op.json file: " << e.what() << endl;
        return 1;
    }

    try
    {
        verify( expected );
    }
    catch (std::exception & e)
    {
        cerr << "Failed to verify: " << e.what() << endl;
        return 1;
    }

    cerr << "OK." << endl;
}
