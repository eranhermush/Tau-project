#include "file_manager.h"

file_manager::file_manager(const std::string& path, parser_main &parser, std::vector<std::string> &file_names, std::string &passwords, std::string &password_function, std::string &hash_args):
    our_parser(parser),
    file_names(file_names),
    compress_scheme_string(parser.get_str_compress()),
    file_char('f'),
    curr_id(1),
    sum_of_works(0),
    current_index_of_work(0),
    work_size(30),
    passwords(passwords),
    password_function(password_function),
    dir_path(path),
    hash_args(hash_args)

{
    /*
    :our_parser(parser),file_names(file_names),compress_scheme_string(our_parser.get_str_compress()), file_char('f'), curr_id(1), sum_of_works(0), current_index_of_work(0), work_size(30), passwords(passwords),
        password_function(password_function), dir_path(path), hash_args(hash_args)
*/
    this->fileobj.intialize();
    update_file_object_no_index(this->fileobj);
    index_vec_con.intialize(this->fileobj);
    validate_input();
    save_sum_of_works();
}
std::string file_manager::get_files_in_string()
{
    std::string result = "";
    for (int i = 0; i< this->file_names.size();i++)
    {
        if (i != 0)
        {
            result = result + "#";
        }
        result += this->file_names[i];
    }
    return result;

}
bool file_manager::validate_input()
{
    if(this->compress_scheme_string.length() <=0)
    {
        throw std::invalid_argument("received an empty string");
    }
    if(std::count(this->compress_scheme_string.begin(), this->compress_scheme_string.end(), this->file_char) != this->file_names.size())
    {
        throw std::invalid_argument("received different sizes for the file names and the number of files in the scheme");
    }
    return true;
}
void file_manager::save_sum_of_works()
{
    if (this->compress_scheme_string.length() <= 0)
    {
        this->sum_of_works = 0;
        return;
    }
    int result = 1;
    for (int i = 0; i<this->compress_scheme_string.length();i++)
    {
        result *= index_vec_con.size_of_object_in_scheme(i);
    }
    this->sum_of_works = result;
}

void file_manager::set_work_size(int size)
{
    this->work_size = size;
}
int file_manager::get_sum_of_works()
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
    f.set_scheme_msg(this->our_parser.get_str_original());
    f.set_passwords(this->passwords);
    f.set_password_function(this->password_function);
    f.set_files_for_scheme(get_files_in_string());
    f.set_worker_id(worker_id);
    f.set_arguments(this->hash_args);
}

int file_manager::create_new_work(file_object& file_obj, int worker_id)
{
    file_object file_obj_former;
    if (this->current_index_of_work >= this->sum_of_works)
    {
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
    std::ofstream myfile;
    FILE *fp;
    bool retVal = false;
    // write the data without the status (write status 2)
    int worker_id = file_obj.get_worker_id();
    std::string path =  dir_path + "/" + std::to_string(worker_id) + ".txt";
    //std::string path = std::to_string(worker_id) + ".txt";
    myfile.open(path,std::fstream::in | std::fstream::out | std::fstream::trunc);
    if (! (myfile.is_open()))
    {
        std::cout << "Error opening file in write_work_to_file" << std::endl;
        return -1;
    }
    myfile << "1\n";
    myfile << file_obj.get_message_to_write_in_file_without_status();
    myfile.flush();
    myfile.close();
    // write the data
    retVal =  helpful_functions::change_status_of_file(path, 0);
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

        retVal = helpful_functions::file_to_file_object(file_obj, full_file_name, print_error);
        if (retVal == -1){
            return false;
        }
    }
    catch ( ...)
    {

        if (file_obj.get_status() == 3)
        {
            return true;
        }
        if (print_error)
        {
            std::cout << "An exception occurred on open file in validate. Exception Nr. "   <<'\n';
        }
        return false;
    }

    // start worker
    if (file_obj.get_status() == 3)
    {
        return true;
    }
    // checks the worker id 
    if  (file_obj.get_worker_id() != file_name_int) 
    {
        return false;
    }
    for (int i = 0; i < this->arr_of_works.size(); i++) {
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
        return false;
    }
    // in this mode the file looks diferent, then we return here (the check equal will return false)
    if (file_obj.get_status() == 6)
    {
        return true;
    }
    // if the relevant file is not consistent with the real file
    if (! file_obj.check_equal(file_in_arr))
    {
        return false;
    }
    return true;
}

// Todo: add print_erorr to all functions
void file_manager::go_over_files( bool print_error)
{

    std::vector<std::string> file_names;
    std::vector<std::string> passwords_founds;
    std::string file_name;
    file_object obj, new_obj;
    file_object file_in_arr;
    int retVal = 0;
    bool val = true;
    int file_name_int = 0;
    helpful_functions::read_directory(this->dir_path, file_names);
    for (int i = 0; i < file_names.size(); i++) {
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
            std::cout << "remove a file, val not true " << std::endl;
            if( remove(file_name.c_str()) != 0 )
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
            if(obj.get_status() == 6)
            {
                passwords_founds = obj.get_passwords_found_vector();
                std::cout << "worker " << std::to_string(obj.get_worker_id()) << " found passwords!!!!!" << std::endl;
                helpful_functions::printcoll(passwords_founds);
                password_function.clear();
            }
            // if we finish a job, we remove it from the array
            if(obj.get_status() == 4 ||  obj.get_status() == 2 || obj.get_status() == 6)
            {
                for (int j = this->arr_of_works.size()-1; j >= 0 ; j--)
                {
                    if(this->arr_of_works.at(j).get_worker_id() == file_name_int)
                    {
                        this->arr_of_works.erase(this->arr_of_works.begin()+j);
                    }
                }
            }
            // if we want a new job - we make it here
            if(obj.get_status() == 3 ||  obj.get_status() == 2 || obj.get_status() == 6)
            {
                new_obj.intialize();
                retVal = create_new_work(new_obj, file_name_int);
                if(retVal == -1){
                    new_obj.intialize_to_error();
                }
                retVal = write_work_to_file(new_obj);
                if(retVal == -1)
                {
                    if(print_error)
                    {
                        std::cout << "an error in write_work_to_file id =  " << file_name_int << std::endl;
                    }
                }
                
            }            
        }
    }
}


