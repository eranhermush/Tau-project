#include "file_manager.h"

file_manager::file_manager(const std::string& path, std::string& scheme_string1, std::vector<std::string> &file_names, 
        std::string &passwords, std::string &password_function, std::string &hash_args, uint64_t start, uint64_t finish, unsigned int size_of_job):
    //our_parser(parser),
    file_names(file_names),
    scheme_string(scheme_string1),
    passwords(passwords),
    password_function(password_function),
    hash_args(hash_args),

    file_char('f'),
    curr_id(1),
    sum_of_works(finish),
    current_index_of_work(start),
    start_index_work(start),
    work_size(size_of_job),
    dir_path(path)
    

{
    /*
    :our_parser(parser),file_names(file_names),compress_scheme_string(our_parser.get_str_compress()), file_char('f'), curr_id(1), sum_of_works(0), current_index_of_work(0), work_size(30), passwords(passwords),
        password_function(password_function), dir_path(path), hash_args(hash_args)
*/
    this->fileobj.intialize();
    //std::cout <<"f" << std::endl;
    update_file_object_no_index(this->fileobj);
    validate_input();
    //if(finish ==-1){
    //    save_sum_of_works();

    //}
}
std::string file_manager::get_files_in_string()
{
    std::string result = "";
    for (unsigned int i = 0; i< this->file_names.size();i++)
    {
        if (i != 0)
        {
            result = result + file_object::delimiter_of_files_in_fileobject_symbol;
        }
        result += this->file_names[i];
    }
    return result;

}
bool file_manager::validate_input()
{
    if(this->scheme_string.length() <=0)
    {
        throw std::invalid_argument("received an empty string");
    }
    int count_result = std::count(this->scheme_string.begin(), this->scheme_string.end(), this->file_char);
    int file_size = this->file_names.size();
    if(count_result != file_size)
    {
        throw std::invalid_argument("received different sizes for the file names and the number of files in the scheme");
    }
    return true;
}
void file_manager::save_sum_of_works()
{

    uint64_t result = 1;
    std::string filter_pattern = Pattern_Utils::filter_pattern(this->scheme_string);
    result *= Pattern_Utils::total_amount_by_char_tokens(filter_pattern);
    std::string full_path;
    for (unsigned int i = 0; i< this->file_names.size(); i++)
    {
        full_path = helpful_functions::get_absolute_path(this->file_names.at(i));
        result *=  helpful_functions::get_file_size(full_path);
    }
    this->sum_of_works = result;
    //std::cout << "Total Work = " << result << std::endl;
}

void file_manager::set_work_size(unsigned int size)
{
    this->work_size = size;
}
uint64_t file_manager::get_sum_of_works()
{
    return this->sum_of_works;
}



/*
**********************************************************
**********************************************************
******************   Work section   **********************
**********************************************************
**********************************************************
*/

int file_manager::get_id_to_file()
{
    this->curr_id++;
    return this->curr_id-1;
}
void file_manager::update_file_object_no_index(file_object& f)
{
    f.set_status(0);
    //file_obj.set_scheme_msg(this->scheme_string);
    f.set_scheme_msg(this->scheme_string);
    f.set_passwords(this->passwords);
    f.set_password_function(this->password_function);
    f.set_files_for_scheme(get_files_in_string());
    f.set_arguments(this->hash_args);
    //std::cout << f.to_string() <<std::endl<<"f: " << this->password_function<<std::endl;
}

