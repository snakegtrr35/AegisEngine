#pragma once

#include "typeInfo.h"

// 下記以外のAegisObjectの継承クラス
#define OBJECT_TYPE_INFO(_base_class_, _class_)\
public:\
    using Base = _base_class_;\
    static aegis::typeInfo::TypeInfo* getDTI();\
    aegis::uint32 virtual GetTypeId();\

// 抽象クラスのDTI
#define ABSTRACT_OBJECT_TYPE_INFO(_base_class_, _class_)\
    OBJECT_TYPE_INFO(_base_class_, _class_)\

// 全ての基底クラスのDTI
#define BASE_OBJECT_TYPE_INFO(_class_)\
public:\
    static aegis::typeInfo::TypeInfo* getDTI();\



//static_assert(std::is_base_of<_base_class_, _class_>::value); \

// OBJECT_TYPE_INFO の実体
#define IMPLEMENT_OBJECT_TYPE_INFO(_base_class_, _class_)\
    static_assert(std::is_base_of<_base_class_, _class_>::value\
                && std::is_convertible<_class_*, _base_class_*>::value);\
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

// BASE_OBJECT_TYPE_INFO の実体
#define IMPLEMENT_BASE_OBJECT_TYPE_INFO(_class_)\
    using namespace aegis::typeInfo;\
    aegis::typeInfo::TypeInfo* _class_::getDTI()\
    {\
        static aegis::typeInfo::TypeInfo DTI(nullptr, #_class_, nullptr, true);\
        \
        return &DTI;\
    }\
    //aegis::typeInfo::TypeInfo _class_::DTI(nullptr, #_class_, nullptr, true);\


// ABSTRACT_OBJECT_TYPE_INFO の実体
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
