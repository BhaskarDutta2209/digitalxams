#include <digitalxams.hpp>

name digitalxams::get_teacher(uint64_t subject_id) {
    auto subjects_itr = _subjects.find(subject_id);
    check(subjects_itr != _subjects.end(), "Subject not found");
    auto teacher_itr = _teachers.find(subjects_itr->teacher_id);
    check(teacher_itr != _teachers.end(), "Teacher not found");
    return teacher_itr->teacher_account;
}

ACTION digitalxams::initiate()
{
   require_auth(_self);
   _permissions.set(permissions{get_self(), get_self(), get_self(), get_self(), get_self()}, get_self());
}

ACTION digitalxams::setperm(name main_admin, name admin_of_faculty, name admin_of_subjects, name admin_of_students, name admin_of_courses) {
    auto permissions_table = _permissions.get_or_create(get_self(), permissions{
        get_self(),
        get_self(),
        get_self(),
        get_self(),
        get_self()
    });

    require_auth(permissions_table.main_admin);

    permissions_table.main_admin = main_admin;
    permissions_table.admin_of_faculty = admin_of_faculty;
    permissions_table.admin_of_subjects = admin_of_subjects;
    permissions_table.admin_of_students = admin_of_students;
    permissions_table.admin_of_courses = admin_of_courses;

    _permissions.set(permissions_table, get_self());
}

ACTION digitalxams::enrollstudnt(string student_name, string college_name, string contact_info) {
    // Check if the user is authorized to perform this action
    require_auth(_permissions.get().admin_of_students);

    // Add the student
    _students.emplace(get_self(), [&](auto &student) {
        student.student_id = _students.available_primary_key();
        student.s_name = student_name;
        student.college_name = college_name;
        student.contact_info = contact_info;
    });
}

ACTION digitalxams::addcourse(string course_name, string course_duration, string course_details, uint8_t no_of_sem) {
    // Check if the user is authorized to perform this action
    require_auth(_permissions.get().admin_of_courses);

    // Add the course
    _courses.emplace(get_self(), [&](auto &course) {
        course.course_id = _courses.available_primary_key();
        course.course_name = course_name;
        course.course_duration = course_duration;
        course.course_details = course_details;
        course.no_of_sem = no_of_sem;
    });
}

ACTION digitalxams::addsubject(string subject_name, string subject_details) {
    // Check if the user is authorized to perform this action
    require_auth(_permissions.get().admin_of_subjects);

    // Add the subject
    _subjects.emplace(get_self(), [&](auto &subject) {
        subject.subject_id = _subjects.available_primary_key();
        subject.subject_name = subject_name;
        subject.subject_details = subject_details;
    });
}

ACTION digitalxams::addteacher(name teacher_account, string teacher_name, string contact_details) {
    // Check if the user is authorized to perform this action
    require_auth(_permissions.get().admin_of_faculty);

    // Add the teacher
    _teachers.emplace(get_self(), [&](auto &teacher) {
        teacher.teacher_id = _teachers.available_primary_key();
        teacher.teacher_account = teacher_account;
        teacher.teacher_name = teacher_name;
        teacher.contact_details = contact_details;
    });
}

ACTION digitalxams::addmarks(uint64_t student_id, uint64_t course_id, uint64_t subject_id, uint64_t full_marks, uint64_t marks_obtained, string grade) {
    // Check if the correct teacher is assigning marks
    require_auth(get_teacher(subject_id));

    // Check if the student is enrolled in the course
    auto student_itr = _students.find(student_id);
    check(student_itr != _students.end(), "Student not found");

    // Check if the course is offered
    auto course_itr = _courses.find(course_id);
    check(course_itr != _courses.end(), "Course not found");

    // Add marks of the student
    _marks.emplace(get_self(), [&](auto &marks) {
        marks.marks_id = _marks.available_primary_key();
        marks.student_id = student_id;
        marks.course_id = course_id;
        marks.subject_id = subject_id;
        marks.full_marks = full_marks;
        marks.marks_obtained = marks_obtained;
        marks.grade = grade;
    });

}
