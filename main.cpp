#include <comparison.h>
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char * argv[])
{
    if (argc < 4)
    {
        cerr << endl << "Usage: "<< argv[0] << " path2file1 path2file2 type [--binary]" << endl;
        cerr << "\tUse '.' for <type> to compare all messages and enums in given files." << endl;
        cerr << "\tUse '--binary' to search field by number not by name" << endl;
        cerr << "\ttype is protobuf message or enum name with package if it exists: e.g. cls_gen.CounterInfo not CounterInfo" << endl;
        cerr << "\tForked from: https://github.com/jleben/protobuf-format-diff" << endl;        
        return 0;
    }

    Comparison::Options options;

    if (argc > 4)
    {
        for (int i = 4; i < argc; ++i)
        {
            string arg = argv[i];
            if (arg == "--binary")
            {
                options.binary = true;
            }
            else
            {
                cerr << "Unknown option: " << arg << endl;
                return 1;
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
        return 1;
    }

    comparison.root.trim();
    comparison.root.print();

    return 0;
}

