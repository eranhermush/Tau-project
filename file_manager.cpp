#include "file_manager.h"

file_manager::file_manager(std::string path, std::vector< std::vector<std::string> > &matrix_all_options, std::vector<std::string> &file_names, 
    std::string &scheme_string,std::string &passwords, std::string &password_function, std::string &hash_args)
{
    this->matrix_all_options = matrix_all_options;
    this->file_names = file_names;
    this->scheme_string = scheme_string;
    this->file_char = 'f';
    this->curr_id = 1;
    this->sum_of_works = 0;
    this->current_index_of_work = 0;
    this->work_size = 30;
    this->passwords = passwords;
    this->password_function = password_function;
    this->dir_path = path;
    this->hash_args =hash_args;
    validate_input();
    save_sum_of_works();
}
std::string file_manager::get_files_in_string()
{
    std::string result = "";
    for (int i = 0; i<this->file_names.size();i++)
    {
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
    if(this->scheme_string.length() != this->matrix_all_options.size())
    {
        throw std::invalid_argument("received different sizes for the matrix and the string");
    }
    if(std::count(this->scheme_string.begin(), this->scheme_string.end(), this->file_char) != this->file_names.size())
    {
        throw std::invalid_argument("received different sizes for the file names and the number of files in the scheme");
    }
    return true;
}
void file_manager::save_sum_of_works()
{
    if (this->scheme_string.length() <= 0)
    {
        this->sum_of_works = 0;
        return;
    }
    int result = 1;
    for (int i = 0; i<this->scheme_string.length();i++)
    {
        result *= size_of_object_in_scheme(i);
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
***************   This is the index Section **************
**********************************************************
**********************************************************
*/
int file_manager::vector_indexes_to_index(std::vector<int> &vec)
{
    int result = 0;
    int sum = 1;
    for (int i = 0; i<vec.size();i++)
    {
        result += sum*vec[i];
        sum *= size_of_object_in_scheme(i);
    }
    return result;
}


int file_manager::size_of_object_in_scheme(int index)
{
    if(index <0 || index >= this->scheme_string.length()){
        throw std::invalid_argument("unexpected index");
    }
    if(this->scheme_string.at(index) != file_char){
        return this->matrix_all_options[index].size();
    }
    int file_index = 0;
    for (int i = 0; i <= index; ++i)
    {
        if (this->scheme_string.at(i) == this->file_char)
        {
            file_index++;
        }
    }
    // we start to count from 0, and file_index counts how many files were(the len)
    file_index--;
    return get_number_of_lines_in_file(this->file_names[file_index]);
}
int file_manager::get_number_of_lines_in_file(std::string filename)
{
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile;
    myfile.open(filename);

    while (std::getline(myfile, line))
        ++number_of_lines;
    return number_of_lines;
}

std::vector<int> file_manager::index_to_vector_indexes(int index){
    std::vector<int> result; 

    // fill the array with 0 this->scheme_string.length() times 
    result.assign(this->scheme_string.length(), 0); 
    int sum = 1;
    for (int i = 0; i<this->scheme_string.length();i++){
        sum *= size_of_object_in_scheme(i);
    }

    for (int i = this->scheme_string.length()-1; i >= 0; i--)
    {
        sum = sum / size_of_object_in_scheme(i);
        result[i] = (int) index/sum;
        index -= result[i]*sum;
    }
    return result;
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
    file_obj.set_status(0);
    file_obj.set_scheme_msg(this->scheme_string);
    file_obj.set_passwords(this->passwords);
    file_obj.set_password_function(this->password_function);
    file_obj.set_files_for_scheme(get_files_in_string());
    file_obj.set_worker_id(worker_id);
    file_obj.set_arguments(this->hash_args);
    this->arr_of_works.push_back(file_obj);
    return 0;
}

int file_manager::write_work_to_file(file_object& file_obj)
{
    std::ofstream myfile;
    FILE *fp;
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

    fp = std::fopen(path.c_str(),"r+");
    fseek(fp, 0, SEEK_SET);
    if (fp == NULL) {
        perror("Error fopen in write_work_to_file ");
        return -1;
    }
    fprintf(fp, "0");
    std::fclose (fp);
    return 0;
}

int file_manager::file_to_file_object(file_object& file_obj, std::string filename, bool print_error)
{
    std::string line, line2;
    std::string msg;
    std::string files = "";
    std::ifstream myfile (filename);
    int status = 0;
    /*
        std::string result = std::to_string(this->id) + '\n' + std::to_string(this->worker_id) + '\n' + this->scheme_msg+ '\n' + this->password_function + 
        '\n' + std::to_string(this->start_index) + '\n' + std::to_string(this->end_index)+ '\n' + this->files_for_scheme + '\n' + this->passwords;
    */
    if (myfile.is_open())
    {
        getline (myfile,line);
        file_obj.set_status(std::stoi(line));
        getline (myfile,line);

        file_obj.set_id(std::stoi(line));
        getline (myfile,line);

        file_obj.set_worker_id(std::stoi(line));
        getline (myfile,line);
        file_obj.set_scheme_msg(line);
        msg = line;
        getline (myfile,line);
        file_obj.set_password_function(line);
        getline (myfile,line);
        getline (myfile,line2);

        file_obj.set_index(std::stoi(line),std::stoi(line2));

        for (int i = 1; i <= std::count(msg.begin(), msg.end(),'f'); i++)
        {
            getline (myfile,line);
            files = files + line + '\n'; 
        }
        file_obj.set_files_for_scheme(files);
        getline (myfile,line);
        file_obj.set_passwords(line);
        getline (myfile,line);
        file_obj.set_arguments(line);

        myfile.close();
    }
    else
    {
        if (print_error)
        {
            std::cout << "Error my file is not open :( file name is " << filename << std::endl;
        }
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

        retVal = file_to_file_object(file_obj, full_file_name, print_error);
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
            // if we finish a job, we remove it from the array
            if(obj.get_status() == 4 ||  obj.get_status() == 2)
            {
                for (int j = this->arr_of_works.size()-1; j >= 0 ; j--)
                {
                    if(this->arr_of_works.at(j).get_worker_id() == file_name_int)
                    {
                        this->arr_of_works.erase (this->arr_of_works.begin()+j);
                    }
                }
            }
            // if we want a new job - we make it here
            if(obj.get_status() == 3 ||  obj.get_status() == 2)
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


