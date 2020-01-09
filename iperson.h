#ifndef PERSON_H
#define PERSON_H


class Person
{

public:
    explicit Person();

    virtual void add()  = 0;
    virtual void edit()  = 0;


    virtual ~Person();


};

#endif // PERSON_H
