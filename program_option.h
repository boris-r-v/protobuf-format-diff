#include <map>
#include <string>
#include <format>
#include <iostream>


class program_option{
    public:
        enum class opt_value_type{skip=0, allown=1, param=2, mandatory=3};
        struct opt_value_t{
            std::string value, description;
            opt_value_type type;
            opt_value_type mtype;
            inline bool is_set() const {return false == value.empty();}
        };
        program_option()=default;
        ~program_option()=default;

        program_option& add(std::string const& key, std::string const& descr, opt_value_type type, opt_value_type mtype = opt_value_type::skip ){
            opt_.insert( { key, {"", descr, type, mtype} } );
            return *this;
        }

        void parse(int argc, char * argv[]){
            for( int i=1; i < argc; ++i ){
                if ( auto fnd = opt_.find(argv[i]); fnd != opt_.end() ){
                    if ( fnd->second.type == opt_value_type::param and (i+1) < argc ){
                        fnd->second.value = argv[++i];
                        if ( opt_.find(fnd->second.value) != opt_.end() ){
                            throw std::runtime_error(std::format( "Missing argument for key '{}' in command line string, read value '{}'", fnd->first, fnd->second.value ) );
                        }
                    }
                    if (fnd->second.type == opt_value_type::allown){
                        fnd->second.value = "set";
                    }
                }
                else{
                    throw std::runtime_error(std::format( "Unknown param '{}' in command line", argv[i]) );
                }
            }
        }
        template<class T>
        void show(T& ss ){
            ss << " Command line params: \n";
            for (auto& it: opt_){
                ss << "\t" << it.first << " : " << it.second.value << "\n";
            }
            ss << " ------------------- \n";
        }
        void check(){
            for (auto& it: opt_){
                if ( opt_value_type::mandatory == it.second.mtype and !it.second.is_set() ){
                    throw std::runtime_error(std::format( "Skipped mandatory cmd-line param '{}'", it.first ) );
                }
            }
        }
        auto const& get( std::string const& key){
            return opt_.at(key);
        }
        void erase(){
            for (auto& it: opt_ ){
                it.second.value="";
            }
        }
        template <class T>
        void help(T& ss){
            for (auto& it: opt_ ){
                ss << "\t" << it.first << " : " << it.second.description << "\n";
            }
        }
    private:
        std::map<std::string, opt_value_t> opt_;

};