int file_manager::create_new_work(file_object& file_obj)
{
    file_object file_obj_former;
    /*
    if(this->current_index_of_work % 1000000 < 100)
    {
        std::cout << "current index of work is " << current_index_of_work << " result is " <<this->sum_of_works <<  std::endl;
    }
    */
    uint64_t frac = (this->sum_of_works + 1 - this->start_index_work) / 5;
    uint64_t x1 = (this->current_index_of_work + 1 - this->start_index_work) / (frac);
    uint64_t x2 = (std::min(this->current_index_of_work + this->work_size, this->sum_of_works-1)+ 1 - this->start_index_work) / frac;
    if(x1 < x2)
    {
        x2 = x2*20;
        std::cout << "Searching approximately " << x2 << "\% of the passwords." << std::endl;
    }
    //std::cout << this->current_index_of_work << " " << this->sum_of_works <<std::endl;
    if (this->current_index_of_work >= this->sum_of_works-1)
    {
        //std::cout << "here this->current_index_of_work >= this->sum_of_works-1" << std::endl;
        return -1;
    }
    if (arr_didnt_do.empty())
    {
        file_obj.set_index(this->current_index_of_work, std::min(this->current_index_of_work + this->work_size, this->sum_of_works-1));
        this->current_index_of_work = std::min(this->current_index_of_work + this->work_size, this->sum_of_works-1);
    }
    else
    {
        file_obj_former = arr_didnt_do.back();
        arr_didnt_do.pop_back();
        file_obj.set_index(file_obj_former.get_start_index(), file_obj_former.get_end_index());
    }

    file_obj.set_id(get_id_to_file());
    update_file_object_no_index(file_obj);
    this->arr_of_works.push_back(file_obj);
    return 0;
}

int file_manager::write_work_to_file(file_object& file_obj)
{

    bool retVal = false;
    // write the data without the status (write status 2)
    int worker_id = file_obj.get_worker_id();

    std::string path =  dir_path + "/" + std::to_string(worker_id) + ".txt";
    retVal =  helpful_functions::change_status_of_file(path, file_object::working_in_process_symbol);
    if (! retVal) {
        return -1;
    }

    std::ofstream myfile;
    // write the data without the status (write status 2)
    //std::string path = std::to_string(worker_id) + ".txt";

    truncate(path.c_str(),2);

    myfile.open(path, std::fstream::out | std::fstream::app);
    
    if (! (myfile.is_open()))
    {
        std::cerr << "Error opening file" << std::endl;
        return false;
    }


    myfile << file_obj.get_message_to_write_in_file_without_status();
    myfile.flush();
    myfile.close();
    // write the data
    if (file_obj.get_status() == file_object::no_more_works_symbol)
    {
        retVal =  helpful_functions::change_status_of_file(path, file_object::no_more_works_symbol);
    }
    else
    {
        retVal =  helpful_functions::change_status_of_file(path, file_object::dont_start_working_symbol);
    }
    if (! retVal) {
        return -1;
    }
    return 0;
}


bool file_manager::check_validate_of_file(std::string file_name, std::string full_file_name, file_object& file_obj, bool print_error)
{
    file_object file_in_arr;
    int retVal = 0;
    bool find = false;
    int file_name_int = 0;



    try
    {
        // we also validate here that the filen-name consist of integers, we make substr to avoid the ".txt"
        file_name_int = std::stoi(file_name.substr(0, file_name.length() -4)); 
    }
    catch ( std::exception& e)
    {

        if (print_error)
        {
            std::cerr << "An exception with stoi of file names "  << e.what()  << '\n';
        }
        return false;
    }


    try
    {

        retVal = helpful_functions::file_to_file_object(file_obj, full_file_name, print_error);
        if (retVal == -1){
            return false;
        }
    }
    catch ( std::exception& e)
    {

        if (file_obj.get_status() == file_object::start_worker_symbol || file_obj.get_status() == file_object::no_more_works_symbol || file_obj.get_status() == file_object::worker_write_to_file_symbol || file_obj.get_status() == file_object::dont_start_working_symbol)
        {
            return true;
        }
        if (print_error)
        {
            std::cerr << "An exception occurred on open file. The exception is:"  << e.what() << std::endl;
            //std::cout << file_obj.to_string() << std::endl;
        }
        return false;
    }

    // start worker
    if (file_obj.get_status() == file_object::start_worker_symbol || file_obj.get_status() == file_object::no_more_works_symbol)
    {
        return true;
    }
    // checks the worker id 
    if  (file_obj.get_worker_id() != file_name_int) 
    {
        std::cerr << "get_worker_id differ then filename" << std::endl;
        return false;
    }
    for (unsigned int i = 0; i < this->arr_of_works.size(); i++) {
        if(this->arr_of_works.at(i).get_worker_id() == file_name_int)
        {
            file_in_arr = this->arr_of_works.at(i);
            find = true;
            break;
        }
    }
    // there is not a relevant file in the arr
    if (! find)
    {
        std::cerr << "There is a file in the directory that is not connect to a work in the server" << std::endl;
        return false;
    }
    // in this mode the file looks diferent, then we return here (the check equal will return false)
    if (file_obj.get_status() == file_object::worker_found_password_symbol)
    {
        return true;
    }
    // if the relevant file is not consistent with the real file
    if (! file_obj.check_equal(file_in_arr))
    {
        std::cerr << "the file object in server array is diferent than the file object in the directory" << std::endl;
        return false;
    }
    return true;
}


