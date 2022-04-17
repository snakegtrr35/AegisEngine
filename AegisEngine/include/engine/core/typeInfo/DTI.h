#pragma once

#include "typeInfo.h"


#define OBJECT_TYPE_INFO(_class_)\
public:\
    static aegis::typeInfo::TypeInfo* getDTI();\
    aegis::uint32 virtual GetTypeId();\

#define ABSTRACT_OBJECT_TYPE_INFO(_class_)\
    OBJECT_TYPE_INFO(_class_)\

#define BASE_OBJECT_TYPE_INFO(_class_)\
public:\
    static aegis::typeInfo::TypeInfo* getDTI();\



#define IMPLEMENT_OBJECT_TYPE_INFO(_base_class_, _class_)\
    using namespace aegis::typeInfo;\
    aegis::typeInfo::TypeInfo* _class_::getDTI()\
    {\
        static aegis::typeInfo::TypeInfo DTI(_base_class_::getDTI(), #_class_, new IClassCreater<_class_>());\
        \
        return &DTI;\
    }\
    aegis::uint32 _class_::GetTypeId()\
    {\
        return this->getDTI()->GetTypeID();\
    }\
    
    //aegis::typeInfo::TypeInfo _class_::DTI(_base_class_::getDTI(), #_class_, new IClassCreater<_class_>());\


#define IMPLEMENT_BASE_OBJECT_TYPE_INFO(_class_)\
    using namespace aegis::typeInfo;\
    aegis::typeInfo::TypeInfo* _class_::getDTI()\
    {\
        static aegis::typeInfo::TypeInfo DTI(nullptr, #_class_, nullptr, true);\
        \
        return &DTI;\
    }\
    //aegis::typeInfo::TypeInfo _class_::DTI(nullptr, #_class_, nullptr, true);\


#define IMPLEMENT_ABSTRACT_OBJECT_TYPE_INFO(_base_class_, _class_)\
    using namespace aegis::typeInfo;\
    aegis::typeInfo::TypeInfo* _class_::getDTI()\
    {\
        static aegis::typeInfo::TypeInfo DTI(_base_class_::getDTI(), #_class_, nullptr, true);\
        \
        return &DTI;\
    }\
    aegis::uint32 _class_::GetTypeId()\
    {\
        return this->getDTI()->GetTypeID();\
    }\
    //aegis::typeInfo::TypeInfo _class_::DTI(_base_class_::getDTI(), #_class_, nullptr, true);\
