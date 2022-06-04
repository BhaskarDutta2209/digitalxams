#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/singleton.hpp>

using namespace std;
using namespace eosio;

CONTRACT digitalxams : public contract
{
public:
   using contract::contract;

   // Define the actions
   ACTION initiate();
   ACTION setperm(name main_admin, name admin_of_faculty, name admin_of_subjects, name admin_of_students, name admin_of_courses);
   ACTION enrollstudnt(string student_name, string college_name, string contact_info);
   ACTION addcourse(string course_name, string course_duration, string course_details, uint8_t no_of_sem);
   ACTION addsubject(string subject_name, string subject_details);
   ACTION addteacher(name teacher_account, string teacher_name, string contact_details);
   ACTION addmarks(uint64_t student_id, uint64_t course_id, uint64_t subject_id, uint64_t full_marks, uint64_t marks_obtained, string grade);

   // Define the constructor
   digitalxams(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                        _students(receiver, receiver.value),
                                                                        _courses(receiver, receiver.value),
                                                                        _subjects(receiver, receiver.value),
                                                                        _teachers(receiver, receiver.value),
                                                                        _marks(receiver, receiver.value),
                                                                        _permissions(receiver, receiver.value) {}

private:
   TABLE students
   {
      uint64_t student_id;
      string s_name;
      string college_name;
      string contact_info;

      uint64_t primary_key() const { return student_id; }
   };
   typedef multi_index<name("students"), students> students_table;
   students_table _students;

   TABLE courses
   {
      uint64_t course_id;
      uint64_t student_id;
      string course_name;
      string course_duration;
      string course_details;
      uint8_t no_of_sem;

      uint64_t primary_key() const { return course_id; }
   };
   typedef multi_index<name("courses"), courses> courses_table;
   courses_table _courses;

   TABLE subjects
   {
      uint64_t subject_id;
      uint64_t course_id;
      uint64_t teacher_id;
      string subject_name;
      string subject_details;

      uint64_t primary_key() const { return subject_id; }
   };
   typedef multi_index<name("subjects"), subjects> subjects_table;
   subjects_table _subjects;

   TABLE teachers
   {
      uint64_t teacher_id;
      name teacher_account;
      string teacher_name;
      string contact_details;

      uint64_t primary_key() const { return teacher_id; }
   };
   typedef multi_index<name("teachers"), teachers> teachers_table;
   teachers_table _teachers;

   TABLE marks
   {
      uint64_t marks_id;
      uint64_t subject_id;
      uint64_t student_id;
      uint64_t course_id;
      uint64_t full_marks;
      uint64_t marks_obtained;
      uint64_t semester_no;
      string grade;

      uint64_t primary_key() const { return marks_id; }
   };
   typedef multi_index<name("marks"), marks> marks_table;
   marks_table _marks;

   TABLE permissions
   {
      name main_admin;
      name admin_of_faculty;
      name admin_of_subjects;
      name admin_of_students;
      name admin_of_courses;
   };
   typedef singleton<name("permissions"), permissions> permissions_table;
   permissions_table _permissions;

   name get_teacher(uint64_t subject_id);
};