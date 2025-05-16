#include <comparison.h>
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;
string proto_include;
int main(int argc, char * argv[])
{
    if (argc < 4)
    {
        cerr << endl << "Usage: "<< argv[0] << " path2file1 path2file2 type [use_field_number] [proto_include]" << endl;
        cerr << "\t - <type> is protobuf message or enum name, with package (if it exists): e.g.: cls_gen.CounterInfo where <cls_gen> is package name" << endl;
        cerr << "\t - use '.' for <type> to compare all messages and enums in given files." << endl;
        cerr << "\t - use 'use_field_number' to search field by number not by name" << endl;
        cerr << "\t - use 'proto_include' to set protobuf include path with common files (e.g.: descriptor.proto) e.g.: /data/geo/cls-libs-docker/libs/include" << endl;
        cerr << "\t - forked from: https://github.com/jleben/protobuf-format-diff" << endl;        

        cerr << "E.g.: ./protobuf-spec-compare ../proto/CLS.proto ../proto/CLS1.proto cls_gen.CounterAttributes /data/geo/cls-libs-docker/libs/include" << endl << endl;        
        return 0;
    }

    Comparison::Options options;

    if (argc > 4)
    {
        for (int i = 4; i < argc; ++i)
        {
            string arg = argv[i];
            if (arg == "use_field_number")
            {   
                options.binary = true;
            }
            else
            {
                proto_include = arg;
            }
        }
    }

    Comparison comparison(options);
   
    try
    {
        fs::path file1(argv[1]);
        fs::path file2(argv[2]);
        string message_name = argv[3];

        Source source1( file1.filename(), file1.parent_path() );
        Source source2( file2.filename(), file2.parent_path() );
        if (message_name == ".")
            comparison.compare(source1, source2);
        else
            comparison.compare(source1, message_name, source2, message_name);
    }
    catch(std::exception & e)
    {
        cerr << e.what() << endl;
        return 2;
    }

    comparison.root.trim();
    std::stringstream ss;
    comparison.root.print(ss);
    cout << ss.str();

    return ss.str().empty() ? 0 : 1;
}

