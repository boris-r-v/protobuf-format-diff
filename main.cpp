#include <comparison.h>
#include <iostream>
#include <filesystem>
#include <program_option.h>

using namespace std;
namespace fs = std::filesystem;
string proto_include;

int main(int argc, char * argv[])
{
    program_option po;
    using po_t = program_option::opt_value_type;

    po.add( "-f1", "path to base proto file", po_t::param, po_t::mandatory );
    po.add( "-f2", "path to comparing proto file", po_t::param, po_t::mandatory );
    po.add( "-n", "use number of fields to fetch it from protobuf scheme, instead of field name", po_t::allown );
    po.add( "-s", "message or enum name to compare with package name, if skipped - will be comparing whole proto file", po_t::param );
    po.add( "-i", "path to protobuf include folder", po_t::param );
    po.add( "-h", "print usage help and exit", po_t::allown );
    po.add( "-o", "show program option set before run", po_t::allown );

    try
    {
        po.parse(argc, argv);
        if ( po.get("-h").is_set() )
        {
            cerr << "Usage: " << argv[0] <<" -f1 path2file1 -f2 path2file2 [-s ...] [-i ...] [-n] " << endl;
            po.help( cerr );
            cerr << "E.g.:" << argv[0] <<" -f1 ../proto/CLS1.proto -f2 ../proto/CLS.proto -i /data/tools/protoc/include -s cls_gen.CounterAttributes " << endl;
            return 0;
        }
        po.check();
        if ( po.get("-o").is_set() )
        {
            po.show(cout);
        }
    }
    catch(std::exception const& e)
    {
        cerr << "Error: " << e.what() << endl;
        cerr << "Usage: " <<  argv[0] <<" -f1 path2file1 -f2 path2file2 [-s ...] [-i ...] [-n] " << endl;
        po.help( cerr );
        cerr << "E.g.:" << argv[0] <<" -f1 ../proto/CLS1.proto -f2 ../proto/CLS.proto -i /data/tools/protoc/include -s cls_gen.CounterAttributes " << endl;
        return 1;
    }

   
    Comparison::Options options;
    options.binary = po.get("-n").is_set();
    Comparison comparison(options);
    proto_include = po.get("-i").value;    
   
    try
    {
        fs::path file1(po.get("-f1").value);
        fs::path file2(po.get("-f2").value);
        string message_name = po.get("-s").is_set() ? po.get("-s").value : ".";

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