bool file_manager::finish_job()
{
    if (arr_didnt_do.size() > 0 || arr_of_works.size() > 0)
    {
        return false;
    }
    if (this->current_index_of_work >= this->sum_of_works-1)
    {
        std::cout << "The enumeration is over." << std::endl;
    }
    return this->current_index_of_work >= this->sum_of_works-1;
}


// Todo: add print_erorr to all functions
bool file_manager::go_over_files( bool print_error)
{

    std::vector<std::string> file_names;
    std::vector<std::string> passwords_founds;
    std::string file_name;
    file_object obj, new_obj;
    file_object file_in_arr;
    int retVal = 0;
    bool val = true;
    int file_name_int = 0;
    bool find_new = false;
    helpful_functions::read_directory(this->dir_path, file_names);
    for (unsigned int i = 0; i < file_names.size(); i++) {
        file_name = this->dir_path + "/" + file_names.at(i);
        try{
            file_name_int = std::stoi(file_name.substr(this->dir_path.length()+1, file_name.length() - 3));
        }

        catch (...)
        {
            val = false;
            file_name_int = -1;
        }
        if (val)
        {
            obj.intialize();
            val = check_validate_of_file(file_names.at(i), file_name, obj,  print_error);
        }


        if(! val)
        {
            // removes the files
            //std::cout << "remove a file, val not true " << std::endl;
            if( remove(file_name.c_str()) != 0 )
            //if( false )
            {
                if (print_error){
                    perror( "Error deleting file" );
                }
            }
            
            // updates the arrays
            for (int j = this->arr_of_works.size()-1; j >= 0 ; j--)
            {
                if(this->arr_of_works.at(j).get_worker_id() == file_name_int)
                {
                    file_in_arr = this->arr_of_works.at(j);
                    arr_didnt_do.push_back(file_in_arr);
                    this->arr_of_works.erase (this->arr_of_works.begin()+j);
                }
            }
        }
        else
        {   
            if(obj.get_status() == file_object::worker_found_password_symbol)
            {
                passwords_founds = obj.get_passwords_found_vector();
                if(passwords_founds.size() == 1){
                    std::cout << "A matching password was found:" << std::endl << "\t" << passwords_founds[0] << std::endl;
                }
                else if(passwords_founds.size() > 1){
                    std::cout << "Multiple passwords were found:" << std::endl;
                    for(unsigned int p = 0; p < passwords_founds.size() && p < 10; ++p){
                        std::cout << "\t" << passwords_founds[p] << std::endl;
                    }
                    if(passwords_founds.size() > 10){
                        std::cout << "And " << passwords_founds.size() - 10 << " more..." << std::endl;
                    }
                }
                //helpful_functions::printcoll(passwords_founds);
                find_new = true;
                //password_function.clear();
            }
            // if we finish a job, we remove it from the array
            if(obj.get_status() == file_object::finish_work_symbol ||  obj.get_status() == file_object::done_working_symbol || obj.get_status() == file_object::worker_found_password_symbol)
            {
                for (int j = this->arr_of_works.size()-1; j >= 0 ; j--)
                {
                    if(this->arr_of_works.at(j).get_worker_id() == file_name_int)
                    {
                        this->arr_of_works.erase(this->arr_of_works.begin()+j);
                    }
                }
                find_new = true;
            }
            // if we want a new job - we make it here
            if(obj.get_status() == file_object::start_worker_symbol ||  obj.get_status() == file_object::done_working_symbol || obj.get_status() == file_object::worker_found_password_symbol)
            {
                new_obj.intialize();
                new_obj.set_worker_id(obj.get_worker_id());
                retVal = create_new_work(new_obj);
                if(retVal == -1){
                    new_obj.intialize_to_finish_work();
                }
                retVal = write_work_to_file(new_obj);
                find_new = true;
            }            
        }
    }
    return ( find_new);
}


